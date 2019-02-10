#include "qwordcompleter.h"

#include <QEvent>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QTextEdit>
#include <QTextCursor>

QWordCompleter::QWordCompleter(const QStringList& completions, QObject *parent) :
	QCompleter(completions, parent)
	, textEdit( new QTextEdit)
	, minCompletionLenght(0)
	, nRows(2)
{
	/*
	QFontMetrics m(textEdit->font());
	double RowHeight = m.lineSpacing();
	//edit->setFixedHeight(nRows * (RowHeight * 1.5)) ;
	textEdit->setFixedHeight(nRows * (RowHeight * 1.4));
	*/

	textEdit->setEnabled(false);
}

QWordCompleter::~QWordCompleter()
{

}


bool QWordCompleter::setQCompleterToLineEdit()
{
	if(!isQCompleterSetToLineEdit())
	{
		QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget());

		//completer = new QCompleter(keywords, this);
		QStringList lst;
		//this->model()->rowCount();
		for (int i = 0; this->setCurrentRow(i); i++)
			lst.append(this->currentCompletion());

		completerForLineEdit = new QCompleter(lst, this /*this->parent()*/);
		completerForLineEdit->setCaseSensitivity( this->caseSensitivity() );
		completerForLineEdit->setMaxVisibleItems( this->maxVisibleItems() );
		completerForLineEdit->setModelSorting( this->modelSorting() );

		lineEdit->setCompleter(completerForLineEdit);
	}
}


bool QWordCompleter::eventFilter(QObject *o, QEvent *e)
{
/*	// ALMOST WORKING VERSION
	if(widget()->inherits("QTextEdit") && e->type() == QEvent::KeyPress)
	{
		QKeyEvent* kEv = static_cast<QKeyEvent*> (e);
		switch(kEv->key())
		{
		//	case Qt::Key_Space:
		//	break;
		case Qt::Key_Enter:
		case Qt::Key_Return:
		case Qt::Key_Tab:
			if(popup()->isVisible())
			{
				popup()->hide();
				if(popup()->currentIndex().isValid())
				{
					emit activated(popup()->currentIndex());
					emit activated(popup()->currentIndex().data(completionRole()).toString());
				}
				return true;
			}
		default:
			//if(kEv->modifiers().testFlag(Qt::ControlModifier))
			{
				// от начала слова, до курсора
				QTextEdit* textEdit = qobject_cast<QTextEdit*>(widget());
				QTextCursor textCursor = textEdit->textCursor();
				textCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
				if(textCursor.selectedText().length() >= minCompletionLenght)
				{
					setCompletionPrefix(textCursor.selectedText()); // На 5 пикселей ниже курсора
					QRect rect  = QRect(textEdit->cursorRect().bottomLeft(), QSize(100, 5));
					complete(rect);
				}
				//return true;
				return QCompleter::eventFilter(o, e);
			}
		}
	}
//*/


//* // CTRL + SPACE - VERSION // QTextEdit

	if(widget()->inherits("QTextEdit") && e->type() == QEvent::KeyPress)
	{
		bool isEditHidden = false;
		QKeyEvent* kEv = static_cast<QKeyEvent*> (e);
		switch(kEv->key())
		{
			case Qt::Key_Space:
				if(kEv->modifiers().testFlag(Qt::ControlModifier))
				{
					// от начала слова, до курсора
					QTextEdit* textEdit = qobject_cast<QTextEdit*>(widget());
					QTextCursor textCursor = textEdit->textCursor();
					textCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
					if(textCursor.selectedText().length() >= minCompletionLenght)
					{
						setCompletionPrefix(textCursor.selectedText()); // На 5 пикселей ниже курсора
						QRect rect  = QRect(textEdit->cursorRect().bottomLeft(), QSize(100, 5));
						complete(rect);
					}
					return true;
				}
			break;
		case Qt::Key_Enter:
		case Qt::Key_Return:
		case Qt::Key_Tab:
			if(popup()->isVisible())
			{
				popup()->hide();
				if(popup()->currentIndex().isValid())
				{
					emit activated(popup()->currentIndex());
					emit activated(popup()->currentIndex().data(completionRole()).toString());
				}
				return true;
			}
		default:
		break;
		}
	}
//*/

	// CTRL + SPACE - VERSION // QLineEdit
	if(widget()->inherits("QLineEdit") && e->type() == QEvent::KeyPress)
	{
		QKeyEvent* kEv = static_cast<QKeyEvent*> (e);
		switch(kEv->key())
		{
			//case Qt::Key_Space:
			case Qt::Key_Down:
				//if(kEv->modifiers().testFlag(Qt::ControlModifier))
				{
					// от начала слова, до курсора
					/*
					QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget());
					QTextCursor textCursor = lineEdit->textCursor();
					textCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
					if(textCursor.selectedText().length() >= minCompletionLenght)
					{
						setCompletionPrefix(textCursor.selectedText()); // На 5 пикселей ниже курсора
						QRect rect  = QRect(lineEdit->cursorRect().bottomLeft(), QSize(100, 5));
						complete(rect);
					}
					return true;
					*/

					//isEditHidden = textEdit->isHidden();
					isEditHidden = textEdit->isEnabled();
					if(!isEditHidden)
					{
					//
						QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget());
						QTextCursor textCursor = textEdit->textCursor();
						textCursor.movePosition(QTextCursor::StartOfWord, QTextCursor::KeepAnchor);
						if(textCursor.selectedText().length() >= minCompletionLenght)
						{
							setCompletionPrefix(textCursor.selectedText()); // На 5 пикселей ниже курсора
							QPoint pointUnderLeft = textEdit->cursorRect().bottomLeft();
							pointUnderLeft.setY( pointUnderLeft.y() + 8 );

							QSize lineEditSize = QSize(lineEdit->width(), 5); //QSize(100, 5);
							QRect rect  = QRect( pointUnderLeft, lineEditSize );
							complete(rect);
							//textEdit->setGeometry(textEdit->x(), textEdit->y(), textEdit->width(), 50);
							textEdit->setEnabled(true);
						}
						return true;
					}
					else
					{
						return QCompleter::eventFilter(o, e);
					}
				}
			break;
		case Qt::Key_Up:
			break;

		case Qt::Key_Enter:
		case Qt::Key_Return:
		//case Qt::Key_Tab:
			if(popup()->isVisible())
			{
				popup()->hide();
				if(popup()->currentIndex().isValid())
				{
					emit activated(popup()->currentIndex());
					emit activated(popup()->currentIndex().data(completionRole()).toString());

					QLineEdit* lineEdit = qobject_cast<QLineEdit*>(widget());
					lineEdit->setText(popup()->currentIndex().data(completionRole()).toString());
				}
				textEdit->setEnabled(false);
				return true;
			}
		default:
			textEdit->setEnabled(false);
			if(popup()->isVisible())
			{
				popup()->hide();
				//return true;
			}
		break;
		}
	}

	return QCompleter::eventFilter(o, e);
}

int QWordCompleter::getMinCompletionLenght() const
{
	return minCompletionLenght;
}

void QWordCompleter::setMinCompletionLenght(int value)
{
	minCompletionLenght = value;
}

