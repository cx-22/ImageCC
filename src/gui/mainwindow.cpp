#include "mainwindow.h"
#include "nodes.h"
#include "functions.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* main_widget = new QWidget(this);
    this->setCentralWidget(main_widget);

    QVBoxLayout* main_layout = new QVBoxLayout(main_widget);



    buildNodeList();
    buildFuncs();

    graph_area = new GraphArea();

    main_layout->addWidget(graph_area);
}


MainWindow::~MainWindow(){
    killList();
    killFuncs();
}
