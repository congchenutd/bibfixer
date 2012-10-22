#include "Reference.h"
#include "Convertor.h"
#include "EnglishName.h"
#include "DlgSettings.h"
#include <QTextStream>

//////////////////////////////////////////////////////////////////////////////
void Reference::convert(const QString& fieldName, const Convertor& convertor)
{
	if(!fields.contains(fieldName))
		return;
    QString converted = convertor.convert(fields[fieldName]);
	if(converted != fields[fieldName])
	{
		fields[fieldName] = converted;
        changedText << converted;
    }
}

void Reference::generateKey()
{
    if(!fields.contains("author"))
        return;

    QStringList authors = fields["author"].split(" and ");
    if(authors.size() < 1)
        return;

    QString firstAuthor = authors[0];
    QString lastName = EnglishName(firstAuthor).getLastName().remove(" ");

    QString year = fields.contains("year") ? fields["year"] : "0000";
    setKey(lastName + year);

    changedText << getKey();
}

QString Reference::toString() const
{
	QString result;
	QTextStream os(&result);
    os << "@" << getType() << "{" << getKey() << ",\r\n";

    QStringList keys = fields.keys();
    for(int i = 0; i < keys.size(); ++i)
    {
        os << "\t" << keys.at(i) << " = " << "{" << fields[keys.at(i)];
        if(i < keys.size() - 1)
            os << "},\r\n";
        else
            os << "}\r\n";
    }

	os << "}";
	return result;
}

void Reference::addField(const QString& fieldName, const QString& value)
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
	foreach(const Reference& record, records)
		result += record.toString() + "\r\n\r\n";
	return result;
}

QStringList ReferenceList::getChangedText() const
{
	QStringList result;
	foreach(const Reference& record, records)
		result << record.getChangedText();
	return result;
}

void ReferenceList::clearChangedText() {
    for(Records::Iterator it = records.begin(); it != records.end(); ++ it)
        it->clearChangedText();
}

void ReferenceList::addRecord(const Reference& record) {
    if(!record.isEmpty())
        records.insert(record.getKey(), record);
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
    convertor.setRules(Setting::getInstance("Rules.ini")->getSelectedAbbreviationRules());
	for(Records::Iterator it = records.begin(); it != records.end(); ++ it)
        it->convert(fieldName, convertor);
}

void ReferenceList::generateKeys()
{
    for(Records::Iterator it = records.begin(); it != records.end();)
    {
        Reference ref = it.value();
        ref.generateKey();
        it = records.erase(it);      // re-insert the record because its key is changed
        records.insert(ref.getKey(), ref);
    }
}
