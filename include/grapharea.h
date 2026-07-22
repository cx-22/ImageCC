#ifndef GRAPHAREA_H
#define GRAPHAREA_H

#include <QtNodes/DataFlowGraphModel>
#include <QtNodes/DataFlowGraphicsScene>
#include <QtNodes/GraphicsView>
#include <QtNodes/NodeData>
#include <QtNodes/NodeDelegateModelRegistry>

#include <QtGui/QScreen>
#include <QtWidgets/QApplication>

#include <QWidget>
#include <QHBoxLayout>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>

#include "graph-model.h"

using namespace QtNodes;

class GraphView : public QtNodes::GraphicsView
{
public:
    explicit GraphView(
        QtNodes::BasicGraphicsScene* scene,
        QWidget* parent = nullptr
        );

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;
    void dropEvent(QDropEvent* event) override;
};



class GraphArea : public QWidget
{
    Q_OBJECT

public:
    GraphArea(QWidget *parent = nullptr);
    std::shared_ptr<NodeDelegateModelRegistry> registry;
    GraphModel* graphModel;
    BasicGraphicsScene* scene;
    GraphView* view;

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
};

#endif // GRAPHAREA_H
