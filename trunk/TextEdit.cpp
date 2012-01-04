#include "TextEdit.h"
#include <QDebug>

TextEdit::TextEdit(QWidget* parent) : QPlainTextEdit(parent) {}

void TextEdit::addHighlightedLine(const QString& line, const QColor& color) {
	highlightedLines.insert(line, color);
}

void TextEdit::highlightLines()
{
	lineSelections.clear();
	for(HighLightedLines::Iterator it = highlightedLines.begin(); it != highlightedLines.end(); ++it)
	{
		QTextCursor cursor = textCursor();
		cursor.setPosition(0);
		cursor = document()->find(it.key(), cursor);
		while(!cursor.isNull())
		{
			QTextEdit::ExtraSelection selection;
			selection.format.setBackground(it.value());
			selection.cursor = cursor;
			lineSelections.append(selection);
			cursor = document()->find(it.key(), cursor);
		}
	}
	setExtraSelections(lineSelections);
}
