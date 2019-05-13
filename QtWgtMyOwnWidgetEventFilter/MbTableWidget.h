#ifndef MB_TABLE_WIDGET_H
#define MB_TABLE_WIDGET_H

#include <QDebug>
#include <QKeyEvent>
#include <QWidget>
#include <QTableWidget>

//class MbTableWidget : public QWidget // QTableWidget
class MbTableWidget : public QTableWidget
{
	Q_OBJECT
public:
	explicit MbTableWidget(QWidget *parent = nullptr)
	{
		installEventFilter(this);
	}
	~MbTableWidget()
	{ }

protected:
	bool eventFilter(QObject *target, QEvent *event);

signals:
	void EnterWasPressed(bool b);

};


#endif // MB_TABLE_WIDGET_H
