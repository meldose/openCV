/*
 * file:   ellipse2Poly.cpp
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
 * ellipse2Poly
 * void ellipse2Poly(Point center, Size axes, int angle, int arcStart, int arcEnd, int delta, vector_Point pts);
 * Gateway routine
 *   nlhs - number of return arguments
 *   plhs - pointers to return arguments
 *   nrhs - number of input arguments
 *   prhs - pointers to input arguments
 */
void mexFunction(int nlhs, mxArray* plhs[],
                 int nrhs, const mxArray* prhs[]) {

  // parse the inputs
  ArgumentParser parser("ellipse2Poly");
  parser.addVariant("ellipse2Poly", 6, 0);
  MxArrayVector sorted = parser.parse(MxArrayVector(prhs, prhs+nrhs));

  // setup
  BridgeVector inputs(sorted.begin(), sorted.end());
  BridgeVector outputs(1);

  
  // unpack the arguments
  Point center = inputs[0].toPoint();
  Size axes = inputs[1].toSize();
  int angle = inputs[2].toInt();
  int arcStart = inputs[3].toInt();
  int arcEnd = inputs[4].toInt();
  int delta = inputs[5].toInt();
  vector_Point pts;

  // call the opencv function
  // [out =] namespace.fun(src1, ..., srcn, dst1, ..., dstn, opt1, ..., optn);
  try {
    cv::ellipse2Poly(center, axes, angle, arcStart, arcEnd, delta, pts);
  } catch(cv::Exception& e) {
    error(std::string("cv::exception caught: ").append(e.what()).c_str());
  } catch(std::exception& e) {
    error(std::string("std::exception caught: ").append(e.what()).c_str());
  } catch(...) {
    error("Uncaught exception occurred in ellipse2Poly");
  }
  
  // assign the outputs into the bridge
  outputs[0] = pts;


  // push the outputs back to matlab
  for (size_t n = 0; n < static_cast<size_t>(std::max(nlhs,1)); ++n) {
    plhs[n] = outputs[n].toMxArray().releaseOwnership();
  }
}