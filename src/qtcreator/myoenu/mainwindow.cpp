#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <Windows.h>

/*
 * 97: o
 * 37: left arrow
 */
#define HOTKEY_SHOWAPP 91
#define HOTKEY_LEFT 37

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    createTrayIcon();

    //Hide title bar
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

    //Set translucent so background is painted behind
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground, true);

    ui->setupUi(this);

    //Start as a 1x1 window so we can still count as visible
    //setFixedSize(1,1);

    //Set focus so we can get key events
    setFocusPolicy(Qt::StrongFocus);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::createTrayIcon()
{
    mTrayIcon = new QSystemTrayIcon(this);

    QAction *onQuit = new QAction( "Exit", mTrayIcon );
    connect( onQuit, SIGNAL(triggered()), this, SLOT(onTrayQuit()) );

    QAction *onShow = new QAction( "Show", mTrayIcon );
    connect( onShow, SIGNAL(triggered()), this, SLOT(onTrayShow()) );

    QMenu *trayMenu = new QMenu;
    trayMenu->addAction( onQuit );
    trayMenu->addAction( onShow );

    mTrayIcon->setContextMenu( trayMenu );

    mTrayIcon->show();
}

void
MainWindow::showOverlay()
{
    //Show fullscreen
    showFullScreen();
    //Bring window to top
    // Windows does not allow windows to bring themselves to the front, so this hack solves this somewhat
    ::SetWindowPos((HWND)effectiveWinId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    ::SetWindowPos((HWND)effectiveWinId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
    // End of hack

    //Do rest of methods to try bring window to the front
    raise();
    show();
    activateWindow();
}

void
MainWindow::onTrayQuit()
{
    QApplication::quit();
}

void
MainWindow::onTrayShow()
{
    showOverlay();
}

/**
 * @brief MainWindow::onRecieveKeyInput
 * @param keyCode - ASCII code of key recieved
 * @param pressedDown - true if key is pressed, false if key released
 * Method called each time a key state change has happened, while all modifier
 * keys are pressed down.
 */
void
MainWindow::onRecieveKeyInput(int keyCode, bool pressedDown)
{
    qDebug() << "Recieved key event " << keyCode << "\n";
    if (pressedDown)
    {
        if (keyCode == HOTKEY_SHOWAPP)
        {
            showOverlay();
        }
        else if (keyCode == HOTKEY_LEFT)
        {

        }

    }
}
