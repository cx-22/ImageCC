#include "image.h"
#include <stdio.h>
#include <string.h>


void printMat(const cv::Mat* mat)
{
    if (!mat) {
        printf("Mat: (null)\n");
        return;
    }

    printf("Mat {\n");
    printf("  data     : %p\n", (void*)mat->data);
    printf("  rows     : %d\n", mat->rows);
    printf("  cols     : %d\n", mat->cols);
    printf("  channels : %d\n", mat->channels());
    printf("  type     : %d\n", mat->type());

    if (mat->data && mat->total() > 0) {
        printf("  data bytes (first up to 32): ");

        size_t size = mat->total() * mat->elemSize();
        size_t n = size < 32 ? size : 32;

        const uchar* buffer = mat->data;

        for (size_t i = 0; i < n; i++) {
            printf("%u ", buffer[i]);
        }

        printf("\n");
    }
    printf("}\n");
}


void pixmapToMat(QPixmap* pixmap, cv::Mat* mat)
{
    QImage qimg = pixmap->toImage()
        .convertToFormat(QImage::Format_RGBA8888);

    // Deep copy: cv::Mat owns its own memory
    *mat = cv::Mat(
        qimg.height(),
        qimg.width(),
        CV_8UC4,
        qimg.bits(),
        qimg.bytesPerLine()
    ).clone();
}


void matToPixmap(cv::Mat* mat, QPixmap* pixmap)
{
    QImage qimg(
        mat->data,
        mat->cols,
        mat->rows,
        mat->step,
        QImage::Format_RGBA8888
    );

    *pixmap = QPixmap::fromImage(qimg);
}