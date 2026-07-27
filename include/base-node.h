#pragma once
#include <iostream>
#include <vector>
#include <atomic>

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <QVBoxLayout>
#include <QtConcurrent>
#include <QFuture>
#include <QThread>

#include <QtNodes/NodeDelegateModel>
#include <QtNodes/NodeDelegateModelRegistry>

#include "types.h"

using QtNodes::NodeData;
using QtNodes::NodeDataType;
using QtNodes::NodeDelegateModel;
using QtNodes::PortIndex;
using QtNodes::PortType;

#define PLAYING 0
#define PAUSED 1
#define NO_VIDEO 2

class BaseNode : public NodeDelegateModel
{
    Q_OBJECT
public:
    BaseNode();
    ~BaseNode();

    int type;
    //uint nid;
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
    std::shared_ptr<NodeData> nodeData;

    uchar status;
    std::vector<cv::Mat*> input_images;
    std::vector<cv::Mat*> output_images;
    std::vector<void*> params;

    struct function* func;

    bool video;
    QWidget* main_widget;
    QVBoxLayout* main_layout;

    cv::VideoCapture cap;
    cv::Mat rgb_dummy;
    std::atomic_char video_status;
    QFuture<void> future;

    void runVideo();
    void initialize();
    void update();
protected:
    bool eventFilter(QObject *object, QEvent *event) override;
public slots:
    void updateVideoFrame(cv::Mat frame);
signals:
    void videoFrameUpdated(cv::Mat frame);

};
