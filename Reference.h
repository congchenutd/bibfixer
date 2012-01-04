#ifndef REFERENCE_H
#define REFERENCE_H

#include <QMap>
#include <QString>
#include <QStringList>

class Convertor;
class ReferenceRecord
{
public:
	typedef QMap<QString, QString> Fields;

public:
	void setType(const QString& t) { type = t; }
	void setID  (const QString& i) { id   = i; }
	QString getType() const { return type; }
	QString getID()   const { return id;   }
	void addField(const QString& fieldName, const QString& fieldValue);

	void convert(const QString& fieldName, Convertor* convertor);
	QString toString() const;

	// for highlighting
	QStringList getChangedValues() const { return changedValues; }
	void clearChangedValues() { changedValues.clear(); }

private:
	QString type;
	QString id;
	Fields  fields;
	QStringList changedValues;
};

class ReferenceList
{
public:
	typedef QMap<QString, ReferenceRecord> Records;

public:
	void addRecord(const ReferenceRecord& record);
	void clear();
	void capitalize(const QString& fieldName);
	void protect   (const QString& fieldName);
	void abbreviate(const QString& fieldName);
	QString toString() const;
	QStringList getChangedValues() const;   // for hightlighting
	void clearChangedValues();

private:
	Records records;
};


#endif // REFERENCE_H
