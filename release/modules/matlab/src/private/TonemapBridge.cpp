/*
 * file:   TonemapBridge.cpp
 * author: A trusty code generator
 * date:   Tue, 28 Feb 2017 10:45:57
 *
 * This file was autogenerated, do not modify.
 * See LICENSE for full modification and redistribution details.
 * Copyright 2017 The OpenCV Foundation
 */
#include <mex.h>
#include <vector>
#include <string>
#include <opencv2/matlab/map.hpp>
#include <opencv2/matlab/bridge.hpp>
#include <opencv2/core.hpp>
using namespace cv;
using namespace matlab;
using namespace bridge;

namespace {

typedef std::vector<Bridge> (*)(Tonemap&, const std::vector<Bridge>&) MethodSignature;


// wrapper for process() method
std::vector<Bridge> process(Tonemap& inst, const std::vector<Bridge>& inputs) {
  std::vector<Bridge> outputs(1);
  
  // unpack the arguments
  Mat src = inputs[0].toMat();
  Mat dst;

  // call the opencv function
  // [out =] namespace.fun(src1, ..., srcn, dst1, ..., dstn, opt1, ..., optn);
  try {
    inst.process(src, dst);
  } catch(cv::Exception& e) {
    error(std::string("cv::exception caught: ").append(e.what()).c_str());
  } catch(std::exception& e) {
    error(std::string("std::exception caught: ").append(e.what()).c_str());
  } catch(...) {
    error("Uncaught exception occurred in process");
  }
  
  // assign the outputs into the bridge
  outputs[0] = dst;

  return outputs;
}

// wrapper for getGamma() method
std::vector<Bridge> getGamma(Tonemap& inst, const std::vector<Bridge>& inputs) {
  std::vector<Bridge> outputs(1);
  
  // unpack the arguments
  float retval;

  // call the opencv function
  // [out =] namespace.fun(src1, ..., srcn, dst1, ..., dstn, opt1, ..., optn);
  try {
    retval = inst.getGamma();
  } catch(cv::Exception& e) {
    error(std::string("cv::exception caught: ").append(e.what()).c_str());
  } catch(std::exception& e) {
    error(std::string("std::exception caught: ").append(e.what()).c_str());
  } catch(...) {
    error("Uncaught exception occurred in getGamma");
  }
  
  // assign the outputs into the bridge
  outputs[0] = retval;

  return outputs;
}

// wrapper for setGamma() method
std::vector<Bridge> setGamma(Tonemap& inst, const std::vector<Bridge>& inputs) {
  std::vector<Bridge> outputs;
  
  // unpack the arguments
  float gamma = inputs[0].toFloat();

  // call the opencv function
  // [out =] namespace.fun(src1, ..., srcn, dst1, ..., dstn, opt1, ..., optn);
  try {
    inst.setGamma(gamma);
  } catch(cv::Exception& e) {
    error(std::string("cv::exception caught: ").append(e.what()).c_str());
  } catch(std::exception& e) {
    error(std::string("std::exception caught: ").append(e.what()).c_str());
  } catch(...) {
    error("Uncaught exception occurred in setGamma");
  }
  

  return outputs;
}

Map<std::string, MethodSignature> createMethodMap() {
  Map<std::string, MethodSignature> m;
  m["process"] = &process;
  m["getGamma"] = &getGamma;
  m["setGamma"] = &setGamma;

  return m;
}
static const Map<std::string, MethodSignature> methods = createMethodMap();

// map of created Tonemap instances. Don't trust the user to keep them safe...
static Map<void *, Tonemap> instances;

/*
 * Tonemap
 * Gateway routine
 *   nlhs - number of return arguments
 *   plhs - pointers to return arguments
 *   nrhs - number of input arguments
 *   prhs - pointers to input arguments
 */
void mexFunction(int nlhs, mxArray* plhs[],
                 int nrhs, const mxArray* prhs[]) {

  // parse the inputs
  Bridge method_name(prhs[0]);

  Bridge handle(prhs[1]);
  std::vector<Bridge> brhs(prhs+2, prhs+nrhs);

  // retrieve the instance of interest
  try {
    Tonemap& inst = instances.at(handle.address());
  } catch (const std::out_of_range& e) {
    mexErrMsgTxt("Invalid object instance provided");
  }

  // invoke the correct method on the data
  try {
    std::vector<Bridge> blhs = (*methods.at(method_name))(inst, brhs);
  } catch (const std::out_of_range& e) {
    mexErrMsgTxt("Unknown method specified");
  }



}

} // end namespace