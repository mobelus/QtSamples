#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QList>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->comboBox->addItem("ru_RU");
	ui->comboBox->addItem("en_US");
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_pushButton_clicked()
{
	const double d = 1.2345;
	QMessageBox::information(this, tr("Result"), tr("The result is: %1").arg(d) );
	//QMessageBox::information(this, tr("Результат"), tr("Результат таков: %1").arg(d) );
}

void MainWindow::on_comboBox_currentTextChanged(const QString &lang)
{
	QString fileToLoad = ":/languages/QtLanguage_";
	if		(lang == "ru_RU")
	{
		MultiTran.load(fileToLoad +lang+ ".qm");
	}
	else if	(lang == "en_US")
	{
		MultiTran.load(fileToLoad +lang+ ".qm");
	}

	//setTranslation(comboBox.currentText)
	//m_translator.load(":/QmlLanguage_" + translation, "."); // Загружаем перевод
	qApp->installTranslator(&MultiTran);                 // Устанавливаем его в приложение

	retranslateUi();
}

void MainWindow::retranslateUi()
{
	ui->pushButton->setText( tr("Super Button") );
	ui->label->setText( tr("Text Label") );
	ui->pushButton->setText( tr("Super Button") );

	//QList<QAction*> lst = ui->menuBar->actions();
	foreach (QAction *action, ui->menuBar->actions())
	{
		if (action->isSeparator())
		{ qDebug("this action is a separator"); }
		else if (action->menu())
		{
			//qDebug("action: %s", qUtf8Printable(action->text()));
			//qDebug(">>> this action is associated with a submenu, iterating it recursively...");
			QString orig = action->text();
			//enumerateMenu(action->menu());
			//const QString trnlt( QObject::tr(qPrintable(orig)) );
			action->setText( QObject::tr(qPrintable(orig)) );
			//qDebug("<<< finished iterating the submenu");
		}
		else
		{ qDebug("action: %s", qUtf8Printable(action->text())); }
	}

	this->setWindowTitle( tr("MainWindow") );
}

