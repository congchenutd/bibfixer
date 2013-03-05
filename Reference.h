#ifndef REFERENCE_H
#define REFERENCE_H

#include <QMap>
#include <QStringList>
#include <QColor>

namespace BibFixer {

class IConvertor;

// a reference, consisting of type, key, and multiple fields
// a field consists of field name and field value
class Reference
{
public:
    QString getType() const { return _type; }
    QString getKey()  const { return _key;  }
    bool isValid()    const { return !_type.isEmpty(); }
    bool isEmpty()    const { return !isValid() || _fields.isEmpty(); }
    void setType (const QString& type) { if(!type.isEmpty()) _type = type; }
    void setKey  (const QString& key)  { if(!key.isEmpty())  _key  = key;  }
    void addField(const QString& fieldName, const QString& fieldValue);
    void convert(const QString& fieldName, const IConvertor* convertor); // convert a field
    void generateKey(const QString& rule);
	QString toString() const;
    QString getFieldValue(const QString& fieldName) const;

	// for highlighting
    QStringList getChangedText() const { return _changedText; }
    void clearChangedText() { _changedText.clear(); }

private:
    QString _type;
    QString _key;
    QMap<QString, QString> _fields;  // fieldName->fieldValue
    QStringList _changedText;        // for highlighting
};

// a list of reference
class ReferenceList
{
private:
    typedef QMap<QString, Reference> Records;   // key->reference

public:
    void clear();
    void addRecord(const Reference& record);
    void convertField(const IConvertor* convertor, const QString& fieldName);
    void generateKeys(const QString& rule);  // different than other conversion: cannot be done in-place
	QString toString() const;

    // for hightlighting
    QStringList getChangedText() const;
    void clearChangedText();

private:
    Records _records;
};

}

#endif // REFERENCE_H
