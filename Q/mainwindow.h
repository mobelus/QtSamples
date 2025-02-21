#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void timerLoop(int msecs = 5000);
    void timerMR(int msecs = 5000);
    void timerVpn(int msces = 5000);

    void autoClickLoop();
    void startMouseMoveLoop();
    void startVpn();
    void startMR();

    void getCurPos(int& xx, int& yy);
    void setMouseToLeftTop(int xStart, int yStart);

    void moveLclickAndReturn(int x, int y);
    void moveLLclickAndReturn(int x, int y);
    void moveLLLclickAndReturn(int x, int y);
    void moveRclickAndReturn(int x, int y);

    void mouseMove(int x, int y);
    void mouseL();
    void mouseDoubleL();
    void mouseTrippleL();
    void mouseR();

    void mouseClick(int button);
    void clickMouse();
    void click2();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    int xStart=0, yStart=0;

    QTimer *timeraAutoclick{nullptr};
};
#endif // MAINWINDOW_H
