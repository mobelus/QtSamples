#include "CustomComboBox.h"

//CustomComboBox::CustomComboBox(QObject *parent) : QObject(parent)
CustomComboBox::CustomComboBox(QWidget *parent) : QComboBox (parent)
{

}


void CustomComboBox::paintEvent(QPaintEvent * /*event*/)
{
    QStyleOptionComboBox opt;
    initStyleOption(&opt);

    QStylePainter p(this);
    p.drawComplexControl(QStyle::CC_ComboBox, opt);

    QRect textRect = style()->subControlRect(QStyle::CC_ComboBox, &opt, QStyle::SC_ComboBoxEditField, this);
    opt.currentText = p.fontMetrics().elidedText(opt.currentText, Qt::ElideRight, textRect.width());
    p.drawControl(QStyle::CE_ComboBoxLabel, opt);
}

