#pragma once
#include <stdint.h>

struct Image{
    uint8_t status;
    int channels;
    uint8_t* buffer;
    uint32_t rows;
    uint32_t cols;
    uint64_t com_size;
    uint64_t size;
};

struct Image* loadPng(char* name);
struct Image* makeImage(uint8_t* data);
void shallowCopyImage(struct Image* to, const struct Image* from);
void deepCopyImage(struct Image* to, const struct Image* from);
void clearImage(struct Image* im);
void destroyImage(struct Image* im);
uint8_t * dumpFileData(const char * filename, uint64_t * size);
void savePng(struct Image* input, char* name);

int decompress(struct Image* im);
int compress(struct Image* im);

void printImage(const struct Image* im);