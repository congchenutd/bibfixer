#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>
#include <QMap>

// A text edit that can highlight certain texts
class TextEdit : public QPlainTextEdit
{
	Q_OBJECT

    typedef QMap<QString, QColor> HighLightedText;

public:
	TextEdit(QWidget* parent = 0);
    void addHighlightedText(const QString& text, const QColor& color);
    void highlight();
    void unHighlight();

protected:
    void insertFromMimeData(const QMimeData* source);  // copy-paste

signals:
    void pasted();

private:
    HighLightedText highlightedTexts;
};

#endif // TEXTEDIT_H
