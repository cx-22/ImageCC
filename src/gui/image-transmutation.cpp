#include "image-transmutation.h"

void pixmapToImage(QPixmap* pixmap, Image* img)
{
    QImage qimg = pixmap->toImage()
                      .convertToFormat(QImage::Format_RGBA8888);

    img->rows = qimg.height();
    img->cols = qimg.width();
    img->channels = 4;

    img->size = img->rows * img->cols * 4;

    img->buffer = (uint8_t*)malloc(img->size);

    if (!img->buffer)
    {
        img->size = 0;
        return;
    }

    memcpy(
        img->buffer,
        qimg.bits(),
        img->size
        );

    img->status = DECOM;


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
