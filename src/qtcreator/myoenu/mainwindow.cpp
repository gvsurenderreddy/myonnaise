#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <Windows.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QPixmap>
#include <QImage>
#include <QDir>
#include <QPropertyAnimation>

/*
 * 79: o
 * 81: q
 * 37: left arrow
 * 39: right arrow
 */
#define HOTKEY_SHOWAPP 79
#define HOTKEY_HIDEAPP 81
#define HOTKEY_LEFT 37
#define HOTKEY_RIGHT 39
#define HOTKEY_UP 38
#define HOTKEY_DOWN 40

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

    m_scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(m_scene);

    QRect rec = QApplication::desktop()->screenGeometry();
    ui->graphicsView->setGeometry(rec);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void
MainWindow::createTrayIcon()
{
    QDir dir;

    mTrayIcon = new QSystemTrayIcon(this);
    mTrayIcon->setIcon(QIcon(dir.relativeFilePath(":/icons/icon_tray.png")));

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
    activateWindow();

    //Do fade in animation
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
    animation->setDuration(500);
    animation->setStartValue(0.0f);
    animation->setEndValue(1.0f);
    //Delete the allocated animation once the animation completes
    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
    animation->start();

    doInitialLayout();
}


void
MainWindow::hideOverlay()
{
    //Do fade out animation
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity", this);
    animation->setDuration(500);
    animation->setStartValue(1.0f);
    animation->setEndValue(0.0f);
    //Delete the allocated animation once the animation completes
    connect(animation, SIGNAL(finished()), animation, SLOT(hide()));
    connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
    animation->start();
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
        else if (keyCode == HOTKEY_HIDEAPP)
        {
            hideOverlay();
        }
        else if (keyCode == HOTKEY_UP)
        {
            onMovementUp();
        }
        else if (keyCode == HOTKEY_DOWN)
        {
            onMovementDown();
        }
    }
}


QGraphicsTextItem*
MainWindow::createTextAtPos(int x, int y, const QString &text)
{
    QGraphicsTextItem* textitem = m_scene->addText(text, QFont("Helvetica", 20));
    x = x - (int)(textitem->boundingRect().width() / 2.0f);
    y = y - (int)(textitem->boundingRect().height() / 2.0f);
    textitem->setPos(x,y);

    m_textItems.push_back(textitem);
    return textitem;
}


void
MainWindow::doInitialLayout()
{
    qDebug() << "Doing layout\n";
    m_selectionRect = m_scene->addRect(-100,-35, 200,70);

    //Create items
    //Hard-coding these due to time constraints
    createTextAtPos(0,-140, "Item 1");
    createTextAtPos(0,-70, "Item 2");
    createTextAtPos(0,0, "Item 3");
    createTextAtPos(0,70, "Item 4");
    createTextAtPos(0,140, "Item 5");
}


void
MainWindow::onMovementUp()
{
    qDebug() << "Doing movement up\n";
    for (auto it = m_textItems.begin(); it != m_textItems.end(); it++)
    {
        QGraphicsTextItem* text = *it;

        //Animate the movement
        QPropertyAnimation *animation = new QPropertyAnimation(text, "pos", text);
        animation->setDuration(200);
        animation->setStartValue(text->pos());
        animation->setEndValue(QPointF(text->pos().x(), text->pos().y() - 70.0f));
        //Delete the allocated animation once the animation completes
        connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
        animation->start();
    }
}


void
MainWindow::onMovementDown()
{
    qDebug() << "Doing movement down\n";
    for (auto it = m_textItems.begin(); it != m_textItems.end(); it++)
    {
        QGraphicsTextItem* text = *it;

        //Animate the movement
        QPropertyAnimation *animation = new QPropertyAnimation(text, "pos", text);
        animation->setDuration(200);
        animation->setStartValue(text->pos());
        animation->setEndValue(QPointF(text->pos().x(), text->pos().y() + 70.0f));
        //Delete the allocated animation once the animation completes
        connect(animation, SIGNAL(finished()), animation, SLOT(deleteLater()));
        animation->start();
    }
}
