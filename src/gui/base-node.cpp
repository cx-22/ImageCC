#include "base-node.h"
#include "functions.h"
#include <QtCore/QEvent>
#include <QtCore/QDir>
#include <QtCore/QEvent>
#include "image.h"

#include <QtWidgets/QFileDialog>
#include <QMenu>
#include <QClipboard>
#include <QApplication>
#include <QMouseEvent>

class ImageData : public NodeData
{
public:
    ImageData(cv::Mat* img) : image(img) {}

    NodeDataType type() const override
    {
        return NodeDataType{ "image", "Image" };
    }

    connect(
        this,
        &BaseNode::videoFrameUpdated,
        this,
        &BaseNode::updateVideoFrame,
        Qt::QueuedConnection
    );

    cv::Mat* image;
};

BaseNode::BaseNode(){
    main_widget = new QWidget();
    main_layout = new QVBoxLayout();

	main_widget->setLayout(main_layout);

    //nid = - 4;
	status = EMPTY;
    video_status.store(NO_VIDEO);
}

void BaseNode::initialize(){
	func = &func_list[type];
    int i = 0;


    if (func->n_outputs == 0){
        return;
	}

    for (i = 0; i < func->n_outputs; i++) {
		cv::Mat* img = new cv::Mat();
        output_images.push_back(img);
    }

    output_images.resize(func->n_outputs);
    for (i = 0; i < func->n_outputs; i++){
        QLabel* label = new QLabel();
        label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        label->setMinimumSize(100, 100);
        labels.push_back(label);
        label->installEventFilter(this);
        main_layout->addWidget(label);

        QPixmap* pix = new QPixmap();
        pixmaps.push_back(pix);
    }

    input_images.resize(func->n_inputs);
    for (i = 0; i < func->n_inputs; i++) {
        cv::Mat* img = NULL;
        input_images.push_back(img);
    }

    params.resize(func->n_params);
    for (i = 0; i < func->n_params; i++) {
        params.push_back(NULL);
    }

    qDebug() << "iages buitl";
}


unsigned int BaseNode::nPorts(PortType portType) const
{
    return portType == PortType::In ? func->n_inputs : func->n_outputs;
}

bool BaseNode::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

        // Right click menu for all image labels
        for (int i = 0; i < labels.size(); i++)
        {
            if (object == labels[i] && mouseEvent->button() == Qt::RightButton)
            {
                if (pixmaps[i] == nullptr || pixmaps[i]->isNull())
                    return true;

                QMenu menu;

                QAction* saveAction = menu.addAction("Save Image");
                QAction* copyAction = menu.addAction("Copy to Clipboard");

                QAction* selected = menu.exec(mouseEvent->globalPosition().toPoint());

                if (selected == saveAction)
                {
                    QString fileName = QFileDialog::getSaveFileName(
                        nullptr,
                        tr("Save Image"),
                        QDir::homePath(),
                        tr("Image Files (*.png *.jpg *.bmp)")
                    );

                    if (!fileName.isEmpty())
                    {
                        pixmaps[i]->save(fileName);
                    }
                }
                else if (selected == copyAction)
                {
                    QApplication::clipboard()->setPixmap(*pixmaps[i]);
                }

                return true;
            }
        }
    }


    if (type == INPUT && !video){
        if (object == labels[0]){
            int w = labels[0]->width();
            int h = labels[0]->height();

            if (event->type() == QEvent::MouseButtonPress){
                QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

                // Ignore right click (handled above)
                if (mouseEvent->button() == Qt::RightButton){
                    return true;
                }

                QString fileName = QFileDialog::getOpenFileName(
                    nullptr,
                    tr("Open Image"),
                    QDir::homePath(),
                    tr("Image Files (*.png *.jpg *.jpeg *.svg *.ppm *.bmp)")
                );

                if (!fileName.isEmpty()){
                    pixmaps[0] = new QPixmap(fileName);

                    labels[0]->setPixmap(
                        pixmaps[0]->scaled(
                            w,
                            h,
                            Qt::KeepAspectRatio
                        )
                    );

                    pixmapToMat(pixmaps[0], output_images[0]);

                    status = ACTIVE;
                    Q_EMIT dataUpdated(0);
                }
                return true;
            }
            else if (event->type() == QEvent::Resize){
                if (!pixmaps.empty() && pixmaps[0] != nullptr){
                    labels[0]->setPixmap(
                        pixmaps[0]->scaled(
                            w,
                            h,
                            Qt::KeepAspectRatio
                        )
                    );
                }
            }
        }

        return false;
    } else if (type == INPUT && video){
        int w = labels[0]->width();
        int h = labels[0]->height();

        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

            // Ignore right click (handled above)
            if (mouseEvent->button() == Qt::RightButton) {
                return true;
            }

            QString fileName = QFileDialog::getOpenFileName(
                nullptr,
                tr("Open Video"),
                QDir::homePath(),
                tr("Video Files (*.mp4 *.avi *.mkv)")
            );

            if (!fileName.isEmpty()) {
                cap.open(fileName.toStdString());
                status = ACTIVE;
                video_status.store(PLAYING);
                future = QtConcurrent::run(&BaseNode::runVideo, this);
            }
            return true;
        }
        else if (event->type() == QEvent::Resize) {
            if (!pixmaps.empty() && pixmaps[0] != nullptr) {
                labels[0]->setPixmap(
                    pixmaps[0]->scaled(
                        w,
                        h,
                        Qt::KeepAspectRatio
                    )
                );
            }
        }
    } else{
        if (event->type() != QEvent::Resize){
            return false;
        }

        for (int i = 0; i < func->n_outputs; i++){
            int w = labels[i]->width();
            int h = labels[i]->height();

            if (pixmaps[i] != nullptr){
                labels[i]->setPixmap(
                    pixmaps[i]->scaled(
                        w,
                        h,
                        Qt::KeepAspectRatio
                    )
                );
            }
        }
        return false;
    }
}

