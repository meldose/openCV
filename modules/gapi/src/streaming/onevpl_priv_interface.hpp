// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2021 Intel Corporation

#ifndef GAPI_STREAMING_ONEVPL_PRIV_INTERFACE_HPP
#define GAPI_STREAMING_ONEVPL_PRIV_INTERFACE_HPP

#include <opencv2/gapi/garg.hpp>
#include <opencv2/gapi/streaming/meta.hpp>
#include <opencv2/gapi/streaming/onevpl_source.hpp>
#include <opencv2/gapi/streaming/onevpl_cfg_params.hpp>

namespace cv {
namespace gapi {
namespace wip {
struct OneVPLSource::IPriv
{
    virtual ~IPriv() {}
    
    virtual bool pull(cv::gapi::wip::Data& data) = 0;
    virtual GMetaArg descr_of() const = 0;
};
} // namespace wip
} // namespace gapi
} // namespace cv

#endif // GAPI_STREAMING_ONEVPL_PRIV_INTERFACE_HPP
