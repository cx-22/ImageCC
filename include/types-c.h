#pragma once
#include <stdint.h>
#include "cvector.h"

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

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*NodeCompleteCallback)(struct Node* node);

// A node will own its own output images, but reference its input
struct Node{
    struct Node** input_nodes;
    struct Node** output_nodes;
    struct Image** input_images;
    struct Image** output_images;
    void** params;
    struct function* func;
    //void** args; // order: input images list, output images list, parameters
    uint8_t status;
    NodeCompleteCallback on_complete;
    void* userdata;
};

struct NodeEntry{
    uint16_t nid;
    int type;
    struct Node* node;
};

#ifdef __cplusplus
}
#endif

struct Image{
    uint8_t status;
    int channels;
    uint8_t* buffer;
    uint32_t rows;
    uint32_t cols;
    uint64_t com_size;
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
    cvector_vector_type(struct NodeEntry*) node_list;
};


typedef void (*operation)(
    struct Image** input_images,
    struct Image** output_images,
    void** params
);
