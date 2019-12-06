#ifndef CUSTOMCOMBOBOX_H
#define CUSTOMCOMBOBOX_H

#include <QObject>
#include <QComboBox>
#include <QWidget>


#include <QStylePainter>
#include <QStylePainter>

class CustomComboBox : public QComboBox
{
    Q_OBJECT
public:
    //explicit CustomComboBox(QObject *parent = nullptr);
    explicit CustomComboBox(QWidget *parent = nullptr);

    void paintEvent(QPaintEvent * /*event*/);

signals:

public slots:
};

#endif // CUSTOMCOMBOBOX_H
