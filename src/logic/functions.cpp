#include "functions.h"
#include "image.h"
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <chrono>

std::vector<function> func_list;

void buildFuncs(){
    func_list.reserve(20);

    func_list.emplace_back(function{ NULL, 0, 1, 0 });
    func_list.emplace_back(function{ NULL, 1, 0, 0 });

    // 0 - 4
    func_list.emplace_back(function{ arithmetic, 1, 1, 1 });
    func_list.emplace_back(function{ add2Images, 2, 1, 1 });
    func_list.emplace_back(function{ RGBSplit, 1, 3, 0 });
    func_list.emplace_back(function{ quantizeRGB, 1, 1, 1 });
    func_list.emplace_back(function{ grayscale, 1, 1, 1 });

	// 5 - 9
}

void killFuncs(){
    //cvector_free(func_list);
}

void arithmetic(
    struct Image** input_images,
    struct Image** output_images,
    void** params
) {
    struct Image* input = input_images[0];
    struct Image* output = output_images[0];

    deepCopyImage(output, input);
    uint8_t* buf_in = input->buffer;
    uint8_t* buf_out = output->buffer;

    int rows = input->rows;
    int cols = input->cols;
    int c = 4;

    uint8_t mode = (params[0] == NULL) ? 0 : *(uint8_t*)params[0];

    uchar* ptr_in;
    uchar* ptr_out;

    uint8_t val = (params[1] == NULL) ? 100 : *(uint8_t*)params[1];
    float factor = (params[1] == NULL) ? 2 : *(float*)params[1];
    int total;

    int x, y, z, idx;

    //mode: 0/1/2 Addition/Subtraction/Factor
    switch (mode) {
    case 0:
        for (x = 0; x < rows; x++) {
            for (y = 0; y < cols; y++) {
                idx = (x * cols + y) * c;
                for (z = 0; z < 3; z++) {
                    total = buf_in[idx + z] + val;
                    buf_out[idx + z] = (total > 255) ? 255 : total;
                }
            }
        }
        break;

    case 1:
        for (x = 0; x < rows; x++) {
            for (y = 0; y < cols; y++) {
                idx = (x * cols + y) * c;
                for (z = 0; z < 3; z++) {
                    total = buf_in[idx + z] - val;
                    buf_out[idx + z] = (total < 0) ? 0 : total;
                }
            }
        }
        break;

    case 2:
        for (x = 0; x < rows; x++) {
            for (y = 0; y < cols; y++) {
                idx = (x * cols + y) * c;
                for (z = 0; z < 3; z++) {
                    total = buf_in[idx + z] * factor;
                    if (total > 255) {
                        total = 255;
                    }
                    else if (total < 0) {
                        total = 0;
                    }
                    buf_out[idx + z] = total;
                }
            }
        }
        break;
    };
}

void add2Images(
    struct Image** input_images,
    struct Image** output_images,
    void** params
){
    struct Image* input1 = input_images[0];
    struct Image* input2 = input_images[1];
    struct Image* output = output_images[0];

    deepCopyImage(output, input1);
    uint8_t* buf_in1 = input1->buffer;
    uint8_t* buf_in2 = input2->buffer;
    uint8_t* buf_out = output->buffer;
    
    int rows = input1->rows;
    int cols = input1->cols;
    int c = 4;

    float alpha = (params[0] == NULL) ? 0.5 : *(float*)params[0];
    int x, y, z, idx, total;
    for (x = 0; x < rows; x++)
    {
        for (y = 0; y < cols; y++){
            idx = (x * cols + y)*c;
            for ( z = 0; z < 4; z++){
                total = (uint8_t)(buf_in1[idx+z] * alpha) + 
                        (buf_in2[idx+z] * (1-alpha));

                buf_out[idx+z] = (total > 255) ? 255 : total;
            }
        }
    }
    //printImage(output);
}


