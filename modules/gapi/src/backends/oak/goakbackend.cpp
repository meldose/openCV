// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2021 Intel Corporation

#include <opencv2/gapi/gkernel.hpp> // GKernelPackage

#ifdef HAVE_OAK

#include <cstring>
#include <unordered_set>
#include <algorithm> // any_of
#include <functional> // reference_wrapper

#include <api/gbackend_priv.hpp>
#include <backends/common/gbackend.hpp>

#include <opencv2/gapi/streaming/meta.hpp> // streaming::meta_tag

#include "depthai/depthai.hpp"

#include <opencv2/gapi/oak/oak.hpp>
#include "oak_media_adapter.hpp"

namespace cv { namespace gimpl {

// Forward declaration
class GOAKContext;
struct OAKNodeInfo;

class GOAKExecutable final: public GIslandExecutable {
    friend class GOAKContext;
    virtual void run(std::vector<InObj>&&,
                     std::vector<OutObj>&&) override {
        GAPI_Assert(false && "Not implemented");
    }

    virtual void run(GIslandExecutable::IInput &in,
                     GIslandExecutable::IOutput &out) override;

    void LinkToParentHelper(ade::NodeHandle handle,
                            const std::vector<ade::NodeHandle>& nodes);

    class ExtractTypeHelper : protected dai::Node {
    public:
        using Input = dai::Node::Input;
        using Output = dai::Node::Output;
        using InputPtr = dai::Node::Input*;
        using OutputPtr = dai::Node::Output*;
    };

    struct OAKNodeInfo {
        std::shared_ptr<dai::Node> node = nullptr;
        bool processed = false;
        std::vector<ExtractTypeHelper::InputPtr> inputs = {};
        std::vector<ExtractTypeHelper::OutputPtr> outputs = {};
    };

    std::reference_wrapper<cv::GArg> packArg(const GArg &arg, std::vector<ExtractTypeHelper::InputPtr>& oak_ins,
                                             std::vector<cv::GArg>& oak_wrapped_args);
    void outArg(const RcDesc &rc, std::vector<ExtractTypeHelper::OutputPtr>& oak_outs);

    const ade::Graph& m_g;
    GModel::ConstGraph m_gm;
    cv::GCompileArgs m_args;

    std::unordered_map<ade::NodeHandle,
                       OAKNodeInfo,
                       ade::HandleHasher<ade::Node>> m_oak_nodes;

    // Will be reworked later when XLinkIn will be introduced as input
    std::shared_ptr<dai::node::ColorCamera> m_camera_input;
    cv::Size m_camera_wh;

    // Backend outputs
    std::vector<std::shared_ptr<dai::node::XLinkOut>> m_xlink_outputs;
    std::vector<std::shared_ptr<dai::DataOutputQueue>> m_out_queues;
    std::vector<std::string> m_out_queue_names;

    // Backend inputs
    std::vector<std::pair<std::string, dai::Buffer>> m_in_queues;

    // Note: dai::Pipeline should be the only one for the whole pipeline,
    // so there is no way to insert any non-OAK node in graph between other OAK nodes.
    // The only heterogeneous case possible is if we insert other backends after or before
    // OAK island.
    std::unique_ptr<dai::Device> m_device;
    std::unique_ptr<dai::Pipeline> m_pipeline;

    std::vector<cv::GArg> m_oak_wrapped_args;

public:
    GOAKExecutable(const ade::Graph& g,
                   const cv::GCompileArgs& args,
                   const std::vector<ade::NodeHandle>& nodes,
                   const std::vector<cv::gimpl::Data>& ins_data,
                   const std::vector<cv::gimpl::Data>& outs_data);
    ~GOAKExecutable() = default;

    // FIXME: could it reshape?
    virtual bool canReshape() const override { return false; }
    virtual void reshape(ade::Graph&, const GCompileArgs&) override {
        GAPI_Assert(false && "GOAKExecutable::reshape() is not supported");
    }

