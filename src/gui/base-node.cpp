#include "base-node.h"
#include "PixmapData.hpp"
#include "functions.h"
#include <QtCore/QEvent>
#include <QtCore/QDir>
#include <QtCore/QEvent>
#include "image-transmutation.h"

#include <QtWidgets/QFileDialog>

BaseNode::BaseNode(){
    nid = - 4;

    QLabel* label = new QLabel();
    label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    label->setMinimumSize(200, 200);
    label->installEventFilter(this);
    labels.push_back(label);
}

void BaseNode::buildImages(){
    struct NodeEntry* ne = NULL;
    int i;
    if (nid != -1) {
        if (num_nodes == 0) {
            ne = node_list[0];
            if (ne->nid == nid) {
                node = ne->node;
            }
        }
        else {
            for (i = 0; i < num_nodes; i++) {
                ne = node_list[i];
                if (ne->nid == nid) {
                    node = ne->node;
                    break;
                }
            }
        }
    }
    node->userdata = this;
    node->on_complete = BaseNode::nodeFinished;

    i = 0;

    do{
        images.push_back(node->output_images[i]);
    } while (i < func_list[type].n_outputs);

    images.resize(node->func->n_outputs);
    for (int i = 0; i < node->func->n_outputs; i++){

        if (i != node->func->n_outputs - 1){
            QLabel* label = new QLabel();
            label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
            label->setMinimumSize(200, 200);
            labels.push_back(label);
        }

        QPixmap* pix = new QPixmap();
        pixmaps.push_back(pix);
    }
    qDebug() << "iages buitl";
}


unsigned int BaseNode::nPorts(PortType portType) const
{
    return portType == PortType::In ? node->func->n_inputs : node->func->n_outputs;
}

bool BaseNode::eventFilter(QObject *object, QEvent *event)
{
    switch (type){
    case -2:
        if (object == labels[0]) {
            int w = labels[0]->width();
            int h = labels[0]->height();

            if (event->type() == QEvent::MouseButtonPress) {
                QString fileName = QFileDialog::getOpenFileName(nullptr,
                                                                tr("Open Image"),
                                                                QDir::homePath(),
                                                                tr("Image Files (*.png *.jpg *.bmp)"));

                pixmaps[0] = new QPixmap(fileName);
                labels[0]->setPixmap(pixmaps[0]->scaled(w, h, Qt::KeepAspectRatio));

                pixmapToImage(pixmaps[0], images[0]);
                Q_EMIT dataUpdated(0);

                return true;
            } else if (event->type() == QEvent::Resize) {
                if (!pixmaps.empty()){
                    if (pixmaps[0] != NULL){
                        labels[0]->setPixmap(pixmaps[0]->scaled(w, h, Qt::KeepAspectRatio));

                    }
                }
            }
        }

        return false;
    case -1:
        if (object == labels[0]) {
            int w = labels[0]->width();
            int h = labels[0]->height();

            if (event->type() == QEvent::Resize) {
                if (!pixmaps.empty()) {
                    if (pixmaps[0] != NULL) {
                        labels[0]->setPixmap(pixmaps[0]->scaled(w, h, Qt::KeepAspectRatio));
                    }
                }
            }
        }
    default:
        if (object == labels[0]) {
            int w = labels[0]->width();
            int h = labels[0]->height();

            if (event->type() == QEvent::Resize) {
                if (!pixmaps.empty()) {
                    if (pixmaps[0] != NULL) {
                        labels[0]->setPixmap(pixmaps[0]->scaled(w, h, Qt::KeepAspectRatio));
                    }
                }
            }
        }
    }

    return false;
}

void BaseNode::setInData(std::shared_ptr<NodeData> nodeData, PortIndex const)
{
    update();
}

NodeDataType BaseNode::dataType(PortType const, PortIndex const) const
{
    return PixmapData().type();
}

std::shared_ptr<NodeData> BaseNode::outData(PortIndex pi)
{
    switch (type){
    case -2:
        return std::make_shared<PixmapData>(*pixmaps[0]);
    case -1:
        return nodeData;
    default:
        return std::make_shared<PixmapData>(*pixmaps[pi]);
    }
}


QWidget* BaseNode::embeddedWidget() {
    if(!labels.empty())
        return labels[0];
    return NULL;
}

void BaseNode::update()
{
    if (node->status == ACTIVE){
        for (int i = 0; i < node->func->n_outputs; i++){
            imageToPixmap(images[i], pixmaps[i]);
            int w = labels[0]->width();
            int h = labels[0]->height();

            if (!pixmaps.empty()) {
                if (pixmaps[0] != NULL) {
                    labels[0]->setPixmap(pixmaps[0]->scaled(w, h, Qt::KeepAspectRatio));
                }
            }
        }
    } else {
        for (int i = 0; i < node->func->n_outputs; i++){
            pixmaps[i]->fill(Qt::transparent);
        }
    }
    Q_EMIT dataUpdated(0);
}

void BaseNode::nodeFinished(struct Node* node)
{
    BaseNode* base = static_cast<BaseNode*>(node->userdata);

    if (base)
        base->update();
}