#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>
#include <QMap>

class TextEdit : public QPlainTextEdit
{
	Q_OBJECT

	typedef QMap<QString, QColor> HighLightedLines;
	typedef QList<QTextEdit::ExtraSelection> Selections;

public:
	TextEdit(QWidget* parent = 0);
	void addHighlightedLine(const QString& line, const QColor& color);
	void highlightLines();

private:
	Selections       lineSelections;
	HighLightedLines highlightedLines;
};

#endif // TEXTEDIT_H