    virtual void handleNewStream() override;
    virtual void handleStopStream() override;
};

class GOAKContext {
public:
    // FIXME: make private?
    using Input = GOAKExecutable::ExtractTypeHelper::Input;
    using Output = GOAKExecutable::ExtractTypeHelper::Output;
    using InputPtr = GOAKExecutable::ExtractTypeHelper::Input*;
    using OutputPtr = GOAKExecutable::ExtractTypeHelper::Output*;

    GOAKContext(const std::unique_ptr<dai::Pipeline>& pipeline,
                const cv::Size& camera_size,
                std::vector<std::reference_wrapper<cv::GArg>>& args,
                std::vector<OutputPtr>& results);

    // Generic accessor API
    template<typename T>
    T& inArg(int input) { return m_args.at(input).get().get<T>(); }

    // FIXME: consider not using raw pointers
    InputPtr& in(int input);
    OutputPtr& out(int output);

    const std::unique_ptr<dai::Pipeline>& pipeline();
    const cv::Size& camera_size() const;

private:
    const std::unique_ptr<dai::Pipeline>& m_pipeline;
    const cv::Size& m_camera_size;
    std::vector<std::reference_wrapper<cv::GArg>>& m_args;
    std::vector<OutputPtr>& m_outputs;
};

GOAKContext::GOAKContext(const std::unique_ptr<dai::Pipeline>& pipeline,
                         const cv::Size& camera_size,
                         std::vector<std::reference_wrapper<cv::GArg>>& args,
                         std::vector<OutputPtr>& results)
    : m_pipeline(pipeline), m_camera_size(camera_size), m_args(args), m_outputs(results) {}

const std::unique_ptr<dai::Pipeline>& GOAKContext::pipeline() {
    return m_pipeline;
}

const cv::Size& GOAKContext::camera_size() const {
    return m_camera_size;
}

GOAKContext::InputPtr& GOAKContext::in(int input) {
    return inArg<GOAKContext::InputPtr>(input);
}

GOAKContext::OutputPtr& GOAKContext::out(int output) {
    return m_outputs.at(output);
}

namespace detail {
template<class T> struct get_in;
template<> struct get_in<cv::GFrame> {
    static GOAKContext::InputPtr& get(GOAKContext &ctx, int idx) { return ctx.in(idx); }
};
template<class T> struct get_in {
    static T get(GOAKContext &ctx, int idx) { return ctx.inArg<T>(idx); }
};
// FIXME: add support of other types

template<class T> struct get_out;
template<> struct get_out<cv::GFrame> {
    static GOAKContext::OutputPtr& get(GOAKContext &ctx, int idx) { return ctx.out(idx); }
};
template<typename U> struct get_out<cv::GArray<U>> {
    static GOAKContext::OutputPtr& get(GOAKContext &ctx, int idx) { return ctx.out(idx); }
};
// FIXME: add support of other types

struct OAKKernelParams {
    const std::unique_ptr<dai::Pipeline>& pipeline;
    const cv::Size& camera_wh;
    std::vector<std::pair<std::string, dai::Buffer>>& m_in_queues;
};

template<typename, typename, typename>
struct OAKCallHelper;

template<typename Impl, typename... Ins, typename... Outs>
struct OAKCallHelper<Impl, std::tuple<Ins...>, std::tuple<Outs...> > {
    template<int... IIs, int... OIs>
    static std::shared_ptr<dai::Node> construct_impl(  GOAKContext &ctx
                                                     , std::vector<std::pair<std::string,
                                                                             dai::Buffer>>& in_queues_params
                                                     , cv::detail::Seq<IIs...>
                                                     , cv::detail::Seq<OIs...>) {
        return Impl::put(OAKKernelParams{ctx.pipeline(),
                                         ctx.camera_size(),
                                         in_queues_params},
                         get_in<Ins>::get(ctx, IIs)...,
                         get_out<Outs>::get(ctx, OIs)...);
    }

