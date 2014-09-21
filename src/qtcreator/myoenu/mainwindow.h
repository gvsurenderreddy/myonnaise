#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <vector>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createTrayIcon();

    void showOverlay();

    void hideOverlay();

private:

    QGraphicsTextItem* createTextAtPos(int x, int y, const QString &text);

    void doInitialLayout();

    void onMovementUp();

    void onMovementDown();


    Ui::MainWindow *ui;
    QSystemTrayIcon *mTrayIcon;
    QGraphicsScene *m_scene;
    std::vector<QGraphicsTextItem *> m_textItems;
    QGraphicsRectItem *m_selectionRect;

public slots:
    void onTrayQuit();
    void onTrayShow();
    void onRecieveKeyInput(int keyCode, bool pressedDown);

};

#endif // MAINWINDOW_H
