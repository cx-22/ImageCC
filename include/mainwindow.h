#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "grapharea.h"
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
};

#endif // MAINWINDOW_H
