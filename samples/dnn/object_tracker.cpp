/*
For usage download models by following links
    For DaSiamRPN:
        network:     https://www.dropbox.com/s/rr1lk9355vzolqv/dasiamrpn_model.onnx?dl=0
        kernel_r1:   https://www.dropbox.com/s/999cqx5zrfi7w4p/dasiamrpn_kernel_r1.onnx?dl=0
        kernel_cls1: https://www.dropbox.com/s/qvmtszx5h339a0w/dasiamrpn_kernel_cls1.onnx?dl=0
    For NanoTrack:
        nanotrack_backbone: https://github.com/HonglinChu/SiamTrackers/blob/master/NanoTrack/models/nanotrackv2/nanotrack_backbone_sim.onnx
        nanotrack_headneck: https://github.com/HonglinChu/SiamTrackers/blob/master/NanoTrack/models/nanotrackv2/nanotrack_head_sim.onnx
    For VitTrack:
        vitTracker: https://github.com/opencv/opencv_zoo/raw/fef72f8fa7c52eaf116d3df358d24e6e959ada0e/models/object_tracking_vittrack/object_tracking_vittrack_2023sep.onnx
*/

#include <iostream>
#include <cmath>

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include "common.hpp"

using namespace cv;
using namespace std;
using namespace cv::dnn;

const string about = "Use this script for Object Tracking using OpenCV. \n\n"
        "To run:\n"
            "\t Nano: \n"
                "\t\t e.g: ./example_dnn_object_tracker nano --backbone=<path to nanotrack_backbone onnx model> --headneck=<path to nanotrack_head onnx model>\n\n"
            "\t vit: \n"
                "\t\t e.g: ./example_dnn_object_tracker vit --vit_net=<path to vitTracker onnx model>\n\n"
            "\t dasiamrpn: \n"
                "\t\t e.g: ./example_dnn_object_tracker dasiamrpn --dasiamrpn_net=<path to dasiamrpn_model onnx model> --kernel_r1=<path to dasiamrpn_kernel_r1 onnx model> --kernel_cls1=<path to dasiamrpn_kernel_cls1 onnx model>\n\n";

const string param_keys =
        "{ help     h    |                            | Print help message }"
        "{ @alias        |                            | An alias name of model to extract preprocessing parameters from models.yml file. }"
        "{ input    i    |                            | Full path to input video folder, the specific camera index. (empty for camera 0) }"
        "{ backbone      |         backbone.onnx      | Path to onnx model of backbone.onnx for nano}"
        "{ headneck      |         headneck.onnx      | Path to onnx model of headneck.onnx for nano}"
        "{ vit_net       |       vitTracker.onnx      | Path to onnx model of vitTracker.onnx for vit}"
        "{ tracking_thrs |             0.3            | Tracking score threshold. If a bbox of score >= 0.3, it is considered as found }"
        "{ dasiamrpn_net |     dasiamrpn_model.onnx   | Path to onnx model of net for dasiamrpn}"
        "{ kernel_r1     |  dasiamrpn_kernel_r1.onnx  | Path to onnx model of kernel_cls1 for dasiamrpn}"
        "{ kernel_cls1   | dasiamrpn_kernel_cls1.onnx | Path to onnx model of kernel_r1 for dasiamrpn}";

const string backend_keys = format(
    "{ backend | default | Choose one of computation backends: "
                    "default: automatically (by default), "
                    "openvino: Intel's Deep Learning Inference Engine (https://software.intel.com/openvino-toolkit), "
                    "opencv: OpenCV implementation, "
                    "vkcom: VKCOM, "
                    "cuda: CUDA, "
                    "webnn: WebNN }");

const string target_keys = format(
    "{ target | cpu | Choose one of target computation devices: "
                    "cpu: CPU target (by default), "
                    "opencl: OpenCL, "
                    "opencl_fp16: OpenCL fp16 (half-float precision), "
                    "vpu: VPU, "
                    "vulkan: Vulkan, "
                    "cuda: CUDA, "
                    "cuda_fp16: CUDA fp16 (half-float preprocess) }");

string keys = param_keys + backend_keys + target_keys;

