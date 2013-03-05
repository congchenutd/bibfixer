#include "Reference.h"
#include "Convertor.h"
#include "EnglishName.h"
#include "KeyGenerator.h"
#include <QTextStream>

namespace BibFixer {

/////////////////////////////////////////////////////////////////////////////////
void Reference::convert(const QString& fieldName, const IConvertor* convertor)
{
    if(!_fields.contains(fieldName))
		return;
    QString converted = convertor->redo(_fields[fieldName]);
    if(converted != _fields[fieldName])
	{
        _fields[fieldName] = converted;
        _changedText << converted;
    }
}

void Reference::generateKey(const QString& rule)
{
    if(rule.isEmpty())
        return;

    QString key = KeyGenerator(rule).generateKey(*this);
    if(!key.isEmpty())
    {
        setKey(key);
        _changedText << key;
    }
}

QString Reference::toString() const
{
    if(!isValid())
        return QObject::tr("Invalid reference");

	QString result;
	QTextStream os(&result);
    os << "@" << getType() << "{" << getKey() << ",\r\n";

    QStringList keyList = _fields.keys();
    for(int i = 0; i < keyList.size(); ++i)
    {
        os << "\t" << keyList.at(i) << " = " << "{" << _fields[keyList.at(i)];
        if(i < keyList.size() - 1)
            os << "},\r\n";
        else
            os << "}\r\n";   // the last field doesn't end with ,
    }

	os << "}";
    return result;
}

QString Reference::getFieldValue(const QString& fieldName) const {
    return _fields.contains(fieldName) ? _fields[fieldName] : QString();
}

void Reference::addField(const QString& fieldName, const QString& value)
{
	// correct wrong page delimiters
	QString fieldValue = value;
	if(fieldName == "pages")
	{
        QRegExp rxPages("(\\d+)(\\D+)?(\\d+)?");  // number something else number
		if(rxPages.indexIn(fieldValue) > -1)
		{
			QString startPage = rxPages.cap(1);
			QString endPage   = rxPages.cap(3);
			fieldValue = startPage;
            if(!endPage.isEmpty())                // may only have a start page
				fieldValue += "-" + endPage;
		}
	}
    _fields.insert(fieldName, fieldValue);
}


//////////////////////////////////////////////////////////////////////////////////
QString ReferenceList::toString() const
{
	QString result;
    foreach(const Reference& record, _records)
		result += record.toString() + "\r\n\r\n";
	return result;
}

QStringList ReferenceList::getChangedText() const
{
	QStringList result;
    foreach(const Reference& record, _records)
		result << record.getChangedText();
	return result;
}

void ReferenceList::clearChangedText() {
    for(Records::Iterator it = _records.begin(); it != _records.end(); ++ it)
        it->clearChangedText();
}

void ReferenceList::addRecord(const Reference& record) {
    if(!record.isEmpty())
        _records.insert(record.getKey(), record);
}

void ReferenceList::clear() {
    _records.clear();
}

void ReferenceList::convertField(const IConvertor* convertor, const QString& fieldName) {
    for(Records::Iterator it = _records.begin(); it != _records.end(); ++ it)
        it->convert(fieldName, convertor);
}

void ReferenceList::generateKeys(const QString& rule)
{
    for(Records::Iterator it = _records.begin(); it != _records.end();)
    {
        Reference ref = it.value();
        ref.generateKey(rule);
        it = _records.erase(it);      // re-insert the record because its key is changed
        _records.insert(ref.getKey(), ref);
    }
}

}  // namespace BibFixer
