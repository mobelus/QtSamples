#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	QDateTime dt = QDateTime::currentDateTime();
    QDir dr; dr.mkdir("C:/Users/Public/_EMU/_Dis_" + dt.toString("dd_MM_yyyy_hh_mm_ss_z"));


    bool maint = isInMMode();

    QDateTime dt = QDateTime::currentDateTime();
    QDir dr;
    maint
    ? dr.mkdir("C:/Users/Public/_EMU/_Dis_For_OO_HotService_T_" + dt.toString("dd_MM_yyyy_hh_mm_ss_z"))
    : dr.mkdir("C:/Users/Public/_EMU/_Dis_For_OO_HotService_F_" + dt.toString("dd_MM_yyyy_hh_mm_ss_z"))
    ;

    //bool maint = isInMMode();
    /*
    QDateTime dt = QDateTime::currentDateTime();
    QDir dr;
    maint
        ? dr.mkdir("C:/Users/Public/_EMU/_Dis_For_TT_HotService_T_" + dt.toString("dd_MM_yyyy_hh_mm_ss_z"))
        : dr.mkdir("C:/Users/Public/_EMU/_Dis_For_TT_HotService_F_" + dt.toString("dd_MM_yyyy_hh_mm_ss_z"))
        ;
    */


	ui->setupUi(this);

	ui->pushButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
	ui->pushButton_2->setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::on_pushButton_clicked()
{
	ui->pushButton->setEnabled(false);
	ui->pushButton_2->setEnabled(true);

	ui->pushButton->clearFocus();
	ui->pushButton_2->clearFocus();
	//ui->pushButton->leaveEvent()

//	ui->pushButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
//	ui->pushButton_2->setFocusPolicy(Qt::FocusPolicy::NoFocus);

	ui->pushButton->setAutoFillBackground( false );
	ui->pushButton_2->setAutoFillBackground( false );
}

void MainWindow::on_pushButton_2_clicked()
{
	ui->pushButton->setEnabled(true);
	ui->pushButton_2->setEnabled(false);

	ui->pushButton->clearFocus();
	ui->pushButton_2->clearFocus();

//	ui->pushButton->setFocusPolicy(Qt::FocusPolicy::NoFocus);
//	ui->pushButton_2->setFocusPolicy(Qt::FocusPolicy::NoFocus);

	ui->pushButton->setAutoFillBackground( false );
	ui->pushButton_2->setAutoFillBackground( false );

}

void MainWindow::on_pushButton_3_clicked()
{

}

void MainWindow::on_pushButton_4_clicked()
{

}
