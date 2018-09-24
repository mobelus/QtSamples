#include "mainwindow.h"
#include <QApplication>

//
// 1. Add to pro-file: TRANSLATIONS = languages/TranslationExample_en.ts  languages/TranslationExample_de.ts
// 2. In menu of Qt Creator: Instruments -> Outsiders -> Lunguist -> Update translations (lupdate)
// Result: We get our *.ts files
// 3. Edit them inside QtLinguist
//
// DOES NOT WORK WITHOUT ENVORIMENT VSRIABLES
//
// 1 step:
// go with cmd -> to projects path, where the .pro-file is
// type in:
// lupdate -pro AppName.pro -ts russian.ts
// QtWgtTranslator

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setStyle("fusion");

	MainWindow w;
	w.show();

	return a.exec();
}
