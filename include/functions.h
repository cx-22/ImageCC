#pragma once
#include "types.h"

extern std::vector<struct function> func_list;

void buildFuncs();
void killFuncs();



void arithmetic(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
);

void add2Images(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
);

void RGBSplit(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
);

void quantizeRGB(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
);

void grayscale(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
);

void HSVSplit(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
);

void BinaryThres(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
);

void RGBMask(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
<<<<<<< HEAD
);

void HistEq(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
);

void HSVMerge(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
);

void RGBMerge(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
);

void ChangeHL(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
=======
>>>>>>> parent of d7f2c15 (add hist eq)
);