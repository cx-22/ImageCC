#include "image.h"
#include <stdio.h>
#include <string.h>


struct Image* makeImage(uint8_t* data, int rows, int cols)
{
    Image* input = new Image{};

    input->buffer = data;
    //input->channels = 4;
    input->rows = rows;
    input->cols = cols;
    input->size = 4 * rows * cols;
    //input->status = DECOM;

    return input;
}


void shallowCopyImage(Image* to, const Image* from)
{
    //to->status = from->status;
    //to->channels = from->channels;
    to->buffer = from->buffer;
    to->rows = from->rows;
    to->cols = from->cols;
    //to->com_size = from->com_size;
    to->size = from->size;
}


void deepCopyImage(Image* to, const Image* from)
{
    if (to->buffer != nullptr) {
        delete[] to->buffer;
    }

    to->buffer = new uint8_t[from->size];

    memcpy(
        to->buffer,
        from->buffer,
        from->size
    );

    //to->status = from->status;
    //to->channels = from->channels;
    to->rows = from->rows;
    to->cols = from->cols;
    //to->com_size = from->com_size;
    to->size = from->size;
}


void clearImage(Image* im)
{
    if (!im)
        return;

    if (im->buffer) {
        delete[] im->buffer;
    }

    im->buffer = nullptr;

    printf("cleared image\n");
}


void destroyImage(Image* im)
{
    if (!im)
        return;

    if (im->buffer) {
        delete[] im->buffer;
        im->buffer = nullptr;
    }

    delete im;

    printf("killed image\n");
}


void printImage(const Image* img)
{
    if (!img) {
        printf("Image: (null)\n");
        return;
    }

    printf("Image {\n");
    //printf("  status   : %d\n", img->status);
    //printf("  channels : %d\n", img->channels);
    printf("  buffer   : %p\n", (void*)img->buffer);
    printf("  rows     : %d\n", img->rows);
    printf("  cols     : %d\n", img->cols);
    //printf("  com_size : %llu\n", img->com_size);
    printf("  size     : %llu\n", img->size);

    if (img->buffer && img->size > 0) {
        printf("  buffer bytes (first up to 32): ");

        uint64_t n = img->size < 32 ? img->size : 32;

        for (uint64_t i = 0; i < n; i++) {
            printf("%u ", img->buffer[i]);
        }

        printf("\n");
    }

    printf("}\n");
}


void pixmapToImage(QPixmap* pixmap, Image* img)
{
    QImage qimg = pixmap->toImage()
        .convertToFormat(QImage::Format_RGBA8888);

    img->rows = qimg.height();
    img->cols = qimg.width();
    //img->channels = 4;

    img->size = img->rows * img->cols * 4;

    if (img->buffer) {
        delete[] img->buffer;
    }

    img->buffer = new uint8_t[img->size];

    memcpy(
        img->buffer,
        qimg.bits(),
        img->size
    );

    //img->status = DECOM;
}


void imageToPixmap(Image* img, QPixmap* pixmap)
{
    QImage qimg(
        img->buffer,
        img->cols,
        img->rows,
        img->cols * 4,
        QImage::Format_RGBA8888
    );

    *pixmap = QPixmap::fromImage(qimg.copy());
}