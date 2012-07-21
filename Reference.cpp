#include "Reference.h"
#include "Convertor.h"
#include <QTextStream>

//////////////////////////////////////////////////////////////////////////////
void ReferenceRecord::convert(const QString& fieldName, const Convertor& convertor)
{
	if(!fields.contains(fieldName))
		return;
    QString converted = convertor.convert(fields[fieldName]);
	if(converted != fields[fieldName])
	{
		fields[fieldName] = converted;
		changedValues << fields[fieldName];
    }
}

bool ReferenceRecord::convertible(const QString& fieldName, const Convertor &convertor) const {
    return fields.contains(fieldName) && convertor.convertible(fields[fieldName]);
}

QString ReferenceRecord::toString() const
{
	QString result;
	QTextStream os(&result);
	os << "@" << type << "{" << id << ",\r\n";
	for(Fields::const_iterator it = fields.constBegin(); it != fields.constEnd(); ++it)
		os << "\t" << it.key() << " = " << "{" << it.value() << "},\r\n";
	os << "}";
	return result;
}

void ReferenceRecord::addField(const QString& fieldName, const QString& value)
{
	// correct wrong page delimiters
	QString fieldValue = value;
	if(fieldName == "pages")
	{
		QRegExp rxPages("(\\d+)(\\D+)?(\\d+)?");
		if(rxPages.indexIn(fieldValue) > -1)
		{
			QString startPage = rxPages.cap(1);
			QString endPage   = rxPages.cap(3);
			fieldValue = startPage;
			if(!endPage.isEmpty())
				fieldValue += "-" + endPage;
		}
	}
	fields.insert(fieldName, fieldValue);
}


//////////////////////////////////////////////////////////////////////////////////
QString ReferenceList::toString() const
{
	QString result;
	foreach(const ReferenceRecord& record, records)
		result += record.toString() + "\r\n\r\n";
	return result;
}

QStringList ReferenceList::getChangedValues() const
{
	QStringList result;
	foreach(const ReferenceRecord& record, records)
		result << record.getChangedValues();
	return result;
}

void ReferenceList::clearChangedValues() {
	for(Records::Iterator it = records.begin(); it != records.end(); ++ it)
		it->clearChangedValues();
}

void ReferenceList::addRecord(const ReferenceRecord& record) {
	records.insert(record.getID(), record);
}

void ReferenceList::clear() {
	records.clear();
}

void ReferenceList::capitalize(const QString& fieldName)
{
    CaseConvertor convertor;
	for(Records::Iterator it = records.begin(); it != records.end(); ++ it)
        it->convert(fieldName, convertor);
}

void ReferenceList::protect(const QString& fieldName)
{
    ProtectionConvertor convertor;
	for(Records::Iterator it = records.begin(); it != records.end(); ++ it)
        it->convert(fieldName, convertor);
}

void ReferenceList::abbreviate(const QString& fieldName)
{
    AbbreviationConvertor convertor;
	for(Records::Iterator it = records.begin(); it != records.end(); ++ it)
        it->convert(fieldName, convertor);
}

bool ReferenceList::canCapitalize(const QString& fieldName) const
{
    CaseConvertor convertor;
    for(Records::Iterator it = records.begin(); it != records.end(); ++ it)
        if(it->convertible(fieldName, convertor))
            return true;
    return false;
}

