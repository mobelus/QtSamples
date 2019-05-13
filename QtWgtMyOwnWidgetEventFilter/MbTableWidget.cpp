#include "MbTableWidget.h"


bool MbTableWidget::eventFilter(QObject *obj, QEvent *event)
{
	//if(obj->v != 0)
	if (obj != nullptr)
	{
		if (event != nullptr)
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			if (keyEvent->key() == Qt::Key_Up)
				qDebug() << "Up";
			if (keyEvent->key() == Qt::Key_Down)
				qDebug() << "Down";
			if (keyEvent->key() == Qt::Key_Enter) // на доп. клавиатуре
				qDebug() << "Key_Enter";
			if (keyEvent->key() == Qt::Key_Return) // над Shift-ом
			{
				//_mainModbusServerEngine.setEnterPressed(true);
				emit EnterWasPressed(true);
				qDebug() << "Key_Return";
			}
		}
	}

	return QObject::eventFilter(obj, event);
}


