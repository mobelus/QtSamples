#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "preferences.h"

#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tmr = new QTimer(this); // Создаем объект класса QTimer и передаем адрес переменной

    time = 1000;
    //tmr->setInterval(1000); // Задаем интервал таймера
    tmr->setInterval(time); // Задаем интервал таймера


    connect(tmr, SIGNAL(timeout()), this, SLOT(updateTime())); // Подключаем сигнал таймера к нашему слоту
//    tmr->start(); // Запускаем таймер
}

MainWindow::~MainWindow()
{
    delete ui;
    delete tmr;
}

void MainWindow::updateTime()
{
    ui->label->setText(QTime::currentTime().toString()); // Обновляем время на экране

    time += 1000;
    tmr->setInterval(time); // Задаем интервал таймера
}

void MainWindow::on_pushButton_clicked()
{
//    this->setAttribute(Qt::WA_ShowModal, true);
//    this->setWindowModality(Qt::ApplicationModal);


    /*
    QMainWindow *window = new QMainWindow(this);
    window->setWindowTitle("Modal");
    window->setAttribute(Qt::WA_ShowModal, true);
    window->setWindowModality(Qt::ApplicationModal);
    window->show();


    setWindowTitle("Non-Modal");
    */



    /*
    QDialog *window = new QDialog(this);
    window->setWindowTitle("Modal");
    window->setAttribute(Qt::WA_ShowModal, true);
    window->setWindowModality(Qt::ApplicationModal);
    window->show();

    setWindowTitle("Non-Modal");
    */


   QMessageBox *window = new QMessageBox(this);
   window->setWindowTitle("Modal");
   window->setAttribute(Qt::WA_ShowModal, true);
   window->setWindowModality(Qt::ApplicationModal);
   window->show();

   setWindowTitle("Non-Modal");


//    Preferences* _Preferences = new Preferences(this);
//    _Preferences->show();

//    this->setAttribute(Qt::WA_ShowModal, false);


    //window->setAttribute(Qt::WA_ShowModal, true);
    //window->setWindowModality(Qt::ApplicationModal);
}




void MainWindow::on_pushButton_2_clicked()
{
      // is Modal
      QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Test", "Quit?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes) {
        //qDebug() << "Yes was clicked";
        QApplication::quit();
      } else {
        //qDebug() << "Yes was *not* clicked";
      }

}
