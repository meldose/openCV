// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2022 Intel Corporation


#include "streaming/onevpl/engine/preproc_engine_interface.hpp"
#include "streaming/onevpl/engine/preproc/preproc_dispatcher.hpp"

#ifdef HAVE_ONEVPL
#include "streaming/onevpl/onevpl_export.hpp"
#include "streaming/onevpl/engine/preproc/preproc_engine.hpp"

#include "streaming/onevpl/accelerators/accel_policy_dx11.hpp"
#include "streaming/onevpl/accelerators/accel_policy_cpu.hpp"
#include "streaming/onevpl/accelerators/surface/surface.hpp"
#include "streaming/onevpl/cfg_param_device_selector.hpp"
#include "streaming/onevpl/cfg_params_parser.hpp"

#endif //HAVE_ONEVPL

#include "logger.hpp"

namespace cv {
namespace gapi {
namespace wip {

template<typename SpecificPreprocEngine, typename ...PreprocEngineArgs >
std::unique_ptr<SpecificPreprocEngine>
IPreprocEngine::create_preproc_engine_impl(const PreprocEngineArgs& ...) {
    GAPI_Assert(false && "Unsupported ");
}

template <>
std::unique_ptr<onevpl::VPPPreprocDispatcher>
IPreprocEngine::create_preproc_engine_impl(const std::string &device_id,
                                           const cv::util::optional<void*> &device_ptr,
                                           const cv::util::optional<void*> &context_ptr) {
    using namespace onevpl;
    cv::util::suppress_unused_warning(device_id);
    cv::util::suppress_unused_warning(device_ptr);
    cv::util::suppress_unused_warning(context_ptr);
    std::unique_ptr<VPPPreprocDispatcher> dispatcher(new VPPPreprocDispatcher);
#ifdef HAVE_ONEVPL
    if (device_ptr.value() && context_ptr.value()) {
        bool gpu_pp_is_created = false;
#ifdef HAVE_DIRECTX
#ifdef HAVE_D3D11
        GAPI_LOG_INFO(nullptr, "Device & Context detected: build GPU VPP preprocessing engine");
        // create GPU VPP preproc engine
        dispatcher->insert_worker<VPPPreprocEngine>(
                                std::unique_ptr<VPLAccelerationPolicy>{
                                        new VPLDX11AccelerationPolicy(
                                            std::make_shared<CfgParamDeviceSelector>(
                                                    device_ptr.value(),
                                                    device_id,
                                                    context_ptr.value(),
                                                    CfgParams{CfgParam::create_acceleration_mode("MFX_ACCEL_MODE_VIA_D3D11")}))
                                });
        GAPI_LOG_INFO(nullptr, "GPU VPP preprocessing engine created");
        gpu_pp_is_created = true;
#endif
#endif
        GAPI_Assert(gpu_pp_is_created && "VPP preproc for GPU is requested, but it is avaiable only for DX11 at now");
    } else {
        GAPI_LOG_INFO(nullptr, "Build CPU VPP preprocessing engine");
        dispatcher->insert_worker<VPPPreprocEngine>(
                        std::unique_ptr<VPLAccelerationPolicy>{
                                new VPLCPUAccelerationPolicy(
                                    std::make_shared<CfgParamDeviceSelector>(CfgParams{}))});
        GAPI_LOG_INFO(nullptr, "CPU VPP preprocessing engine created");
    }
#endif // HAVE_ONEVPL
    return dispatcher;
}

template <>
std::unique_ptr<onevpl::VPPPreprocDispatcher>
IPreprocEngine::create_preproc_engine_impl(const onevpl::Device &device,
                                           const onevpl::Context &context) {
    using namespace onevpl;
    cv::util::suppress_unused_warning(device);
    cv::util::suppress_unused_warning(context);
    std::unique_ptr<VPPPreprocDispatcher> dispatcher(new VPPPreprocDispatcher);
#ifdef HAVE_ONEVPL
    if (device.get_type() == onevpl::AccelType::DX11) {
        bool gpu_pp_is_created = false;
#ifdef HAVE_DIRECTX
#ifdef HAVE_D3D11
        GAPI_LOG_INFO(nullptr, "Device & Context detected: build GPU VPP preprocessing engine");
        // create GPU VPP preproc engine
        dispatcher->insert_worker<VPPPreprocEngine>(
                                std::unique_ptr<VPLAccelerationPolicy>{
                                        new VPLDX11AccelerationPolicy(
                                            std::make_shared<CfgParamDeviceSelector>(
                                                    device, context, CfgParams{}))
                                });
        GAPI_LOG_INFO(nullptr, "GPU VPP preprocessing engine created");
        gpu_pp_is_created = true;
#endif
#endif
        GAPI_Assert(gpu_pp_is_created && "VPP preproc for GPU is requested, but it is avaiable only for DX11 at now");
    } else {
        GAPI_LOG_INFO(nullptr, "Build CPU VPP preprocessing engine");
        dispatcher->insert_worker<VPPPreprocEngine>(
                        std::unique_ptr<VPLAccelerationPolicy>{
                                new VPLCPUAccelerationPolicy(
                                    std::make_shared<CfgParamDeviceSelector>(CfgParams{}))});
        GAPI_LOG_INFO(nullptr, "CPU VPP preprocessing engine created");
    }
#endif // HAVE_ONEVPL
    return dispatcher;
}


// Force instantiation
template
std::unique_ptr<onevpl::VPPPreprocDispatcher>
IPreprocEngine::create_preproc_engine_impl<onevpl::VPPPreprocDispatcher, const std::string &,
                                           const cv::util::optional<void*> &, const cv::util::optional<void*> &>
                                          (const std::string &device_id,
                                           const cv::util::optional<void*> &device_ptr,
                                           const cv::util::optional<void*> &context_ptr);
} // namespace wip
} // namespace gapi
} // namespace cv
