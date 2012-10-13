#include "TextEdit.h"
#include <QDebug>

TextEdit::TextEdit(QWidget* parent) : QPlainTextEdit(parent) {}

void TextEdit::addHighlightedText(const QString& text, const QColor& color) {
    highlightedTexts.insert(text, color);
}

void TextEdit::highlight()
{
    QList<QTextEdit::ExtraSelection> selections;
	for(HighLightedText::Iterator it = highlightedTexts.begin(); it != highlightedTexts.end(); ++it)
	{
		QTextCursor cursor = textCursor();
		cursor.setPosition(0);
		cursor = document()->find(it.key(), cursor);
		while(!cursor.isNull())
		{
			QTextEdit::ExtraSelection selection;
			selection.format.setBackground(it.value());
			selection.cursor = cursor;
            selections << selection;
			cursor = document()->find(it.key(), cursor);
		}
	}
    setExtraSelections(selections);
}

void TextEdit::unHighlight()
{
    highlightedTexts.clear();
    highlight();
}

void TextEdit::insertFromMimeData(const QMimeData* source)
{
    QPlainTextEdit::insertFromMimeData(source);
    emit pasted();
}