void BaseNode::runVideo() {
    int fps = cap.get(cv::CAP_PROP_FPS);
    float pause = 1000.0 / static_cast<float>(fps);

    while (video_status.load() == PLAYING) {
        if (cap.read(rgb_dummy)) {
            cv::cvtColor(rgb_dummy, *output_images[0], cv::COLOR_BGR2RGBA);
            Q_EMIT videoFrameUpdated();
            QThread::msleep(pause);
        } else {
            cap.set(cv::CAP_PROP_POS_FRAMES, 0);
        }

    }
}

void BaseNode::updateVideoFrame(cv::Mat frame)
{
    matToPixmap(&frame, pixmaps[0]);

    labels[0]->setPixmap(
        pixmaps[0]->scaled(
            labels[0]->width(),
            labels[0]->height(),
            Qt::KeepAspectRatio
        )
    );
}

void BaseNode::setInData(std::shared_ptr<NodeData> nodeData, PortIndex const index)
{
    auto data = std::dynamic_pointer_cast<ImageData>(nodeData);

    if (!data)
        return;

    input_images[index] = data->image;

    // Check if all inputs are available
    bool ready = true;
    for (int i = 0; i < func->n_inputs; i++) {
        if (input_images[i] == NULL || input_images[i]->empty()) {
            ready = false;
            break;
        }
    }

    if (ready) {
        status = ACTIVE;
        update();
    }
}


std::shared_ptr<NodeData> BaseNode::outData(PortIndex pi)
{
    if (pi >= output_images.size())
        return nullptr;

    return std::make_shared<ImageData>(output_images[pi]);
}

NodeDataType BaseNode::dataType(PortType const, PortIndex const) const
{
    return NodeDataType{ "image", "Image" };
}

QWidget* BaseNode::embeddedWidget() {
    return main_widget;
}

void BaseNode::update()
{
    if (status != ACTIVE){
        return;
    }
    int i;

    for (i = 0; i < func->n_inputs; i++) {
        if (input_images[i] == nullptr || input_images.empty()) {
            return;
		}
    }
    func->op(
        input_images.data(),
        output_images.data(),
        params.data()
    );

    for (i = 0; i < func->n_outputs; i++) {
        if (output_images[i] == nullptr)
            continue;

        matToPixmap(output_images[i], pixmaps[i]);

        if (i < labels.size() && pixmaps[i] != nullptr) {
            int w = labels[i]->width();
            int h = labels[i]->height();

            labels[i]->setPixmap(
                pixmaps[i]->scaled(w, h, Qt::KeepAspectRatio)
            );
        }
    }

    for (i = 0; i < func->n_outputs; i++) {
        Q_EMIT dataUpdated(i);
    }
}

BaseNode::~BaseNode() {
    video_status.store(NO_VIDEO);
    future.waitForFinished();
    
}