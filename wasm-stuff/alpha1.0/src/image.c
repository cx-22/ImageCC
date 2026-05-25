#include "image.h"
#include "lz4.h"
#include "tpng.h"
#include "svpng.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHUNK_SIZE 1024
#define DECOM   0
#define COM     1

struct Image* loadPng(char* name){
    struct Image* input = calloc(1, sizeof(struct Image));
    uint32_t  pngsize;
    uint8_t * pngdata = dumpFileData(name, &input->size);

    uint8_t* rgbaData = tpng_get_rgba(
        pngdata,
        input->size,
        &input->cols,
        &input->rows
    );

    if (rgbaData == NULL) {
        free(input);
        return NULL;
    }

    free(pngdata);

    input->buffer = rgbaData;
    input->channels = 4;
    input->size = input->channels * input->rows * input->cols;
    input->status = DECOM;
    return input;
}

struct Image* makeImage(uint8_t* data){
    struct Image* input = calloc(1, sizeof(struct Image));
    input->buffer = data;
    input->channels = 4;
    input->size = input->channels * input->rows * input->cols;
    input->status = DECOM;
    return input;
}

void shallowCopyImage(struct Image* to, const struct Image* from){
    free(to->buffer);
    to->status = from->status;
    to->channels = from->channels;
    to->buffer = from->buffer;
    to->rows = from->rows;
    to->cols = from->cols;
    to->com_size = from->com_size;
    to->size = from->size;
}

void deepCopyImage(struct Image* to, const struct Image* from){
    free(to->buffer);
    to->buffer = malloc(from->size);
    memcpy(to->buffer, from->buffer, from->size);
    to->status = from->status;
    to->channels = from->channels;
    to->rows = from->rows;
    to->cols = from->cols;
    to->com_size = from->com_size;
    to->size = from->size;
}

void clearImage(struct Image* im){
    if (im->buffer){
        free(im->buffer);
    }
    
    im->buffer = NULL;
    printf("cleared image\n");
}

void destroyImage(struct Image* im){
    free(im->buffer);
    im->buffer = NULL;
    free(im);
    im = NULL;
    printf("killed image\n");
}

uint8_t * dumpFileData(const char * filename, uint64_t * size) {
    FILE * f = fopen(filename, "rb");
    if (!f) {
        printf("Cannot open file!");
        exit(2);
    }

    char chunk[CHUNK_SIZE];
    *size = 0;
    uint32_t readAmt = 0;
    while((readAmt = fread(chunk, 1, CHUNK_SIZE, f))) 
        *size += readAmt;

    fseek(f, SEEK_SET, 0);
    uint8_t * out = malloc(*size);
    if (fread(out, 1, *size, f) != *size) {
        printf("Cannot read file!");
        exit(1);
    }
    fclose(f);
    return out;
}

void savePng(struct Image* input, char* name){
    FILE* fp = fopen(name, "wb");
    svpng(fp, input->cols, input->rows, (unsigned char*)input->buffer, 1);
    fclose(fp);
}



int compress(struct Image* img){
    int maxCompressedSize =
        LZ4_compressBound(img->size);

    uint8_t* compressed =
        (uint8_t*)malloc(sizeof(uint8_t) * maxCompressedSize);

    img->com_size =
        LZ4_compress_default(
            (const char*)img->buffer,
            (char*)compressed,
            img->size,
            maxCompressedSize
        );
    

    if (img->com_size <= 0)
    {
        free(compressed);
        printf("Compression failed\n");
        return 1;
    }
    free(img->buffer);
    img->buffer = compressed;
    img->status = COM;
    return 0;
}

int decompress(struct Image* img){
    uint8_t* decompressed =
        (uint8_t*)malloc(img->size);

    int decompressedSize =
        LZ4_decompress_safe(
            (const char*)img->buffer,
            (char*)decompressed,
            img->com_size,
            img->size
        );
    free(img->buffer);

    //printf("com size: %ld dcom size: %ld ratio: %f\n", img->size, img->com_size, ((float)img->com_size/(float)img->size));

    if (decompressedSize < 0)
    {
        free(decompressed);
        printf("Decompression failed\n");
        return 1;
    }

    img->buffer = decompressed;
    img->status = DECOM;
    return 0;
}

void printImage(const struct Image* img)
{
    if (!img) {
        printf("Image: (null)\n");
        return;
    }

    printf("Image {\n");
    printf("  status   : %d\n", img->status);
    printf("  channels : %d\n", img->channels);
    printf("  buffer   : %p\n", (void*)img->buffer);
    printf("  rows     : %d\n", img->rows);
    printf("  cols     : %d\n", img->cols);
    printf("  com_size : %llu\n", img->com_size);
    printf("  size     : %llu\n", img->size);

    if (img->buffer && img->size > 0) {
        printf("  buffer bytes (first up to 32): ");
        uint64_t n = img->size < 32 ? img->size : 32;

        for (uint32_t i = 0; i < n; i++) {
            printf("%u ", img->buffer[i]);
        }
        printf("\n");
    }

    printf("}\n");
}