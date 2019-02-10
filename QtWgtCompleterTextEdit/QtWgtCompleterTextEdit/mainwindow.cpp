#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qwordcompleter.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QStringList keywords;
	keywords << "SELECT" << "FROM" << "WHERE" << "ORDER" << "BY" << "DELETE" << "DROP" << "SELENA" << "SERBIA"<< "SALINA"<< "SBREN";
	qSort(keywords.begin(), keywords.end());

	QWordCompleter* undercompleter = new QWordCompleter(keywords, this);
	undercompleter->setCaseSensitivity(Qt::CaseInsensitive);
	undercompleter->setMaxVisibleItems(5);
	undercompleter->setModelSorting(QCompleter::CaseSensitivelySortedModel);

	//connect(undercompleter, &QWordCompleter::activated, ui->lineEdit, &QLineEdit::setText);

	//ui->comboBox->setCompleter(compl);
	//ui->lineEdit->setCompleter(compl);

	//completer->setWidget(ui->textEdit);

	undercompleter->setWidget(ui->lineEdit);
	undercompleter->setQCompleterToLineEdit();

	//ui->lineEdit->setCompleter(completer);
}

MainWindow::~MainWindow()
{
	delete ui;
}

//void MainWindow::on_textEdit_textChanged()
//{
//
//}
