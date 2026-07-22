#pragma once
#include <iostream>
#include <vector>

#include <QtCore/QObject>
#include <QtWidgets/QLabel>

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeDelegateModelRegistry>

#include "nodes.h"

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

class BaseNode : public NodeDelegateModel
{
    Q_OBJECT
public:
    BaseNode();

    int type;
    uint nid;
    std::string title;

    virtual QString modelName() const { return QString("Resulting Image"); }

    unsigned int nPorts(PortType const portType) const override;

    NodeDataType dataType(PortType const portType, PortIndex const portIndex) const override;

    std::shared_ptr<NodeData> outData(PortIndex const port) override;

    void setInData(std::shared_ptr<NodeData> nodeData, PortIndex const port) override;

    QWidget *embeddedWidget() override;

    bool resizable() const override { return true; }

    QString caption() const override {return QString::fromStdString(title);}
    QString name() const override {return QString::fromStdString(title);}

    std::vector<QLabel*> labels;
    std::vector<QPixmap*> pixmaps;
    std::vector<struct Image*> images;
    struct Node* node;
    std::shared_ptr<NodeData> nodeData;

    void buildImages();
    void update();
protected:
    bool eventFilter(QObject *object, QEvent *event) override;
    static void nodeFinished(struct Node* node);

};
