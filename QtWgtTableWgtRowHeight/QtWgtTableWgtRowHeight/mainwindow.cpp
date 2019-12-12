#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QToolTip>
#include <QCursor>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
//*

	//
	// MAGIC WITH THE RESIZE AND ADJUST
	// WORD AND TEXT WRAP TO CONTEXT
	// IS HAPPENING HERE:
	//

	connect(
	ui->tblwgtTaxRates->horizontalHeader(),
	SIGNAL(sectionResized(int, int, int)),
	ui->tblwgtTaxRates,
	SLOT(resizeRowsToContents()));

	//
	// MAGIC WITH THE RESIZE AND ADJUST
	// WORD AND TEXT WRAP TO CONTEXT
	// IS HAPPENING HERE:
	//


    ui->tblwgtTaxRates->clear();
    ui->tblwgtTaxRates->setHorizontalHeaderLabels(QStringList() << "Id95896е4ш5епщ" );
    ui->tblwgtTaxRates->setColumnWidth(0, 200);
    ui->tblwgtTaxRates->setRowHeight ( 0, 120 );

//    ui->tblwgtTaxRates->resizeRowsToContents();
//    ui->tblwgtTaxRates->verticalHeader()->setDefaultSectionSize(50);
//	ui->tblwgtTaxRates->verticalHeader()->setSectionResizeMode();

    for(int i=0; i < 3; i++)
    {
        QString t = "asldfjhksa jdhfgksjhagkjhgdfgerygfjh fgkjdshjkhsdk_" + QString::number(i);

		QString s = "S DNFL SJDFLKS DLFKLSDKF s dfs djf ljsdlfk;ds kfjls dkjflksd jlfksdlf, S DNFL SJDFLKS DLFKLSDKF s dfs djf ljsdlfk;ds kfjls dkjflksd jlfksdlf, S DNFL SJDFLKS DLFKLSDKF s dfs djf ljsdlfk;ds kfjls dkjflksd jlfksdlf, S DNFL SJDFLKS DLFKLSDKF s dfs djf ljsdlfk;ds kfjls dkjflksd jlfksdlf";
		QString id;
        //id = s;
        id = t;

        int pos = (ui->tblwgtTaxRates->rowCount() );
        ui->tblwgtTaxRates->insertRow( pos );

        auto item = new QTableWidgetItem( id );
        ui->tblwgtTaxRates->setItem( pos, 0,  item);
    }

    ui->tblwgtTaxRates->model()->sort(0);
//*/
}

void MainWindow::show_my()
{
    int row = ui->tblwgtTaxRates->currentRow();
    if(row < 0) return ;

    QString taxRateId     = (ui->tblwgtTaxRates->model()->index( row, 0 )).data().toString();
    QToolTip::showText(QCursor::pos(), taxRateId);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_tblwgtTaxRates_cellActivated(int row, int column)
{
    show_my();
}

void MainWindow::on_tblwgtTaxRates_cellClicked(int row, int column)
{
    show_my();

}
