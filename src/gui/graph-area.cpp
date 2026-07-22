#include "grapharea.h"
#include "node-registry.h"

GraphArea::GraphArea(QWidget *parent){
    std::shared_ptr<NodeDelegateModelRegistry> registry = build_reg();;
    dataFlowGraphModel = new DataFlowGraphModel(registry);
    scene = new DataFlowGraphicsScene(*dataFlowGraphModel);
    view = new GraphicsView(scene);

    QHBoxLayout* layout = new QHBoxLayout(this);

    layout->addWidget(view);
}