    static std::shared_ptr<dai::Node> construct(GOAKContext &ctx,
                                                std::vector<std::pair<std::string,
                                                                      dai::Buffer>>& in_queues_params) {
        return construct_impl(ctx,
                              in_queues_params,
                              typename cv::detail::MkSeq<sizeof...(Ins)>::type(),
                              typename cv::detail::MkSeq<sizeof...(Outs)>::type());
    }
};

} // namespace detail

struct GOAKKernel {
    using F = std::function<std::shared_ptr<dai::Node>(GOAKContext&,
                                                       std::vector<std::pair<std::string, dai::Buffer>>&)>;
    explicit GOAKKernel(const F& f) : m_f(f) {}
    const F m_f;
};

struct OAKComponent
{
    static const char *name() { return "OAK Component"; }
    GOAKKernel k;
};

}} // namespace gimpl // namespace cv

using OAKGraph = ade::TypedGraph
    < cv::gimpl::OAKComponent
    // FIXME: extend
    >;

using ConstOAKGraph = ade::ConstTypedGraph
    < cv::gimpl::OAKComponent
    // FIXME: extend
    >;

// This function links dai operation nodes - parent's output to child's input.
// It utilizes G-API graph to search for operation's node it's previous operation in graph
// when links them in dai graph.
void cv::gimpl::GOAKExecutable::LinkToParentHelper(ade::NodeHandle handle,
                                                   const std::vector<ade::NodeHandle>& nodes)
{
    ade::NodeHandle parent;
    for (const auto& data_nh : handle.get()->inNodes()) {
        // data_nh - node's input data
        // need to find which other node produces that data
        for (const auto& nh : nodes) {
            if (m_gm.metadata(nh).get<NodeType>().t == NodeType::OP) {
                for (const auto& outdatah : nh.get()->outNodes()) {
                    if (data_nh == outdatah) {
                        parent = nh;
                    }
                }
            }
        }
    }
    GAPI_Assert(m_oak_nodes.find(parent) != m_oak_nodes.end());
    GAPI_Assert(m_oak_nodes[handle].inputs.size() ==
                m_oak_nodes[parent].outputs.size());
    for (size_t i = 0; i < m_oak_nodes[handle].inputs.size(); ++i) {
        m_oak_nodes[parent].outputs[i]->link(*(m_oak_nodes[handle].inputs[i]));
    }
}

std::reference_wrapper<cv::GArg>
cv::gimpl::GOAKExecutable::packArg(const GArg &arg,
                                   std::vector<ExtractTypeHelper::InputPtr>& oak_ins,
                                   std::vector<cv::GArg>& oak_wrapped_args) {
    if (arg.kind != cv::detail::ArgKind::GOBJREF) {
        GAPI_Assert(   arg.kind != cv::detail::ArgKind::GMAT
                    && arg.kind != cv::detail::ArgKind::GSCALAR
                    && arg.kind != cv::detail::ArgKind::GARRAY
                    && arg.kind != cv::detail::ArgKind::GOPAQUE
                    && arg.kind != cv::detail::ArgKind::GFRAME);
        // All other cases - pass as-is, with no transformations to
        // GArg contents.
        return const_cast<cv::GArg&>(arg);
    }
    const cv::gimpl::RcDesc &ref = arg.get<cv::gimpl::RcDesc>();
    switch (ref.shape) {
    case GShape::GFRAME:
        oak_ins.push_back(nullptr);
        oak_wrapped_args.push_back(GArg(oak_ins.back()));
        return oak_wrapped_args.back();
        break;
    default:
        util::throw_error(std::logic_error("Unsupported GShape type in OAK backend"));
        break;
    }
}

void cv::gimpl::GOAKExecutable::outArg(const RcDesc &rc,
                                       std::vector<ExtractTypeHelper::OutputPtr>& oak_outs) {
    switch (rc.shape) {
    case GShape::GFRAME:
        oak_outs.push_back(nullptr);
        break;
    case GShape::GARRAY:
        oak_outs.push_back(nullptr);
        break;
    default:
        util::throw_error(std::logic_error("Unsupported GShape type in OAK backend"));
        break;
    }
}

