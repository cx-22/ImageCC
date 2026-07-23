#include "mainwindow.h"
#include "functions.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qDebug() << "MainWindow constructor";
    buildFuncs();

    qDebug() << "map input is:" << g_name_func_map["Input"];
    qDebug() << "func size:" << func_list.size();
    QWidget* main_widget = new QWidget(this);
    this->setCentralWidget(main_widget);

    QHBoxLayout* main_layout = new QHBoxLayout(main_widget);


    graph_area = new GraphArea(this);
    sidebar = new SideBar(this);

    main_layout->addWidget(sidebar);
    main_layout->addWidget(graph_area);
}


MainWindow::~MainWindow(){
    //killList();
    //killFuncs();
}
