/*
 * file:   clipLine.cpp
 * author: A trusty code generator
 * date:   Tue, 28 Feb 2017 10:45:57
 *
 * This file was autogenerated, do not modify.
 * See LICENSE for full modification and redistribution details.
 * Copyright 2017 The OpenCV Foundation
 */
#include <string>
#include <vector>
#include <cassert>
#include <exception>
#include <opencv2/matlab/bridge.hpp>
#include <opencv2/imgproc.hpp>
using namespace cv;
using namespace matlab;
using namespace bridge;

/*
 * clipLine
 * bool clipLine(Rect imgRect, Point pt1, Point pt2);
 * Gateway routine
 *   nlhs - number of return arguments
 *   plhs - pointers to return arguments
 *   nrhs - number of input arguments
 *   prhs - pointers to input arguments
 */
void mexFunction(int nlhs, mxArray* plhs[],
                 int nrhs, const mxArray* prhs[]) {

  // parse the inputs
  ArgumentParser parser("clipLine");
  parser.addVariant("clipLine", 3, 0);
  MxArrayVector sorted = parser.parse(MxArrayVector(prhs, prhs+nrhs));

  // setup
  BridgeVector inputs(sorted.begin(), sorted.end());
  BridgeVector outputs(3);

  
  // unpack the arguments
  Rect imgRect = inputs[0].toRect();
  Point pt1 = inputs[1].toPoint();
  Point pt2 = inputs[2].toPoint();
  bool retval;

  // call the opencv function
  // [out =] namespace.fun(src1, ..., srcn, dst1, ..., dstn, opt1, ..., optn);
  try {
    retval = cv::clipLine(imgRect, pt1, pt2);
  } catch(cv::Exception& e) {
    error(std::string("cv::exception caught: ").append(e.what()).c_str());
  } catch(std::exception& e) {
    error(std::string("std::exception caught: ").append(e.what()).c_str());
  } catch(...) {
    error("Uncaught exception occurred in clipLine");
  }
  
  // assign the outputs into the bridge
  outputs[0] = retval;
  outputs[1] = pt1;
  outputs[2] = pt2;


  // push the outputs back to matlab
  for (size_t n = 0; n < static_cast<size_t>(std::max(nlhs,1)); ++n) {
    plhs[n] = outputs[n].toMxArray().releaseOwnership();
  }
}