cv::gimpl::GOAKExecutable::GOAKExecutable(const ade::Graph& g,
                                          const cv::GCompileArgs &args,
                                          const std::vector<ade::NodeHandle>& nodes,
                                          const std::vector<cv::gimpl::Data>& ins_data,
                                          const std::vector<cv::gimpl::Data>& outs_data)
    : m_g(g), m_gm(m_g), m_args(args),
      m_device(nullptr), m_pipeline(new dai::Pipeline)
    {
        // FIXME: change the hard-coded behavior (XLinkIn path)
        auto camRgb = m_pipeline->create<dai::node::ColorCamera>();
        // FIXME: extract camera compile arguments here and properly convert them for dai
        camRgb->setBoardSocket(dai::CameraBoardSocket::RGB);
        camRgb->setResolution(dai::ColorCameraProperties::SensorResolution::THE_1080_P);

        // Set camera output. Fixme: consider working with other camera outputs
        m_camera_input = camRgb;
        // FIXME: change when other camera censors are introduced
        std::tuple<int, int> video_size = camRgb->getVideoSize();
        m_camera_wh = cv::Size{std::get<0>(video_size), std::get<1>(video_size)};

        // Prepare XLinkOut nodes for each output object in graph
        for (size_t i = 0; i < outs_data.size(); ++i) {
            auto xout = m_pipeline->create<dai::node::XLinkOut>();
            std::string xout_name = "xout" + std::to_string(i);
            m_out_queue_names.push_back(xout_name);
            xout->setStreamName(xout_name);
            m_xlink_outputs.push_back(xout);
        }

        // Reserve for all wrapped GArgs
        size_t garg_size = 0;
        for (const auto& nh : nodes) {
            if (m_gm.metadata(nh).get<NodeType>().t == NodeType::OP) {
                garg_size += m_gm.metadata(nh).get<Op>().args.size();
            }
        }
        m_oak_wrapped_args.reserve(garg_size);

        size_t curr_arg_idx = 0;

        // Create OAK node for each node in this backend
        for (const auto& nh : nodes) {
            if (m_gm.metadata(nh).get<NodeType>().t == NodeType::OP) {
                const auto& op = m_gm.metadata(nh).get<Op>();
                const auto &u = ConstOAKGraph(m_g).metadata(nh).get<OAKComponent>();
                // pass kernel input args and compile args to prepare OAK node and
                // store it to link later
                m_oak_nodes[nh] = {};
                m_oak_nodes[nh].inputs.reserve(op.args.size());
                m_oak_nodes[nh].outputs.reserve(op.outs.size());

                std::vector<std::reference_wrapper<cv::GArg>> in_ctx_args;
                in_ctx_args.reserve(op.args.size());
                for (auto &op_arg : op.args) in_ctx_args.push_back(packArg(op_arg,
                                                                           m_oak_nodes[nh].inputs,
                                                                           m_oak_wrapped_args));
                for (auto &&op_out : op.outs) outArg(op_out, m_oak_nodes[nh].outputs);
                GAPI_Assert(!m_oak_nodes[nh].inputs.empty());
                GAPI_Assert(!m_oak_nodes[nh].outputs.empty());

                GOAKContext ctx(m_pipeline, m_camera_wh, in_ctx_args, m_oak_nodes[nh].outputs);
                m_oak_nodes[nh].node = u.k.m_f(ctx, m_in_queues);
                GAPI_Assert(m_oak_nodes[nh].node != nullptr);

                // Restore wrapped input references
                // FIXME: where is a problem somewhere - should work w/o this hack
                for (size_t i = 0; i < m_oak_nodes[nh].inputs.size(); ++i) {
                    m_oak_nodes[nh].inputs[i] = m_oak_wrapped_args.at(curr_arg_idx + i).get<ExtractTypeHelper::InputPtr>();
                }
                curr_arg_idx += op.args.size();
            }
        }

        // Check that all inputs and outputs are properly filled after constructing kernels
        // to then link it together
        for (const auto& node : m_oak_nodes) {
            auto el = node.second;
            if (std::any_of(el.inputs.cbegin(), el.inputs.cend(),
                            [](ExtractTypeHelper::InputPtr ptr) {
                    return ptr == nullptr;
                })) {
                GAPI_Assert(false && "DAI input are not set");
            }
            if (std::any_of(el.outputs.cbegin(), el.outputs.cend(),
                            [](ExtractTypeHelper::OutputPtr ptr) {
                    return ptr == nullptr;
                })) {
                GAPI_Assert(false && "DAI outputs are not set");
            }
        }

        // Properly link all nodes
        // 1. Link input nodes to camera
        for (const auto& d : ins_data)
        {
            for (const auto& nh : nodes)
            {
                if (m_gm.metadata(nh).contains<cv::gimpl::Op>())
                {
                    for (const auto& indata : nh.get()->inNodes()) {
                        auto rc = m_gm.metadata(indata).get<cv::gimpl::Data>().rc;
                        if (rc == d.rc)
                        {
                            GAPI_Assert(m_oak_nodes.find(nh) != m_oak_nodes.end());
                            GAPI_Assert(m_oak_nodes[nh].inputs.size() == 1);
                            // FIXME: covert other camera outputs
                            m_camera_input->video.link(*(m_oak_nodes[nh].inputs[0]));
                            m_oak_nodes[nh].processed = true;
                        }
                    }
                }
            }
        }

        // 2. Link output nodes to XLinkOut nodes
        size_t out_counter = 0;
        for (const auto& d : outs_data)
        {
            for (const auto& nh : nodes)
            {
                if (m_gm.metadata(nh).contains<cv::gimpl::Op>())
                {
                    for (const auto& outdata : nh.get()->outNodes()) {
                        auto rc = m_gm.metadata(outdata).get<cv::gimpl::Data>().rc;
                        if (rc == d.rc)
                        {
                            GAPI_Assert(m_oak_nodes.find(nh) != m_oak_nodes.end());
                            GAPI_Assert(m_oak_nodes[nh].outputs.size() == 1);
                            GAPI_Assert(out_counter < m_xlink_outputs.size());
                            m_oak_nodes[nh].outputs[0]->link(m_xlink_outputs[out_counter++]->input);

                            if (!m_oak_nodes[nh].processed) {
                                LinkToParentHelper(nh, nodes);
                                m_oak_nodes[nh].processed = true;
                            }
                        }
                    }
                }
            }
        }

        // 3. Link internal nodes to their parents
        for (const auto& nh : nodes) {
            if (m_gm.metadata(nh).get<NodeType>().t == NodeType::OP) {
                GAPI_Assert(m_oak_nodes.find(nh) != m_oak_nodes.end());
                if (!m_oak_nodes[nh].processed) {
                    LinkToParentHelper(nh, nodes);
                    m_oak_nodes[nh].processed = true;
                }
            }
        }

        m_device = std::unique_ptr<dai::Device>(new dai::Device(*m_pipeline));

        // Prepare all output queues
        for (size_t i = 0; i < outs_data.size(); ++i) {
            // FIXME: add queue parameters
            m_out_queues.push_back(m_device->getOutputQueue(m_out_queue_names[i], 30, true));
        }
    }

