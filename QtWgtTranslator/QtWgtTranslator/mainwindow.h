#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTranslator>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
//*
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	//QTranslator m_translator;
	QTranslator MultiTran;
	void retranslateUi();

private slots:
	void on_pushButton_clicked();

	void on_comboBox_currentTextChanged(const QString &arg1);

private:
	Ui::MainWindow *ui;
//*/


/*
protected:
 // this event is called, when a new translator is loaded or the system language is changed
 void changeEvent(QEvent*);

protected slots:
 // this slot is called by the language menu actions
 void slotLanguageChanged(QAction* action);

private:
 // loads a language by the given language shortcur (e.g. de, en)
 void loadLanguage(const QString& rLanguage);

 // creates the language menu dynamically from the content of m_langPath
 void createLanguageMenu(void);

 //Ui::MainWindow ui; // ui definition from designer
 QTranslator m_translator; // contains the translations for this application
 QTranslator m_translatorQt; // contains the translations for qt
 QString m_currLang; // contains the currently loaded language
 QString m_langPath; // Path of language files. This is always fixed to /languages.
*/

};

#endif // MAINWINDOW_H
