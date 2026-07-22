#include "functions.h"
#include "image.h"
#include <stdlib.h>
#include <stdio.h>

cvector_vector_type(struct function) func_list = NULL;

void buildFuncs(){
    cvector_reserve(func_list, 20);

    cvector_push_back(func_list, ((struct function){NULL, 0, 1, 0}));      // input
    //cvector_push_back(func_list, ((struct function){display, 1, 0, 1}));     // output for headless testing
    cvector_push_back(func_list, ((struct function){NULL, 1, 0, 0}));     // output

    //"0 index" from here when refering to node types
    cvector_push_back(func_list, ((struct function){add, 1, 1, 1}));
    cvector_push_back(func_list, ((struct function){subtract, 1, 1, 1})); 
    cvector_push_back(func_list, ((struct function){add2Images, 2, 1, 1})); 
}

void killFuncs(){
    cvector_free(func_list);
}

// Input image, filename
void display(
    struct Image** input_images,
    struct Image** output_images,
    void** params
){
    printf("saving\n");
    struct Image* input = input_images[0];
    printf("trying to save:%p\n", input);
    //printf("3buffer[0]: %d\n",  input->buffer[0]);
    char* name = (char*)params[0];
    printImage(input);
    savePng(input, name);
}

// Input image, output image, addition value
void add(
    struct Image** input_images,
    struct Image** output_images,
    void** params
){
    struct Image* input = input_images[0];
    struct Image* output = output_images[0];

    deepCopyImage(output, input);
    uint8_t* buf_in = input->buffer;
    uint8_t* buf_out = output->buffer;
    
    int rows = input->rows;
    int cols = input->cols;
    int c = input->channels;

    uint8_t val = *(uint8_t*)params[0];
    int x, y, z, idx, total;
    printf("adding with %d\n", val);
    for (x = 0; x < rows; x++)
    {
        for (y = 0; y < cols; y++){
            idx = (x * cols + y)*c;
            for (z = 0; z < 4; z++){
                if(z == 3){
                    buf_out[idx+z] = buf_in[idx+z];
                    continue;
                }
                total = buf_in[idx+z] + val;
                buf_out[idx+z] = (total > 255) ? 255 : total;
            }
        }
    }
    printImage(output);
}

void subtract(
    struct Image** input_images,
    struct Image** output_images,
    void** params
){
    struct Image* input = input_images[0];
    struct Image* output = output_images[0];

    deepCopyImage(output, input);
    uint8_t* buf_in = input->buffer;
    uint8_t* buf_out = output->buffer;
    
    int rows = input->rows;
    int cols = input->cols;
    int c = input->channels;

    uint8_t val = *(uint8_t*)params[0];
    int x, y, z, idx, total;
    printf("subbing with %d\n", val);
    for (x = 0; x < rows; x++)
    {
        for (y = 0; y < cols; y++){
            idx = (x * cols + y)*c;
            for (z = 0; z < 4; z++){
                if(z == 3){
                    buf_out[idx+z] = buf_in[idx+z];
                    continue;
                }
                total = buf_in[idx+z] - val;
                buf_out[idx+z] = (total < 0) ? 0 : total;
            }
        }
    }
    printImage(output);
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
    int c = input1->channels;

    float alpha = *(float*)params[0];
    int x, y, z, idx, total;
    printf("blending with %f\n", alpha);
    for (x = 0; x < rows; x++)
    {
        for (y = 0; y < cols; y++){
            idx = (x * cols + y)*c;
            for ( z = 0; z < 4; z++){
                if(z == 3){
                    buf_out[idx+z] = buf_in1[idx+z];
                    continue;
                }
                total = (uint8_t)(buf_in1[idx+z] * alpha) + 
                        (buf_in2[idx+z] * (1-alpha));

                buf_out[idx+z] = (total > 255) ? 255 : total;
            }
        }
    }
    printImage(output);
}
