#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    configPath = "C:/Users/Public/QtProjects/QtWdgtIniFile/QtWdgtIniFile/Configuration.ini";

//    QSettings * qsettings = new QSettings(":/config.ini",QSettings::IniFormat);
//    bool status = qsettings->value("preview","").toBool();
//    qDebug() << status;

    /*
    QSettings settings;
    settings.setValue("ape");
    settings.setValue("monkey", 1);
    settings.setValue("monkey/sea", 2);
    settings.setValue("monkey/doe", 4);

    settings.beginGroup("monkey");
    settings.remove("");
    settings.endGroup();

    QStringList keys = settings.allKeys();
    // keys: ["ape"]
    */


    //writeSettings();
    //readSettings();

    saveSettings();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
void MainWindow::getSettings()
{
    QFile checkConfig(configPath);
    if(checkConfig.exists())
    {
        qsettings = new QSettings(configPath, QSettings::IniFormat);
        bool status = qsettings->value("preview","").toBool();
    }
}

void MainWindow::setSettings()
{
    QFile checkConfig(configPath);
    if(checkConfig.exists())
    {
        qsettings = new QSettings(configPath, QSettings::IniFormat);
        qsettings.beginGroup("MainWindow");
        qsettings.setValue("size", size());
        qsettings.setValue("pos", pos());
        qsettings.endGroup();

    }

}
//*/

/*
settings->beginGroup("General");
const QStringList childKeys = settings->childKeys();
QStringList values;
foreach (const QString &childKey, childKeys)
    values << settings->value(childKey).toString();
settings->endGroup();
QString test = configPath;
int a;
*/



//QSettings settings(":/config.ini", QSettings::IniFormat);


void MainWindow::on_pushButton_clicked()
{
    loadSettings();
}


void MainWindow::saveSettings()
{
    QSettings* qsettings = new QSettings(configPath, QSettings::IniFormat);

    qsettings->beginGroup("MainWindow");
    qsettings->setValue("size", size());
    qsettings->setValue("pos", pos());
    qsettings->endGroup();
}

void MainWindow::loadSettings()
{
    QSettings* qsettings = new QSettings(configPath, QSettings::IniFormat);

    qsettings->beginGroup("MainWindow");
    resize(qsettings->value("size", QSize(400, 400)).toSize());
    move(qsettings->value("pos", QPoint(200, 200)).toPoint());
    qsettings->endGroup();
}

