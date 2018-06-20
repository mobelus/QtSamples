#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QDialog>

class Preferences : public QDialog
{
Q_OBJECT

public:
   Preferences(QWidget *parent = 0);
   ~Preferences();
};

#endif // PREFERENCES_H
