/*
 * file:   estimateAffinePartial2D.cpp
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
#include <opencv2/calib3d.hpp>
using namespace cv;
using namespace matlab;
using namespace bridge;

/*
 * estimateAffinePartial2D
 * Mat estimateAffinePartial2D(Mat from, Mat to, Mat inliers=Mat(), int method=RANSAC, double ransacReprojThreshold=3, size_t maxIters=2000, double confidence=0.99, size_t refineIters=10);
 * Gateway routine
 *   nlhs - number of return arguments
 *   plhs - pointers to return arguments
 *   nrhs - number of input arguments
 *   prhs - pointers to input arguments
 */
void mexFunction(int nlhs, mxArray* plhs[],
                 int nrhs, const mxArray* prhs[]) {

  // parse the inputs
  ArgumentParser parser("estimateAffinePartial2D");
  parser.addVariant("estimateAffinePartial2D", 2, 5, "method", "ransacReprojThreshold", "maxIters", "confidence", "refineIters");
  MxArrayVector sorted = parser.parse(MxArrayVector(prhs, prhs+nrhs));

  // setup
  BridgeVector inputs(sorted.begin(), sorted.end());
  BridgeVector outputs(2);

  
  // unpack the arguments
  Mat from = inputs[0].toMat();
  Mat to = inputs[1].toMat();
  int method = inputs[2].empty() ? (int) RANSAC : inputs[2].toInt();
  double ransacReprojThreshold = inputs[3].empty() ? (double) 3 : inputs[3].toDouble();
  size_t maxIters = inputs[4].empty() ? (size_t) 2000 : inputs[4].toSizeT();
  double confidence = inputs[5].empty() ? (double) 0.99 : inputs[5].toDouble();
  size_t refineIters = inputs[6].empty() ? (size_t) 10 : inputs[6].toSizeT();
  Mat inliers;
  Mat retval;

  // call the opencv function
  // [out =] namespace.fun(src1, ..., srcn, dst1, ..., dstn, opt1, ..., optn);
  try {
    retval = cv::estimateAffinePartial2D(from, to, inliers, method, ransacReprojThreshold, maxIters, confidence, refineIters);
  } catch(cv::Exception& e) {
    error(std::string("cv::exception caught: ").append(e.what()).c_str());
  } catch(std::exception& e) {
    error(std::string("std::exception caught: ").append(e.what()).c_str());
  } catch(...) {
    error("Uncaught exception occurred in estimateAffinePartial2D");
  }
  
  // assign the outputs into the bridge
  outputs[0] = retval;
  outputs[1] = inliers;


  // push the outputs back to matlab
  for (size_t n = 0; n < static_cast<size_t>(std::max(nlhs,1)); ++n) {
    plhs[n] = outputs[n].toMxArray().releaseOwnership();
  }
}