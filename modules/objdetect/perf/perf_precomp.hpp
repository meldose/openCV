#ifdef __GNUC__
#  pragma GCC diagnostic ignored "-Wmissing-declarations"
#  if defined __clang__ || defined __APPLE__
#    pragma GCC diagnostic ignored "-Wmissing-prototypes"
#    pragma GCC diagnostic ignored "-Wextra"
#  endif
#endif

#ifndef OPENCV_PERF_PRECOMP_HPP
#define OPENCV_PERF_PRECOMP_HPP

#include "opencv2/ts.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"

#ifdef GTEST_CREATE_SHARED_LIBRARY
#error no modules except ts should have GTEST_CREATE_SHARED_LIBRARY defined
#endif

#endif
