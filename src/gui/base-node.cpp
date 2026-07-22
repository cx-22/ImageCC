#include "base-node.h"
#include "PixmapData.hpp"
#include "functions.h"
#include <QtCore/QEvent>
#include <QtCore/QDir>
#include <QtCore/QEvent>

#include <QtWidgets/QFileDialog>

BaseNode::BaseNode(){}

unsigned int BaseNode::nPorts(PortType portType) const
{
    uchar i = func_list[type].n_inputs;
    uchar o = func_list[type].n_outputs;

    return portType == PortType::In ? func_list[type].n_inputs : func_list[type].n_outputs;

}

bool BaseNode::eventFilter(QObject *object, QEvent *event)
{
    switch (type){
    case -2:
        if (object == _label) {
            int w = _label->width();
            int h = _label->height();

            if (event->type() == QEvent::MouseButtonPress) {
                QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                                tr("Open Image"),
                                                                QDir::homePath(),
                                                                tr("Image Files (*.png *.jpg *.bmp)"));

                _pixmap = QPixmap(fileName);

                _label->setPixmap(_pixmap.scaled(w, h, Qt::KeepAspectRatio));

                Q_EMIT dataUpdated(0);

                return true;
            } else if (event->type() == QEvent::Resize) {
                if (!_pixmap.isNull())
                    _label->setPixmap(_pixmap.scaled(w, h, Qt::KeepAspectRatio));
            }
        }

        return false;
    case -1:
        if (object == _label) {
            int w = _label->width();
            int h = _label->height();

            if (event->type() == QEvent::Resize) {
                auto d = std::dynamic_pointer_cast<PixmapData>(_nodeData);
                if (d) {
                    _label->setPixmap(d->pixmap().scaled(w, h, Qt::KeepAspectRatio));
                }
            }
        }
    default:
        if (object == _label) {
            int w = _label->width();
            int h = _label->height();

            if (event->type() == QEvent::Resize) {
                auto d = std::dynamic_pointer_cast<PixmapData>(_nodeData);
                if (d) {
                    _label->setPixmap(d->pixmap().scaled(w, h, Qt::KeepAspectRatio));
                }
            }
        }
    }

    return false;
}

void BaseNode::setInData(std::shared_ptr<NodeData> nodeData, PortIndex const)
{
    _nodeData = nodeData;

    if (_nodeData) {
        auto d = std::dynamic_pointer_cast<PixmapData>(_nodeData);

        int w = _label->width();
        int h = _label->height();

        _label->setPixmap(d->pixmap().scaled(w, h, Qt::KeepAspectRatio));
    } else {
        _label->setPixmap(QPixmap());
    }

    Q_EMIT dataUpdated(0);
}

NodeDataType BaseNode::dataType(PortType const, PortIndex const) const
{
    return PixmapData().type();
}

std::shared_ptr<NodeData> BaseNode::outData(PortIndex)
{
    switch (type){
    case -2:
        return std::make_shared<PixmapData>(_pixmap);
    case -1:
        return _nodeData;
    default:
        return _nodeData;
    }
}
