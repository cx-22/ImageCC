#pragma once

#include <QApplication>
#include <QDrag>
#include <QLabel>
#include <QMimeData>
#include <QMouseEvent>

class DraggableLabel : public QLabel
{
    Q_OBJECT

public:
    explicit DraggableLabel(
        const QString& nodeName,
        QWidget* parent = nullptr
        )
        : QLabel(nodeName, parent),
        nodeName(nodeName)
    {
        setCursor(Qt::OpenHandCursor);
        setTextInteractionFlags(Qt::NoTextInteraction);
    }

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            dragStartPosition = event->pos();
        }

        QLabel::mousePressEvent(event);
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (!(event->buttons() & Qt::LeftButton))
            return;

        if ((event->pos() - dragStartPosition).manhattanLength()
            < QApplication::startDragDistance())
        {
            return;
        }

        QDrag* drag = new QDrag(this);

        QMimeData* mimeData = new QMimeData();
        mimeData->setText(nodeName);

        drag->setMimeData(mimeData);

        drag->exec(Qt::CopyAction);
    }

private:
    QPoint dragStartPosition;
    QString nodeName;
};
