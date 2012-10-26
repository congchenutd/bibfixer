#ifndef REFERENCE_H
#define REFERENCE_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <QColor>

namespace BibFixer {

class Convertor;
class Reference
{
public:
	typedef QMap<QString, QString> Fields;

public:
	QString getType() const { return type; }
	QString getKey()  const { return key;  }
	bool isValid() const { return !type.isEmpty(); }
	bool isEmpty() const { return !isValid() || fields.isEmpty(); }
    void setType (const QString& t) { if(!t.isEmpty()) type = t; }
    void setKey  (const QString& k) { if(!k.isEmpty()) key  = k; }
    void addField(const QString& fieldName, const QString& fieldValue);
	void convert(const QString& fieldName, const Convertor& convertor);
    void generateKey(const QString& rule);
	QString toString() const;
    QString getFieldValue(const QString& fieldName) const;

	// for highlighting
    QStringList getChangedText() const { return changedText; }
    void clearChangedText() { changedText.clear(); }

private:
	QString type;
    QString key;
	Fields  fields;
    QStringList changedText;
};

class ReferenceList
{
public:
    typedef QMap<QString, Reference> Records;

public:
    void clear();
    void addRecord(const Reference& record);
	void capitalize(const QString& fieldName);
	void protect   (const QString& fieldName);
	void abbreviate(const QString& fieldName);
    void generateKeys();
	QString toString() const;

    QStringList getChangedText() const;   // for hightlighting
    void clearChangedText();
    QColor getHighlightingColor() const { return color; }
    void setHighlightingColor(const QColor& highlightingColor) { color = highlightingColor; }

private:
    Records records;
	QColor  color;     // each snapshot stores its color
};

}

#endif // REFERENCE_H
