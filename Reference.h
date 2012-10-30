#ifndef REFERENCE_H
#define REFERENCE_H

#include <QMap>
#include <QStringList>
#include <QColor>

namespace BibFixer {

class Convertor;

// a reference, consisting of type, key, and multiple fields
// a field consists of field name and field value
class Reference
{
public:
    typedef QMap<QString, QString> Fields;  // fieldName, fieldValue

public:
    QString getType() const { return _type; }
    QString getKey()  const { return _key;  }
    bool isValid() const { return !_type.isEmpty(); }
    bool isEmpty() const { return !isValid() || _fields.isEmpty(); }
    void setType (const QString& type) { if(!type.isEmpty()) _type = type; }
    void setKey  (const QString& key)  { if(!key.isEmpty())  _key  = key;  }
    void addField(const QString& fieldName, const QString& fieldValue);
	void convert(const QString& fieldName, const Convertor& convertor);
    void generateKey(const QString& rule);
	QString toString() const;
    QString getFieldValue(const QString& fieldName) const;

	// for highlighting
    QStringList getChangedText() const { return _changedText; }
    void clearChangedText() { _changedText.clear(); }

private:
    QString _type;
    QString _key;
    Fields  _fields;
    QStringList _changedText;
};

// a list of reference
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

    // for hightlighting
    QStringList getChangedText() const;
    void clearChangedText();
    QColor getHighlightingColor() const { return _color; }
    void setHighlightingColor(const QColor& color) { _color = color; }

private:
    Records _records;
    QColor  _color;     // each reference list stores its color
};

}

#endif // REFERENCE_H
