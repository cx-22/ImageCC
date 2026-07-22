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

using namespace QtNodes;

class GraphArea : public QWidget
{
    Q_OBJECT

public:
    GraphArea(QWidget *parent = nullptr);
    std::shared_ptr<NodeDelegateModelRegistry> registry;
    DataFlowGraphModel* dataFlowGraphModel;
    DataFlowGraphicsScene* scene;
    GraphicsView* view;
};

#endif // GRAPHAREA_H
