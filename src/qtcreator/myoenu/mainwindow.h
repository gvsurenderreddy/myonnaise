#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QKeyEvent>

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

protected:
    void keyPressEvent(QKeyEvent* event);

private:
    Ui::MainWindow *ui;

    QSystemTrayIcon *mTrayIcon;

public slots:
    void onTrayQuit();
    void onTrayShow();

};

#endif // MAINWINDOW_H
