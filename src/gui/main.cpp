#include <mainwindow.h>
#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setQuitOnLastWindowClosed(true);
    MainWindow w(NULL);
    w.showMaximized();
    return a.exec();
}
