#include "mainwindow.h"
#include "inputthread.h"
#include <QApplication>
#include <QDebug>

//We want win+alt+ctrl to be pressed for the hotkeys
#define HOTKEY_MOD1 91
#define HOTKEY_MOD2 18
#define HOTKEY_MOD3 162

// Win+Alt+O to show the menu
#define HOTKEY_SHOW 79

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    InputThread keyThread;

//    //Set key modifiers
    keyThread.addModifierKey(HOTKEY_MOD1); //Win
    keyThread.addModifierKey(HOTKEY_MOD2); //Alt
    keyThread.addModifierKey(HOTKEY_MOD3); //Ctrl

    //Associate fullscreen show with hotkey

    QObject::connect(&keyThread, SIGNAL(sigOnKeyPressed(int,bool)), &w, SLOT(onRecieveKeyInput(int,bool)), Qt::QueuedConnection);

    //Start listening for hotkey presses (This occurs on a separate thread)
    keyThread.start();

    return a.exec();
    //Make sure the keyboard input thread finishes
    keyThread.quit();
    keyThread.wait();
}
