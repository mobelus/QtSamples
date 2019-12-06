#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
/*
    cmbx = new CustomComboBox(this);

    cmbx->addItem("q1w2e3r4t5y6u7i8o9p0[a1s2d3f4g5h6j7k8l9;0");
    cmbx->addItem("a1s2d3f4g5h6j7k8l9;0'1zx23c4v5bn67,89.");
    cmbx->addItem("z1x2c3v4b5n6m7,8.9/01q1w2e3r4t5y6u7i8o9p");
    cmbx->setGeometry(100, 100, 100, 25);

//*/

    //cmbx1 = new QComboBox(this);
    //cmbx1->setGeometry(100, 100, 100, 25);

    ui->comboBox->addItem("q1w2e3r4t5y6u7i8o9p0[a1s2d3f4g5h6j7k8l9;0348v345b349t5gbgbotgobv thvb ");
    ui->comboBox->addItem("a1s2d3f4g5h6j7k8l9;0'1zx23c4v5bn67,8934543534534fdsgerfgrg45.");
    ui->comboBox->addItem("z1x2c3v4b5n6m7,8.9/01q1w2e3r4t5y6u7i8o9p34r3r4fr4f45tg5");


}

MainWindow::~MainWindow()
{
    delete ui;
}
