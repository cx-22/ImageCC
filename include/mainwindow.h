#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "grapharea.h"
#include "sidebar.h"
#include "node-registry.h"
#include <QMainWindow>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    GraphArea* graph_area;
    SideBar* sidebar;
};

#endif // MAINWINDOW_H
