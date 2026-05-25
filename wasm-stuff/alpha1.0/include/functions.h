#pragma once
#include "cvector.h"
#include "image.h"

typedef void (*operation)(
    struct Image** input_images,
    struct Image** output_images,
    void** params
);

struct function{
    operation op;
	uint8_t n_inputs; //images
	uint8_t n_outputs; //images
    uint8_t n_params;
};

extern cvector_vector_type(struct function) func_list;

void buildFuncs();
void killFuncs();

void display(
    struct Image** input_images,
    struct Image** output_images,
    void** params
);

void add(
    struct Image** input_images,
    struct Image** output_images,
    void** params
);

void subtract(
    struct Image** input_images,
    struct Image** output_images,
    void** params
);

void add2Images(
    struct Image** input_images,
    struct Image** output_images,
    void** params
);