#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    createTrayIcon();

    //Set translucent so background is painted behind
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_NoSystemBackground, true);

    ui->setupUi(this);

    //Start as a 1x1 window so we can still count as visible
    setFixedSize(1,1);

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
MainWindow::onTrayQuit()
{
    QApplication::quit();
}

void
MainWindow::onTrayShow()
{
    showFullScreen();
}

void
MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Down) {
        printf("SHOW");
        showFullScreen();
    }
}
