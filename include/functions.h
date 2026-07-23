#pragma once
#include "types.h"

extern std::vector<struct function> func_list;

void buildFuncs();
void killFuncs();

void arithmetic(
    struct Image** input_images,
    struct Image** output_images,
    void** params
);

void add2Images(
    struct Image** input_images,
    struct Image** output_images,
    void** params
);

void RGBSplit(
    struct Image** input_images,
    struct Image** output_images,
    void** params
);

void quantizeRGB(
    struct Image** input_images,
    struct Image** output_images,
    void** params
);

void grayscale(
    struct Image** input_images,
    struct Image** output_images,
    void** params
);


