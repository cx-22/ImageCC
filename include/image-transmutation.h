#pragma once

#include <QPixmap>
#include <QImage>
#include <cstdlib>
#include <cstring>

extern "C" {
#include "image.h"
}

void pixmapToImage(QPixmap* pixmap, Image* img);

void imageToPixmap(Image* img, QPixmap* pixmap);
