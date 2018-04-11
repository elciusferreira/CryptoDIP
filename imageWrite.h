#ifndef CRYPTODIP_SIMPLE_BUS_IMAGEWRITE_H
#define CRYPTODIP_SIMPLE_BUS_IMAGEWRITE_H

#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>
//#include "opencv2"


void inline imageWrite(int* pixelArray, int height, int width, const char* file)
{
//    uchar data[12] = {0,0,255,0,0,255,0,0,255,0,0,255};
//    Mat src =  Mat(2,2, CV_8UC3, data);
    int size = height * width;

    char *data = new char[size*3];

    for (int i = 0; i < size; ++i){
        data[i*3] = char((pixelArray[i] >> 8) & 0xff);  // Blue component
        data[i*3 +1] = char((pixelArray[i] >> 16) & 0xff); // Green component
        data[i*3 +2] = char((pixelArray[i] >> 24) & 0xff); // Red component
    }

    cv::Mat src = cv::Mat(width, height, CV_8UC3, data);
    cv::imwrite(file, src);
}

#endif //CRYPTODIP_SIMPLE_BUS_IMAGEWRITE_H
