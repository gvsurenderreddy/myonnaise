#include "mainwindow.h"
#include "inputcontroller.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    InputController keycontrol;

    //Hide the title bar and show fullscreen
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //Start listening for hotkey presses (This occurs on a separate thread)
    keycontrol.startListen();

    return a.exec();
}
