#include "grapharea.h"
#include "node-registry.h"
#include "nodes.h"

GraphArea::GraphArea(QWidget *parent){
    setAcceptDrops(true);
    std::shared_ptr<NodeDelegateModelRegistry> registry = build_reg();;
    graphModel = new GraphModel(registry);
    scene = new DataFlowGraphicsScene(*graphModel);
    view = new GraphView(scene);

    view->viewport()->setAcceptDrops(true);
    view->viewport()->installEventFilter(this);

    QHBoxLayout* layout = new QHBoxLayout(this);

    layout->addWidget(view);
}

void GraphArea::dragEnterEvent(
    QDragEnterEvent* event
    )
{
    if (event->mimeData()->hasText())
    {
        event->acceptProposedAction();
    }
}


void GraphArea::dropEvent(
    QDropEvent* event
    )
{
    QString nodeName =
        event->mimeData()->text();

    //auto id = graphModel->addNode(nodeName);

    auto id = graphModel->addNode("Input");
    qDebug() << "Created node id:" << id;

    qDebug() << "Dropped node:"
             << nodeName;


    event->acceptProposedAction();
}

bool GraphArea::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == view->viewport())
    {
        if (event->type() == QEvent::Drop)
        {
            auto* dropEvent =
                static_cast<QDropEvent*>(event);

            QString nodeName = dropEvent->mimeData()->text();

            qDebug() << "my nod" << graphModel->_nextNodeId;


            createNode(graphModel->_nextNodeId, g_name_func_map[nodeName.toStdString()]);
            auto id = graphModel->addNode(nodeName);

            QPoint viewPos = dropEvent->position().toPoint();
            QPointF scenePos = view->mapToScene(viewPos);

            graphModel->setNodeData(
                id,
                QtNodes::NodeRole::Position,
                scenePos
                );

            qDebug() << "qtn node:" << id;

            dropEvent->acceptProposedAction();

            return true;
        }

        if (event->type() == QEvent::DragEnter)
        {
            auto* dragEvent =
                static_cast<QDragEnterEvent*>(event);

            if (dragEvent->mimeData()->hasText())
            {
                dragEvent->acceptProposedAction();
                return true;
            }
        }
    }

    return QWidget::eventFilter(watched, event);
}


GraphView::GraphView(
    QtNodes::BasicGraphicsScene* scene,
    QWidget* parent
    )
    : QtNodes::GraphicsView(scene, parent)
{
    setAcceptDrops(true);
}


void GraphView::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasText())
    {
        event->acceptProposedAction();
    }
}


void GraphView::dragMoveEvent(QDragMoveEvent* event)
{
    if (event->mimeData()->hasText())
    {
        event->acceptProposedAction();
    }
}


void GraphView::dropEvent(QDropEvent* event)
{
    QString nodeName =
        event->mimeData()->text();

    qDebug() << "Dropped node:"
             << nodeName;

    event->acceptProposedAction();
}
