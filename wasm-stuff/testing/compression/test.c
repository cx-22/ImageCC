#include "tpng.h"
#include <stdio.h>
#include "helper.h"
#include <stdlib.h>
#include "svpng.h"
#include <string.h>
#include <time.h> 
#include "lz4.h"

struct Image{
    uint8_t* buffer;
    uint32_t rows;
    uint32_t cols;
    int channels;
    uint32_t com_size;
    uint32_t size;
};

void addImage(struct Image* input, uint8_t val){
    int rows = input->rows;
    int cols = input->cols;
    int c = input->channels;
    uint8_t* buf = input->buffer;
    for (int x = 0; x < rows; x++)
    {
        for (int y = 0; y < cols; y++){
            int idx = (x * cols + y)*c;
            for (int z = 0; z < 3; z++){
                int sum = buf[idx+z] + val;
                buf[idx+z] = (sum > 255) ? 255 : sum;
            }
        }
    }
}


void save_png(struct Image* input, char* name){
    FILE* fp = fopen(name, "wb");
    svpng(fp, input->cols, input->rows, (unsigned char*)input->buffer, 1);
    fclose(fp);
}


struct Image* load_png(char* name){
    struct Image* input = malloc(sizeof(struct Image));
    uint32_t  pngsize;
    uint8_t * pngdata = dump_file_data(name, &input->size);


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
    return input;
}

void decon_image(struct Image* im){
    free(im->buffer);
    free(im);
}

int compress(struct Image* im){
    int maxCompressedSize =
        LZ4_compressBound(im->size);

    uint8_t* compressed =
        (uint8_t*)malloc(sizeof(uint8_t) * maxCompressedSize);

    im->com_size =
        LZ4_compress_default(
            im->buffer,
            compressed,
            im->size,
            maxCompressedSize
        );
    

    if (im->com_size <= 0)
    {
        free(compressed);
        printf("Compression failed\n");
        return 1;
    }
    free(im->buffer);
    im->buffer = compressed;
    return 0;
}

int decompress(struct Image* im){
    uint8_t* decompressed =
        (uint8_t*)malloc(im->size);

    int decompressedSize =
        LZ4_decompress_safe(
            im->buffer,
            decompressed,
            im->com_size,
            im->size
        );
    free(im->buffer);

    printf("com size: %d dcom size: %d ratio: %f\n", im->size, im->com_size, ((float)im->com_size/(float)im->size));

    if (decompressedSize < 0)
    {
        free(decompressed);
        printf("Decompression failed\n");
        return 1;
    }

    im->buffer = decompressed;
}


int main(){
    struct Image* image = load_png("len.png");
    uint8_t* test = malloc(sizeof(uint8_t) * image->size);
    memcpy(test, image->buffer, (size_t)image->size);
    clock_t t; 
    t = clock(); 
    
    compress(image);
    decompress(image);
    if (memcmp(image->buffer, test, (size_t)image->size) != 0){
        printf("rip\n");
    }


    //addImage(image, 100);

    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds 
    printf(" took %f seconds to execute \n", time_taken); 

    save_png(image, "output.png");
    decon_image(image);

    return 0;
}