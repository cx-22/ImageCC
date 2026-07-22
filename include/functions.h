#pragma once
#include "types-c.h"


#ifdef __cplusplus
extern "C" {
#endif

extern cvector_vector_type(struct function) func_list;
void buildFuncs();
void killFuncs();

#ifdef __cplusplus
}
#endif


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
