#include "functions.h"
#include "image.h"
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <chrono>

std::vector<function> func_list;

/*
    auto start = std::chrono::high_resolution_clock::now();
    /////
    auto end = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "OPENCV: " << time << " ms\n";



    start = std::chrono::high_resolution_clock::now();
    end = std::chrono::high_resolution_clock::now();
    ///////
    time = std::chrono::duration<double, std::milli>(end - start).count();
*/

void buildFuncs(){
    func_list.reserve(20);

    func_list.emplace_back(function{ NULL, 0, 1, 0 });

    // 1 - 5
    func_list.emplace_back(function{ arithmetic, 1, 1, 1 });
    func_list.emplace_back(function{ add2Images, 2, 1, 1 });
    func_list.emplace_back(function{ RGBSplit, 1, 3, 0 });
    func_list.emplace_back(function{ quantizeRGB, 1, 1, 1 });
    func_list.emplace_back(function{ grayscale, 1, 1, 1 });

	// 6 - 10
    func_list.emplace_back(function{ HSVSplit, 1, 3, 0 });
    func_list.emplace_back(function{ BinaryThres, 1, 1, 1 });
    func_list.emplace_back(function{ RGBMask, 2, 1, 0 });
    func_list.emplace_back(function{ HistEq, 1, 1, 0 });
}

void killFuncs(){
    //cvector_free(func_list);
}

void arithmetic(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
) {
    cv::Mat* input = input_images[0];
    cv::Mat* output = output_images[0];

    input->copyTo(*output);

    int rows = input->rows;
    int cols = input->cols;
    int c = 4;

    uchar mode = (params[0] == NULL) ? 0 : *(uchar*)params[0];


    uchar val = (params[1] == NULL) ? 100 : *(uchar*)params[1];
    float factor = (params[1] == NULL) ? 2 : *(float*)params[1];
    int total;

    uchar* buf_in;
    uchar* buf_out;

    int x, y, z, i;
    int idx = 0;
    int pixels = rows * cols;

    //mode: 0/1/2 Addition/Subtraction/Factor
    switch (mode) {
    case 0:
        *output = *input + cv::Scalar(val, val, val);
        break;

    case 1:
        buf_in = input->data;
        buf_out = output->data;
        for (i = 0; i < pixels; i++) {
            total = buf_in[idx] - val;
            buf_out[idx] = (total < 0) ? 0 : total;

            total = buf_in[idx + 1] - val;
            buf_out[idx + 1] = (total < 0) ? 0 : total;

            total = buf_in[idx + 2] - val;
            buf_out[idx + 2] = (total < 0) ? 0 : total;
            idx += 4;
        }
        break;

    case 2:
        buf_in = input->data;
        buf_out = output->data;
        for (int i = 0; i < pixels; i++) {

            int total;

            total = buf_in[idx] * factor;
            buf_out[idx] = (total > 255) ? 255 : total;

            total = buf_in[idx + 1] * factor;
            buf_out[idx + 1] = (total > 255) ? 255 : total;

            total = buf_in[idx + 2] * factor;
            buf_out[idx + 2] = (total > 255) ? 255 : total;
            idx += 4;
        }

        break;
    };
}

void add2Images(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
){
    cv::Mat* input1 = input_images[0];
    cv::Mat* input2 = input_images[1];
    cv::Mat* output = output_images[0];

    if (input1->size() != input2->size()) {
        std::cerr << "Error: Input images must have the same size." << std::endl;
        return;
	}

    float alpha = (params[0] == NULL) ? 0.5 : *(float*)params[0];
    if (alpha == 1) {
        *output = input1->clone();
        return;
    } else if (alpha == 0){
        *output = input2->clone();
        return;
    }

    cv::addWeighted(*input1, alpha, *input2, 1 - alpha, 0, *output);

    //printImage(output);
}