void cv::gimpl::GOAKExecutable::handleNewStream() {
    // do nothing
}

void cv::gimpl::GOAKExecutable::handleStopStream() {
    // do nothing
}

void cv::gimpl::GOAKExecutable::run(GIslandExecutable::IInput  &in,
                                    GIslandExecutable::IOutput &out) {
    const auto in_msg = in.get();

    if (cv::util::holds_alternative<cv::gimpl::EndOfStream>(in_msg)) {
        out.post(cv::gimpl::EndOfStream{});
        return;
    }

    for (size_t i = 0; i < m_in_queues.size(); ++i) {
        auto q = m_device->getInputQueue(m_in_queues[i].first);
        q->send(m_in_queues[i].second);
    }

    for (size_t i = 0; i < m_out_queues.size(); ++i) {
        auto q = m_out_queues[i];
        // TODO: support other DAI types if needed
        auto oak_frame = q->get<dai::ImgFrame>();

        auto out_arg = out.get(i);

        switch(out_arg.index()) {
        case cv::GRunArgP::index_of<cv::MediaFrame*>():
            // FIXME: hard-coded NV12
            *cv::util::get<cv::MediaFrame*>(out_arg) =
                    cv::MediaFrame::Create<cv::gapi::oak::OAKMediaAdapter>(
                            cv::Size(static_cast<int>(oak_frame->getWidth()),
                                     static_cast<int>(oak_frame->getHeight())),
                            cv::MediaFormat::NV12,
                            std::move(oak_frame->getData()));
            break;
        case cv::GRunArgP::index_of<cv::detail::VectorRef>():
            cv::util::get<cv::detail::VectorRef>(out_arg).wref<uint8_t>() = std::move(oak_frame->getData());
            break;
        // FIXME: Add support for remaining types
        default:
            GAPI_Assert(false && "Unsupported type in OAK backend");
        }

        using namespace cv::gapi::streaming::meta_tag;
        cv::GRunArg::Meta meta;
        meta[timestamp] = oak_frame->getTimestamp();
        meta[seq_id]    = oak_frame->getSequenceNum();

        out.meta(out_arg, meta);
        out.post(std::move(out_arg));
    }
}

