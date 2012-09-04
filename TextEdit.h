#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>
#include <QMap>

// A text edit that can highlight certain lines
class TextEdit : public QPlainTextEdit
{
	Q_OBJECT

	typedef QMap<QString, QColor> HighLightedLines;

public:
	TextEdit(QWidget* parent = 0);
	void addHighlightedLine(const QString& line, const QColor& color);
	void highlightLines();
    void unHighlightLines();

protected:
    void insertFromMimeData(const QMimeData* source);

signals:
    void pasted();

private:
	HighLightedLines highlightedLines;
};

#endif // TEXTEDIT_H
