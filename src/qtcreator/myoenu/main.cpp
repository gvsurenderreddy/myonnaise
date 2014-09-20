#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //Hide the title bar and show fullscreen
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.showFullScreen();

    return a.exec();
}