// Built-in kernels for OAK /////////////////////////////////////////////////////

class GOAKBackendImpl final : public cv::gapi::GBackend::Priv {
    virtual void unpackKernel(ade::Graph            &graph,
                              const ade::NodeHandle &op_node,
                              const cv::GKernelImpl &impl) override {
        OAKGraph gm(graph);

        const auto &kimpl  = cv::util::any_cast<cv::gimpl::GOAKKernel>(impl.opaque);
        gm.metadata(op_node).set(cv::gimpl::OAKComponent{kimpl});
    }

    virtual EPtr compile(const ade::Graph &graph,
                         const cv::GCompileArgs &args,
                         const std::vector<ade::NodeHandle> &nodes,
                         const std::vector<cv::gimpl::Data>& ins_data,
                         const std::vector<cv::gimpl::Data>& outs_data) const override {
        cv::gimpl::GModel::ConstGraph gm(graph);
        // FIXME: pass streaming/non-streaming option to support non-camera case
        // NB: how could we have non-OAK source in streaming mode, then OAK backend in
        //     streaming mode but without camera input?
        if (!gm.metadata().contains<cv::gimpl::Streaming>()) {
            GAPI_Assert(false && "OAK backend only supports Streaming mode for now");
        }
        return EPtr{new cv::gimpl::GOAKExecutable(graph, args, nodes, ins_data, outs_data)};
    }
};

cv::gapi::GBackend cv::gapi::oak::backend() {
    static cv::gapi::GBackend this_backend(std::make_shared<GOAKBackendImpl>());
    return this_backend;
}