static float getTrackingScore(Ptr<Tracker> tracker)
{
    // Try casting to TrackerDaSiamRPN
    if (Ptr<TrackerDaSiamRPN> trackerDaSiam = dynamic_pointer_cast<TrackerDaSiamRPN>(tracker))
    {
        return trackerDaSiam->getTrackingScore();
    }

    // Try casting to TrackerVit
    if (Ptr<TrackerVit> trackerVit = dynamic_pointer_cast<TrackerVit>(tracker))
    {
        return trackerVit->getTrackingScore();
    }

    // Try casting to TrackerVit
    if (Ptr<TrackerNano> trackerVit = dynamic_pointer_cast<TrackerNano>(tracker))
    {
        return trackerVit->getTrackingScore();
    }

    // If tracker type does not have getTrackingScore
    return -1;  // Return -1 or some other default value indicating no score available
}

static int trackObject(const string& windowName, Ptr<Tracker> tracker, const string& inputName)
{
    namedWindow(windowName, WINDOW_AUTOSIZE);
    FontFace fontFace("sans");
    int stdSize = 20;
    int stdWeight = 400;
    int stdImgSize = 512;
    int imgWidth = -1;
    int fontSize = 50;
    int fontWeight = 500;
    Rect selectRect;

    // Open a video file or an image file or a camera stream.
    VideoCapture cap;

    if (inputName.empty() || (isdigit(inputName[0]) && inputName.size() == 1))
    {
        int c = inputName.empty() ? 0 : inputName[0] - '0';
        cout << "Trying to open camera #" << c << " ..." << endl;
        if (!cap.open(c))
        {
            cout << "Capture from camera #" << c << " didn't work. Specify -i=<video> parameter to read from video file" << endl;
            return 2;
        }
    }
    else if (inputName.size())
    {
        string filePath = findFile(inputName);
        if (!cap.open(filePath))
        {
            cout << "Could not open: " << inputName << endl;
            return 2;
        }
    }

    Mat image;
    char key;

    for (;;)
    {
        cap >> image;
        if (image.empty())
        {
            cerr << "Can't capture frame. End of video stream?" << endl;
            return 2;
        }
        else if (imgWidth == -1){
            imgWidth = min(image.rows, image.cols);
            fontSize = (stdSize*imgWidth)/stdImgSize;
            fontWeight = (stdWeight*imgWidth)/stdImgSize;
        }
        const string label = "Press space bar to pause video to draw bounding box.";
        Rect r = getTextSize(Size(), label, Point(), fontFace, fontSize, fontWeight);
        r.height += 2 * fontSize; // padding
        r.width += 10; // padding
        rectangle(image, r, Scalar::all(255), FILLED);
        putText(image, label, Point(10, fontSize), Scalar(0,0,0), fontFace, fontSize, fontWeight);
        putText(image, "Press space bar after selecting.", Point(10, 2*fontSize), Scalar(0,0,0), fontFace, fontSize, fontWeight);
        imshow(windowName, image);
        key = waitKey(200);
        if (key == ' ')
        {
            selectRect = selectROI(windowName, image);
            break;
        }
        if (key == 27) // ESC key to exit
        {
            exit(0);
        }
    }

    Mat image_select = image.clone();

    cout << "ROI=" << selectRect << endl;

    tracker->init(image, selectRect);

    TickMeter tickMeter;

    for (int count = 0; ; ++count)
    {
        cap >> image;
        if (image.empty())
        {
            cerr << "Can't capture frame " << count << ". End of video stream?" << endl;
            break;
        }

        Rect rect;

        tickMeter.start();
        bool ok = tracker->update(image, rect);
        tickMeter.stop();

        float score = getTrackingScore(tracker);

        Mat render_image = image.clone();

        if (ok)
        {
            rectangle(render_image, rect, Scalar(0, 255, 0), 2);

            string timeLabel = format("Inference time: %.2f ms", tickMeter.getTimeMilli());
            string scoreLabel = format("Score: %f", score);
            Rect r = getTextSize(Size(), timeLabel, Point(), fontFace, fontSize, fontWeight);
            r.height += 2 * fontSize; // padding
            r.width += 10; // padding
            rectangle(render_image, r, Scalar::all(255), FILLED);
            putText(render_image, timeLabel, Point(10, fontSize), Scalar(0,0,0), fontFace, fontSize, fontWeight);
            putText(render_image, scoreLabel, Point(10, 2*fontSize), Scalar(0,0,0), fontFace, fontSize, fontWeight);
       }

        imshow(windowName, render_image);

        tickMeter.reset();

        int c = waitKey(100);
        if (c == 27 /*ESC*/)
            exit(0);
    }
    return 0;
}