void RGBSplit(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat* input = input_images[0];

    cv::Mat chans[4];
    cv::split(*input, chans);

    std::vector<cv::Mat> r_channels = {
        chans[0],
        cv::Mat::zeros(chans[0].size(), CV_8UC1),
        cv::Mat::zeros(chans[0].size(), CV_8UC1),
        chans[3]
    };

    std::vector<cv::Mat> g_channels = {
        cv::Mat::zeros(chans[0].size(), CV_8UC1),
        chans[1],
        cv::Mat::zeros(chans[0].size(), CV_8UC1),
        chans[3]
    };

    std::vector<cv::Mat> b_channels = {
        cv::Mat::zeros(chans[0].size(), CV_8UC1),
        cv::Mat::zeros(chans[0].size(), CV_8UC1),
        chans[2],
        chans[3]
    };

    cv::merge(b_channels, *output_images[2]);
    cv::merge(g_channels, *output_images[1]);
    cv::merge(r_channels, *output_images[0]);

    auto end = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Custom: " << time << " ms\n";
    //printImage(output);
}


void quantizeRGB(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
) {
    cv::Mat* input = input_images[0];
    cv::Mat* output = output_images[0];

    input->copyTo(*output);

    int rows = input->rows;
    int cols = input->cols;
    int c = 4;

    uchar val = (params[0] == NULL) ? 100 : *(uchar*)params[0];
    uchar table[256];
    
    for (int i = 0; i < 256; i++) {
        table[i] = static_cast<uchar>(val * (i / val));
    }

    int idx = 0;
    int pixels = rows * cols;

    uchar* buf_in = input->data;
    uchar* buf_out = output->data;
    for (int i = 0; i < pixels; i++) {
        buf_out[idx] = table[buf_in[idx]];
        buf_out[idx + 1] = table[buf_in[idx + 1]];
        buf_out[idx + 2] = table[buf_in[idx + 2]];
        idx += 4;
    }
}

void grayscale(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
) {
    cv::Mat* input = input_images[0];
    cv::Mat* output = output_images[0];

    input->copyTo(*output);
    uchar* buf_in;
    uchar* buf_out;

    int rows = input->rows;
    int cols = input->cols;
    int c = 4;

    uchar mode = (params[0] == NULL) ? 0 : *(uchar*)params[0];

    int x, y, z, idx;

    //mode: 0 (NTSC), 1/2/3 R/G/B
    switch (mode) {
    case 0:
        for (x = 0; x < rows; x++) {
            buf_in = input->ptr<uchar>(x);
            buf_out = output->ptr<uchar>(x);
            for (y = 0; y < cols; y++) {
                idx = y * c;
                uchar gray = (uchar)(0.299 * buf_in[idx] + 0.587 * buf_in[idx + 1] + 0.114 * buf_in[idx + 2]);
                buf_out[idx] = gray;
                buf_out[idx + 1] = gray;
                buf_out[idx + 2] = gray;
                buf_out[idx + 3] = buf_in[idx + 3];
            }
        }
        break;

    case 1:
        for (x = 0; x < rows; x++) {
            buf_in = input->ptr<uchar>(x);
            buf_out = output->ptr<uchar>(x);
            for (y = 0; y < cols; y++) {
                idx = y * c;
                buf_out[idx + 1] = buf_in[idx];
                buf_out[idx + 2] = buf_in[idx];
            }
        }
        break;

    case 2:
        for (x = 0; x < rows; x++) {
            buf_in = input->ptr<uchar>(x);
            buf_out = output->ptr<uchar>(x);
            for (y = 0; y < cols; y++) {
                idx = y * c;
                buf_out[idx] = buf_in[idx + 1];
                buf_out[idx + 2] = buf_in[idx + 1];
            }
        }
        break;

    case 3:
        for (x = 0; x < rows; x++) {
            buf_in = input->ptr<uchar>(x);
            buf_out = output->ptr<uchar>(x);
            for (y = 0; y < cols; y++) {
                idx = y * c;
                buf_out[idx + 0] = buf_in[idx + 2];
                buf_out[idx + 1] = buf_in[idx + 2];
            }
        }
        break;
    };
}