namespace cv {
namespace gimpl {
namespace oak {

namespace {
static dai::VideoEncoderProperties::Profile convertEncProfile(cv::gapi::oak::EncoderConfig::Profile pf) {
    switch (pf) {
        case cv::gapi::oak::EncoderConfig::Profile::H264_BASELINE:
            return dai::VideoEncoderProperties::Profile::H264_BASELINE;
        case cv::gapi::oak::EncoderConfig::Profile::H264_HIGH:
            return dai::VideoEncoderProperties::Profile::H264_HIGH;
        case cv::gapi::oak::EncoderConfig::Profile::H264_MAIN:
            return dai::VideoEncoderProperties::Profile::H264_MAIN;
        case cv::gapi::oak::EncoderConfig::Profile::H265_MAIN:
            return dai::VideoEncoderProperties::Profile::H265_MAIN;
        case cv::gapi::oak::EncoderConfig::Profile::MJPEG:
            return dai::VideoEncoderProperties::Profile::MJPEG;
        default:
            // basically unreachable
            GAPI_Assert("Unsupported encoder profile");
            return {};
    }
}
} // anonymous namespace

// Kernels ///////////////////////////////////////////////////////////////

template<class Impl, class K>
class GOAKKernelImpl: public detail::OAKCallHelper<Impl, typename K::InArgs, typename K::OutArgs>
                    , public cv::detail::KernelTag {
    using P = detail::OAKCallHelper<Impl, typename K::InArgs, typename K::OutArgs>;
public:
    using API = K;
    static cv::gapi::GBackend   backend() { return cv::gapi::oak::backend();  }
    static GOAKKernel kernel()  { return GOAKKernel(&P::construct); }
};

#define GAPI_OAK_KERNEL(Name, API) \
    struct Name: public cv::gimpl::oak::GOAKKernelImpl<Name, API>

namespace {
GAPI_OAK_KERNEL(GOAKEncFrame, cv::gapi::oak::GEncFrame) {
    static std::shared_ptr<dai::Node> put(const cv::gimpl::detail::OAKKernelParams& params,
                                          GOAKContext::InputPtr& in,
                                          const cv::gapi::oak::EncoderConfig& cfg,
                                          GOAKContext::OutputPtr& out) {
        auto videoEnc = params.pipeline->create<dai::node::VideoEncoder>();

        // FIXME: convert all the parameters to dai
        videoEnc->setDefaultProfilePreset(cfg.width, cfg.height,
                                          cfg.frameRate,
                                          convertEncProfile(cfg.profile));

        in = &(videoEnc->input);
        out = &(videoEnc->bitstream);

        return videoEnc;
    }
};

GAPI_OAK_KERNEL(GOAKSobelXY, cv::gapi::oak::GSobelXY) {
    static std::shared_ptr<dai::Node> put(const cv::gimpl::detail::OAKKernelParams& params,
                                          GOAKContext::InputPtr& in,
                                          const cv::Mat& hk,
                                          const cv::Mat& vk,
                                          GOAKContext::OutputPtr& out) {
        auto edgeDetector = params.pipeline->create<dai::node::EdgeDetector>();

        edgeDetector->setMaxOutputFrameSize(params.camera_wh.width * params.camera_wh.height);

        auto xinEdgeCfg = params.pipeline->create<dai::node::XLinkIn>();
        xinEdgeCfg->setStreamName("sobel_cfg");

        auto mat2vec = [&](cv::Mat m) {
            std::vector<std::vector<int>> v(m.rows);
            for (int i = 0; i < m.rows; ++i)
            {
                m.row(i).reshape(1,1).copyTo(v[i]);
            }
            return v;
        };

        dai::EdgeDetectorConfig cfg;
        cfg.setSobelFilterKernels(mat2vec(hk), mat2vec(vk));

        xinEdgeCfg->out.link(edgeDetector->inputConfig);

        params.m_in_queues.push_back({"sobel_cfg", cfg});

        in = &(edgeDetector->inputImage);
        out = &(edgeDetector->outputImage);

        return edgeDetector;
    }
};
} // anonymous namespace

cv::gapi::GKernelPackage kernels();

cv::gapi::GKernelPackage kernels() {
    return cv::gapi::kernels< GOAKEncFrame
                            , GOAKSobelXY
                            >();
}

} // namespace oak
} // namespace gimpl
} // namespace cv

#else

namespace cv {
namespace gimpl {
namespace oak {

cv::gapi::GKernelPackage kernels();

cv::gapi::GKernelPackage kernels() {
    GAPI_Assert(false && "Built without OAK support");
    return {};
}

} // namespace oak
} // namespace gimpl
} // namespace cv

#endif // HAVE_OAK
