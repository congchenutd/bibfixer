#include "Reference.h"
#include "Convertor.h"
#include "EnglishName.h"
#include "DlgSettings.h"
#include "KeyGenerator.h"
#include <QTextStream>

namespace BibFixer {

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

void Reference::generateKey(const QString& rule)
{
    if(rule.isEmpty())
        return;

    QString key = KeyGenerator(this).generate(rule);
    if(!key.isEmpty())
    {
        setKey(key);
        changedText << key;
    }
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

QString Reference::getFieldValue(const QString& fieldName) const {
    return fields.contains(fieldName) ? fields[fieldName] : QString();
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
    Convertor* convertor;
    if(Setting::getInstance()->getProtectFirstLetter())
        convertor = new FirstLetterProtectionConvertor;
    else
        convertor = new AllProtectionConvertor;
	for(Records::Iterator it = records.begin(); it != records.end(); ++ it)
        it->convert(fieldName, *convertor);
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
    QString rule = Setting::getInstance("Rules.ini")->getKeyGenRule();
    for(Records::Iterator it = records.begin(); it != records.end();)
    {
        Reference ref = it.value();
        ref.generateKey(rule);
        it = records.erase(it);      // re-insert the record because its key is changed
        records.insert(ref.getKey(), ref);
    }
}

}
