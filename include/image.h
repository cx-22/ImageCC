#pragma once
#include <stdint.h>
#include "types.h"
#include <QPixmap>
#include <QImage>

void printMat(const cv::Mat* mat);

void pixmapToMat(QPixmap* pixmap, cv::Mat* mat);
void matToPixmap(cv::Mat* mat, QPixmap* pixmap);