/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#include "test_precomp.hpp"

namespace opencv_test { namespace {

enum { MINEIGENVAL=0, HARRIS=1, EIGENVALSVECS=2 };


#if 0   //set 1 to switch ON debug message
    #define TEST_MESSAGE( message )   std::cout << message;
    #define TEST_MESSAGEL( message, val)   std::cout << message << val << std::endl;
#else
    #define TEST_MESSAGE( message )
    #define TEST_MESSAGEL( message, val)
#endif

/////////////////////ref//////////////////////

struct greaterThanPtr
{
    bool operator () (const float * a, const float * b) const
    { return *a > *b; }
};

static void
test_goodFeaturesToTrack( InputArray _image, OutputArray _corners,
                              int maxCorners, double qualityLevel, double minDistance,
                              InputArray _mask, int blockSize, int gradientSize,
                              bool useHarrisDetector, double harrisK, OutputArray _corners_values)
{

    CV_Assert( qualityLevel > 0 && minDistance >= 0 && maxCorners >= 0 );
    CV_Assert( _mask.empty() || (_mask.type() == CV_8UC1 && _mask.sameSize(_image)) );


    Mat image = _image.getMat(), mask = _mask.getMat();
    int borderType = BORDER_DEFAULT;

    Mat eig, tmp, tt;

    eig.create( image.size(), CV_32F );

    if( useHarrisDetector )
        cornerHarris( image, eig, blockSize, gradientSize, harrisK );
    else
        cornerMinEigenVal( image, eig, blockSize, gradientSize );

    double maxVal = 0;

    cvtest::minMaxIdx( eig, 0, &maxVal, 0, 0, mask );
    cvtest::threshold( eig, eig, (float)(maxVal*qualityLevel), 0.f,THRESH_TOZERO );
    cvtest::dilate( eig, tmp, Mat(),Point(-1,-1),borderType,0);

    Size imgsize = image.size();

    vector<const float*> tmpCorners;

    // collect list of pointers to features - put them into temporary image
    for( int y = 1; y < imgsize.height - 1; y++ )
    {
        const float* eig_data = (const float*)eig.ptr(y);
        const float* tmp_data = (const float*)tmp.ptr(y);
        const uchar* mask_data = mask.data ? mask.ptr(y) : 0;

        for( int x = 1; x < imgsize.width - 1; x++ )
        {
            float val = eig_data[x];
            if( val != 0 && val == tmp_data[x] && (!mask_data || mask_data[x]) )
            {
                tmpCorners.push_back(eig_data + x);
            }
        }
    }

    vector<Point2f> corners;
    vector<float> corners_values;
    size_t i, j, total = tmpCorners.size(), ncorners = 0;

    std::sort( tmpCorners.begin(), tmpCorners.end(), greaterThanPtr() );

    if(minDistance >= 1)
    {
         // Partition the image into larger grids
        int w = image.cols;
        int h = image.rows;

        const int cell_size = cvRound(minDistance);
        const int grid_width = (w + cell_size - 1) / cell_size;
        const int grid_height = (h + cell_size - 1) / cell_size;

        std::vector<std::vector<Point2f> > grid(grid_width*grid_height);

        minDistance *= minDistance;

        for( i = 0; i < total; i++ )
        {
            int ofs = (int)((const uchar*)tmpCorners[i] - eig.data);
            int y = (int)(ofs / eig.step);
            int x = (int)((ofs - y*eig.step)/sizeof(float));

            bool good = true;

            int x_cell = x / cell_size;
            int y_cell = y / cell_size;

            int x1 = x_cell - 1;
            int y1 = y_cell - 1;
            int x2 = x_cell + 1;
            int y2 = y_cell + 1;

            // boundary check
            x1 = std::max(0, x1);
            y1 = std::max(0, y1);
            x2 = std::min(grid_width-1, x2);
            y2 = std::min(grid_height-1, y2);

            for( int yy = y1; yy <= y2; yy++ )
            {
                for( int xx = x1; xx <= x2; xx++ )
                {
                    vector <Point2f> &m = grid[yy*grid_width + xx];

                    if( m.size() )
                    {
                        for(j = 0; j < m.size(); j++)
                        {
                            float dx = x - m[j].x;
                            float dy = y - m[j].y;

                            if( dx*dx + dy*dy < minDistance )
                            {
                                good = false;
                                goto break_out;
                            }
                        }
                    }
                }
            }

            break_out:

            if(good)
            {
                grid[y_cell*grid_width + x_cell].push_back(Point2f((float)x, (float)y));

                corners.push_back(Point2f((float)x, (float)y));
                corners_values.push_back(eig.at<float>(y, x));
                ++ncorners;

                if( maxCorners > 0 && (int)ncorners == maxCorners )
                    break;
            }
        }
    }
    else
    {
        for( i = 0; i < total; i++ )
        {
            int ofs = (int)((const uchar*)tmpCorners[i] - eig.data);
            int y = (int)(ofs / eig.step);
            int x = (int)((ofs - y*eig.step)/sizeof(float));

            corners.push_back(Point2f((float)x, (float)y));
            corners_values.push_back(eig.at<float>(y, x));
            ++ncorners;

            if( maxCorners > 0 && (int)ncorners == maxCorners )
                break;
        }
    }

    Mat(corners).convertTo(_corners, _corners.fixedType() ? _corners.type() : CV_32F);
    Mat(corners_values).convertTo(_corners_values, _corners_values.fixedType() ? _corners_values.type() : CV_32F);

}

/////////////////end of ref code//////////////////////////



class CV_GoodFeatureToTTest : public cvtest::ArrayTest
{
public:
    CV_GoodFeatureToTTest();

protected:
    int prepare_test_case( int test_case_idx );
    void run_func();
    int validate_test_results( int test_case_idx );

