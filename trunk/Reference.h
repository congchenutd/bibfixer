#ifndef REFERENCE_H
#define REFERENCE_H

#include <QMap>
#include <QString>
#include <QStringList>
#include <QColor>

class Convertor;
class ReferenceRecord
{
public:
	typedef QMap<QString, QString> Fields;

public:
    void setType (const QString& t) { type = t; }
    void setKey  (const QString& k) { key  = k; }
	QString getType() const { return type; }
    QString getKey()  const { return key;  }
	void addField(const QString& fieldName, const QString& fieldValue);

	void convert(const QString& fieldName, const Convertor& convertor);
    void generateKey();
	QString toString() const;

	// for highlighting
	QStringList getChangedValues() const { return changedValues; }
	void clearChangedValues() { changedValues.clear(); }

private:
	QString type;
    QString key;
	Fields  fields;
	QStringList changedValues;
};

class ReferenceList
{
public:
	typedef QMap<QString, ReferenceRecord> Records;

public:
    void clear();
	void addRecord(const ReferenceRecord& record);
	void capitalize(const QString& fieldName);
	void protect   (const QString& fieldName);
	void abbreviate(const QString& fieldName);
    void generateKeys();

	QString toString() const;

	QStringList getChangedValues() const;   // for hightlighting
	void clearChangedValues();
    QColor getHighlightColor() const { return highlightColor; }
    void setHighlightColor(const QColor& clr) { highlightColor = clr; }

private:
	Records records;
    QColor  highlightColor;
};


#endif // REFERENCE_H
