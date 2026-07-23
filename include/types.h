#pragma once
#include <stdint.h>
#include <vector>

#define MAXNODES	20
#define MAX_OUT		10

#define INPUT		-2
#define OUTPUT		-1

#define ACTIVE		0
#define INACTIVE	1
#define EMPTY		2

#define CHUNK_SIZE 1024
#define DECOM   0
#define COM     1


struct Image{
    uint8_t* buffer;
    uint32_t rows;
    uint32_t cols;
    uint64_t size;
};

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


typedef void (*operation)(
    struct Image** input_images,
    struct Image** output_images,
    void** params
);
