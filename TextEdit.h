#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QPlainTextEdit>
#include <QMap>

namespace BibFixer {

// A text edit that can highlight certain texts
class TextEdit : public QPlainTextEdit
{
	Q_OBJECT

    typedef QMap<QString, QColor> HighLightedText;  // text, bg color

public:
	TextEdit(QWidget* parent = 0);
    void addHighlighting(const QString& text, const QColor& bgColor);
    void highlight();
    void unHighlight();

protected:
    void insertFromMimeData(const QMimeData* source);  // copy-paste

signals:
    void pasted();

private:
    HighLightedText _highlightedTexts;
};

}

#endif // TEXTEDIT_H
