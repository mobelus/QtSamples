#ifndef QWORDCOMPLETER_H
#define QWORDCOMPLETER_H

#include <QCompleter>
#include <QTextEdit>
#include <QLineEdit>

class QWordCompleter : public QCompleter
{
public:
	QWordCompleter(const QStringList& completions, QObject *parent = Q_NULLPTR);
	~QWordCompleter();

	//
public:
	virtual bool eventFilter(QObject *o, QEvent *e);

	bool isQCompleterSetToLineEdit() { return m_isQCompleterSetToLineEdit; }
	void setIsQCompleterSetToLineEdit(bool& b) { m_isQCompleterSetToLineEdit = b; }

	int getMinCompletionLenght()	const;
	void setMinCompletionLenght(int value);


	bool setQCompleterToLineEdit();
	QCompleter* getQCompleterFromLineEdit() { return m_isQCompleterSetToLineEdit ? completerForLineEdit : nullptr; }

private:
	int minCompletionLenght;

	double nRows;
	QTextEdit* textEdit;

	QCompleter* completerForLineEdit;
	bool m_isQCompleterSetToLineEdit;
};

#endif // QWORDCOMPLETER_H