    Mat src, src_gray;
    Mat src_gray32f, src_gray8U;
    Mat mask;

    int maxCorners;
    vector<Point2f> corners;
    vector<float> corners_values;
    vector<Point2f> Refcorners;
    vector<float> Refcorners_values;
    double qualityLevel;
    double minDistance;
    int blockSize;
    int gradientSize;
    bool useHarrisDetector;
    double k;
    int SrcType;
};


CV_GoodFeatureToTTest::CV_GoodFeatureToTTest()
{
    RNG& rng = ts->get_rng();
    maxCorners = rng.uniform( 50, 100 );
    qualityLevel = 0.01;
    minDistance = 10;
    blockSize = 3;
    gradientSize = 3;
    useHarrisDetector = false;
    k = 0.04;
    mask = Mat();
    test_case_count = 4;
    SrcType = 0;
}

int CV_GoodFeatureToTTest::prepare_test_case( int test_case_idx )
{
    const static int types[] = { CV_32FC1, CV_8UC1 };

    cvtest::TS& tst = *cvtest::TS::ptr();
    src = imread(string(tst.get_data_path()) + "shared/fruits.png", IMREAD_COLOR);

    CV_Assert(src.data != NULL);

    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    SrcType = types[test_case_idx & 0x1];
    useHarrisDetector = test_case_idx & 2 ?  true : false;
    return 1;
}


void CV_GoodFeatureToTTest::run_func()
{
    int cn = src_gray.channels();

    CV_Assert( cn == 1 );
    CV_Assert( ( CV_MAT_DEPTH(SrcType) == CV_32FC1 ) || ( CV_MAT_DEPTH(SrcType) == CV_8UC1 ));

    TEST_MESSAGEL ("             maxCorners = ", maxCorners)
    if (useHarrisDetector)
    {
        TEST_MESSAGE ("             useHarrisDetector = true\n");
    }
    else
    {
        TEST_MESSAGE ("             useHarrisDetector = false\n");
    }

    if( CV_MAT_DEPTH(SrcType) == CV_32FC1)
    {
        if (src_gray.depth() != CV_32FC1 ) src_gray.convertTo(src_gray32f, CV_32FC1);
        else   src_gray32f = src_gray.clone();

        TEST_MESSAGE ("goodFeaturesToTrack 32f\n")

        goodFeaturesToTrack( src_gray32f,
               corners,
               maxCorners,
               qualityLevel,
               minDistance,
               Mat(),
               blockSize,
               gradientSize,
               useHarrisDetector,
               k,
               corners_values);
    }
    else
    {
        if (src_gray.depth() != CV_8UC1 ) src_gray.convertTo(src_gray8U, CV_8UC1);
        else   src_gray8U = src_gray.clone();

        TEST_MESSAGE ("goodFeaturesToTrack 8U\n")

        goodFeaturesToTrack( src_gray8U,
               corners,
               maxCorners,
               qualityLevel,
               minDistance,
               Mat(),
               blockSize,
               gradientSize,
               useHarrisDetector,
               k,
               corners_values);
    }
}


int CV_GoodFeatureToTTest::validate_test_results( int test_case_idx )
{
    static const double eps = 2e-6;

    if( CV_MAT_DEPTH(SrcType) == CV_32FC1 )
    {
        if (src_gray.depth() != CV_32FC1 ) src_gray.convertTo(src_gray32f, CV_32FC1);
        else   src_gray32f = src_gray.clone();

        TEST_MESSAGE ("test_goodFeaturesToTrack 32f\n")

        test_goodFeaturesToTrack( src_gray32f,
               Refcorners,
               maxCorners,
               qualityLevel,
               minDistance,
               Mat(),
               blockSize,
               gradientSize,
               useHarrisDetector,
               k,
               Refcorners_values);
    }
    else
    {
        if (src_gray.depth() != CV_8UC1 ) src_gray.convertTo(src_gray8U, CV_8UC1);
        else   src_gray8U = src_gray.clone();

        TEST_MESSAGE ("test_goodFeaturesToTrack 8U\n")

        test_goodFeaturesToTrack( src_gray8U,
               Refcorners,
               maxCorners,
               qualityLevel,
               minDistance,
               Mat(),
               blockSize,
               gradientSize,
               useHarrisDetector,
               k,
               Refcorners_values);
    }

    double e = cv::norm(corners, Refcorners); // TODO cvtest

    if (e > eps)
    {
        TEST_MESSAGEL ("Number of features: Refcorners =  ", Refcorners.size())
        TEST_MESSAGEL ("                    TestCorners = ", corners.size())
        TEST_MESSAGE ("\n")

        ts->printf(cvtest::TS::CONSOLE, "actual error: %g, expected: %g", e, eps);
        ts->set_failed_test_info(cvtest::TS::FAIL_BAD_ACCURACY);

        for(int i = 0; i < (int)std::min((unsigned int)(corners.size()), (unsigned int)(Refcorners.size())); i++){
            if ( (corners[i].x != Refcorners[i].x) || (corners[i].y != Refcorners[i].y) || (corners_values[i] != Refcorners_values[i]))
                printf("i = %i X %2.6f Xref %2.6f Y %2.6f Yref %2.6f Values %2.6f Values ref %2.6f\n",i,corners[i].x,Refcorners[i].x,corners[i].y,Refcorners[i].y,corners_values[i],Refcorners_values[i]);
        }
    }
    else
    {
        TEST_MESSAGEL (" Refcorners =  ", Refcorners.size())
        TEST_MESSAGEL (" TestCorners = ", corners.size())
        TEST_MESSAGE ("\n")

        ts->set_failed_test_info(cvtest::TS::OK);
    }

    return BaseTest::validate_test_results(test_case_idx);

}

TEST(Imgproc_GoodFeatureToT, accuracy) { CV_GoodFeatureToTTest test; test.safe_run(); }


}} // namespace
/* End of file. */
