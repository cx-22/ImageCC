#pragma once
#include <stdint.h>
#include "types.h"
#include <QPixmap>
#include <QImage>

struct Image* makeImage(uint8_t* data, int rows, int cols);
void shallowCopyImage(struct Image* to, const struct Image* from);
void deepCopyImage(struct Image* to, const struct Image* from);
void clearImage(struct Image* im);
void destroyImage(struct Image* im);


void pixmapToImage(QPixmap* pixmap, Image* img);
void imageToPixmap(Image* img, QPixmap* pixmap);

void printImage(const struct Image* im);