void HSVSplit(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat* input = input_images[0];

    cv::Mat rgb;
    cv::cvtColor(*input, rgb, cv::COLOR_RGBA2RGB);
    cv::Mat hsv;
    cv::cvtColor(rgb, hsv, cv::COLOR_RGB2HSV);

    // Split the original RGBA to get alpha
    cv::Mat rgba[4];
    cv::split(*input, rgba);

    // Split HSV
    cv::Mat hsv_chans[3];
    cv::split(hsv, hsv_chans);

    // H output
    {
        cv::Mat out[4] = {
            hsv_chans[0],
            hsv_chans[0],
            hsv_chans[0],
            rgba[3]
        };
        cv::merge(out, 4, *output_images[0]);
    }

    // S output
    {
        cv::Mat out[4] = {
            hsv_chans[1],
            hsv_chans[1],
            hsv_chans[1],
            rgba[3]
        };
        cv::merge(out, 4, *output_images[1]);
    }

    // V output
    {
        cv::Mat out[4] = {
            hsv_chans[2],
            hsv_chans[2],
            hsv_chans[2],
            rgba[3]
        };
        cv::merge(out, 4, *output_images[2]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Custom: " << time << " ms\n";
    //printImage(output);
}

void BinaryThres(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
) {
    cv::Mat* input = input_images[0];
    cv::Mat* output = output_images[0];
    input->copyTo(*output);

    uchar val = (params[0] == NULL) ? 100 : *(uchar*)params[0];


    int rows = input->rows;
    int cols = input->cols;
    
	int pixels = rows * cols;
	int idx = 0;

    uchar* buf_in = input->data;
    uchar* buf_out = output->data;
    for (int i = 0; i < pixels; i++) {
        if (buf_in[idx] >= val) {
            buf_out[idx] = buf_out[idx + 1] = buf_out[idx + 2] = 255;
        } else {
            buf_out[idx] = buf_out[idx + 1] = buf_out[idx + 2] = 0;
        }
        idx += 4;
    }
}

//TODO: give options for binary, inv binary, or smooth
void RGBMask(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
) {
    cv::Mat* color = input_images[0];
    cv::Mat* mask = input_images[1];
    cv::Mat* output = output_images[0];
    color->copyTo(*output);

    int rows = color->rows;
    int cols = color->cols;

    int pixels = rows * cols;
    int idx = 0;

    uchar* buf_in = mask->data;
    uchar* buf_out = output->data;
    for (int i = 0; i < pixels; i++) {
        if (buf_in[idx] == 0) {
            buf_out[idx] = buf_out[idx + 1] = buf_out[idx + 2] = 0;
        }
        idx += 4;
    }
}


void HistEq(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
) {
    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat* input = input_images[0];

    cv::Mat rgb;
    cv::cvtColor(*input, rgb, cv::COLOR_RGBA2RGB);
    cv::Mat hsv;
    cv::cvtColor(rgb, hsv, cv::COLOR_RGB2HSV);

    cv::Mat rgba[4];
    cv::split(*input, rgba);

    cv::Mat hsv_chans[3];
    cv::split(hsv, hsv_chans);

	cv::equalizeHist(hsv_chans[2], hsv_chans[2]);

    cv::merge(hsv_chans, 3, hsv);
    cv::cvtColor(hsv, rgb, cv::COLOR_HSV2RGB);

    cv::Mat rgb_chans[3];
    cv::split(rgb, rgb_chans);

    cv::Mat out[4] = {
        rgb_chans[0],
        rgb_chans[1],
        rgb_chans[2],
        rgba[3]
    };

    cv::merge(out, 4, *output_images[0]);

    auto end = std::chrono::high_resolution_clock::now();
    double time = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Custom: " << time << " ms\n";
    //printImage(output);
}