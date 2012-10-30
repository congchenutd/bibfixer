#include "TextEdit.h"

namespace BibFixer {

TextEdit::TextEdit(QWidget* parent) : QPlainTextEdit(parent) {}

void TextEdit::addHighlighting(const QString& text, const QColor& bgColor) {
    _highlightedTexts.insert(text, bgColor);
}

void TextEdit::highlight()
{
    QList<QTextEdit::ExtraSelection> selections;
	for(HighLightedText::Iterator it = _highlightedTexts.begin(); it != _highlightedTexts.end(); ++it)
	{
        QString text  = it.key();
        QColor  color = it.value();

        // select all occurances of text and highlight them with color
		QTextCursor cursor = textCursor();
		cursor.setPosition(0);
        cursor = document()->find(text, cursor);
		while(!cursor.isNull())
		{
			QTextEdit::ExtraSelection selection;
            selection.format.setBackground(color);
			selection.cursor = cursor;
            selections << selection;
            cursor = document()->find(text, cursor);   // next occurance
		}
	}
    setExtraSelections(selections);
}

void TextEdit::unHighlight()
{
    _highlightedTexts.clear();
    highlight();
}

void TextEdit::insertFromMimeData(const QMimeData* source)
{
    QPlainTextEdit::insertFromMimeData(source);
    emit pasted();
}

}
