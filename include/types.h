#pragma once
#include <opencv2/opencv.hpp>
#include <stdint.h>
#include <vector>

#define MAXNODES	20
#define MAX_OUT		10

#define INPUT	0

#define ACTIVE		0
#define INACTIVE	1
#define EMPTY		2

#define CHUNK_SIZE 1024
#define DECOM   0
#define COM     1

typedef void (*operation)(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
    );

struct function{
    operation op;
    uchar n_inputs; //images
    uchar n_outputs; //images
    uchar n_params;
};


typedef void (*operation)(
    cv::Mat** input_images,
    cv::Mat** output_images,
    void** params
);