static int run(int argc, char** argv)
{
    // Parse command line arguments.
    CommandLineParser parser(argc, argv, keys);

    if (parser.has("help"))
    {
        cout<<about<<endl;
        parser.printMessage();
        return 0;
    }

    string inputName = parser.get<String>("input");
    string modelName = parser.get<String>("@alias");
    string net, headneck, backbone, kernel_cls1, kernel_r1;

    int backend = getBackendID(parser.get<String>("backend"));
    int target = getTargetID(parser.get<String>("target"));

    if (modelName == "vit"){
        net = parser.get<String>("vit_net");
        float tracking_score_threshold = parser.get<float>("tracking_thrs");

        Ptr<TrackerVit> tracker;
        try
        {
            TrackerVit::Params params;
            params.net = findModel(net, "");
            params.backend = backend;
            params.target = target;
            params.tracking_score_threshold = tracking_score_threshold;
            tracker = TrackerVit::create(params);
        }
        catch (const cv::Exception& ee)
        {
            std::cerr << "Exception: " << ee.what() << std::endl;
            std::cout << "Can't load the network by using the following files:" << std::endl;
            std::cout << "net : " << net << std::endl;
            return 2;
        }

        trackObject("vitTracker", tracker, inputName);
    }
    else if (modelName == "nano"){
        backbone = parser.get<String>("backbone");
        headneck = parser.get<String>("headneck");

        Ptr<TrackerNano> tracker;
        try
        {
            TrackerNano::Params params;
            params.backbone = findModel(backbone, "");
            params.neckhead = findModel(headneck, "");
            params.backend = backend;
            params.target = target;
            tracker = TrackerNano::create(params);
        }
        catch (const cv::Exception& ee)
        {
            std::cerr << "Exception: " << ee.what() << std::endl;
            std::cout << "Can't load the network by using the following files:" << std::endl;
            std::cout << "backbone : " << backbone << std::endl;
            std::cout << "headneck : " << headneck << std::endl;
            return 2;
        }
        trackObject("nanoTracker", tracker, inputName);
        trackObject("DaSiamRPNTracker", tracker, inputName);
    }
    else if (modelName == "dasiamrpn"){
        net = parser.get<String>("dasiamrpn_net");
        kernel_cls1 = parser.get<String>("kernel_cls1");
        kernel_r1 = parser.get<String>("kernel_r1");

        Ptr<TrackerDaSiamRPN> tracker;
        try
        {
            TrackerDaSiamRPN::Params params;
            params.model = findModel(net, "");
            params.kernel_cls1 = findModel(kernel_cls1, "");
            params.kernel_r1 = findModel(kernel_r1, "");
            params.backend = backend;
            params.target = target;
            tracker = TrackerDaSiamRPN::create(params);
        }
        catch (const cv::Exception& ee)
        {
            cerr << "Exception: " << ee.what() << endl;
            cout << "Can't load the network by using the following files:" << endl;
            cout << "siamRPN : " << net << endl;
            cout << "siamKernelCL1 : " << kernel_cls1 << endl;
            cout << "siamKernelR1 : " << kernel_r1 << endl;
            return 2;
        }
        trackObject("DaSiamRPNTracker", tracker, inputName);
    }
    else{
        cout<<"Pass the valid alias. Choices are { nano, vit, dasiamrpn }"<<endl;
        return -1;
    }
    return 0;
}


int main(int argc, char **argv)
{
    try
    {
        return run(argc, argv);
    }
    catch (const exception& e)
    {
        cerr << "FATAL: C++ exception: " << e.what() << endl;
        return 1;
    }
}
