#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMenu>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    createTrayIcon();

    ui->setupUi(this);
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
    connect( onQuit, SIGNAL(triggered()), this, SLOT(onTrayExit()) );

    QAction *onShow = new QAction( "Do something", mTrayIcon );
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

}

void
MainWindow::onTrayShow()
{

}