void RGBSplit(
    struct Image** input_images,
    struct Image** output_images,
    void** params
) {
    struct Image* input = input_images[0];
    struct Image* outputR = output_images[0];
    struct Image* outputG = output_images[1];
    struct Image* outputB = output_images[2];

    deepCopyImage(outputR, input);
    deepCopyImage(outputG, input);
    deepCopyImage(outputB, input);
    uint8_t* buf_in = input->buffer;
    uint8_t* buf_out_r = outputR->buffer;
    uint8_t* buf_out_g = outputG->buffer;
    uint8_t* buf_out_b = outputB->buffer;

    int rows = input->rows;
    int cols = input->cols;
    int c = 4;

    int x, y, z, idx, total;
    for (x = 0; x < rows; x++)
    {
        for (y = 0; y < cols; y++) {
            idx = (x * cols + y) * c;
            for (z = 0; z < 4; z++) {
                switch (z) {
                    case 0:
                        buf_out_r[idx + z] = buf_in[idx + z];
                        buf_out_g[idx + z] = 0;
                        buf_out_b[idx + z] = 0;
                        break;
                    case 1:
                        buf_out_r[idx + z] = 0;
                        buf_out_g[idx + z] = buf_in[idx + z];
                        buf_out_b[idx + z] = 0;
                        break;
                    case 2:
                        buf_out_r[idx + z] = 0;
                        buf_out_g[idx + z] = 0;
                        buf_out_b[idx + z] = buf_in[idx + z];
                        break;
                    case 3:
                        buf_out_r[idx + z] = buf_in[idx + z];
                        buf_out_g[idx + z] = buf_in[idx + z];
                        buf_out_b[idx + z] = buf_in[idx + z];
                        break;
				}
            }
        }
    }
    //printImage(output);
}


void quantizeRGB(
    struct Image** input_images,
    struct Image** output_images,
    void** params
) {
    struct Image* input = input_images[0];
    struct Image* output = output_images[0];

    deepCopyImage(output, input);
    uint8_t* buf_in = input->buffer;
    uint8_t* buf_out = output->buffer;

    int rows = input->rows;
    int cols = input->cols;
    int c = 4;

    uint8_t val = (params[0] == NULL) ? 100 : *(uint8_t*)params[0];
    int table[256];
    
    for (int i = 0; i < 256; i++) {
        table[i] = (uint8_t)(val * (i / val));
    }

    uchar* ptr_in;
    uchar* ptr_out;

    int x, y, z, idx;
    //printf("adding with %d\n", val);
    for (x = 0; x < rows; x++)
    {
        for (y = 0; y < cols; y++) {
            idx = (x * cols + y) * c;
            for (z = 0; z < 3; z++) {
                buf_out[idx + z] = table[buf_in[idx + z]];
            }
        }
    }
}

void grayscale(
    struct Image** input_images,
    struct Image** output_images,
    void** params
) {
    struct Image* input = input_images[0];
    struct Image* output = output_images[0];

    deepCopyImage(output, input);
    uint8_t* buf_in = input->buffer;
    uint8_t* buf_out = output->buffer;

    int rows = input->rows;
    int cols = input->cols;
    int c = 4;

    uint8_t mode = (params[0] == NULL) ? 0 : *(uint8_t*)params[0];

    uchar* ptr_in;
    uchar* ptr_out;

    int x, y, z, idx;

    //mode: 0 (NTSC), 1/2/3 R/G/B
    switch (mode) {
    case 0:
        for (x = 0; x < rows; x++) {
            for (y = 0; y < cols; y++) {
                idx = (x * cols + y) * c;
                uint8_t gray = (uint8_t)(0.299 * buf_in[idx] + 0.587 * buf_in[idx + 1] + 0.114 * buf_in[idx + 2]);
                buf_out[idx] = gray;
                buf_out[idx + 1] = gray;
                buf_out[idx + 2] = gray;
                buf_out[idx + 3] = buf_in[idx + 3];
            }
        }
        break;

    case 1:
        for (x = 0; x < rows; x++) {
            for (y = 0; y < cols; y++) {
                idx = (x * cols + y) * c;
                buf_out[idx + 1] = buf_in[idx];
                buf_out[idx + 2] = buf_in[idx];
            }
        }
        break;

    case 2:
        for (x = 0; x < rows; x++) {
            for (y = 0; y < cols; y++) {
                idx = (x * cols + y) * c;
                buf_out[idx] = buf_in[idx + 1];
                buf_out[idx + 2] = buf_in[idx + 1];
            }
        }
        break;

    case 3:
        for (x = 0; x < rows; x++) {
            for (y = 0; y < cols; y++) {
                idx = (x * cols + y) * c;
                buf_out[idx + 0] = buf_in[idx + 2];
                buf_out[idx + 1] = buf_in[idx + 2];
            }
        }
        break;
    };
}