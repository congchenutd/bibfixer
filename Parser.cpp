#include "Parser.h"
#include "DlgSettings.h"
#include <QRegExp>

/////////////////////////////////////////////////////////////////////////////
ReferenceList BibParser::parse(const QString& content) const
{
	ReferenceList result;
	int idxStart = findRecordStart(content);
	int idxEnd   = findRecordEnd  (content, idxStart + 1);
	while(idxStart > -1)
	{
		QString record = content.mid(idxStart, idxEnd - idxStart + 1);
		result.addRecord(parseRecord(record));
		idxStart = findRecordStart(content, idxEnd);
		idxEnd   = findRecordEnd  (content, idxStart + 1);
	}
	return result;
}

ReferenceRecord BibParser::parseRecord(const QString& content) const
{
	ReferenceRecord result;

	// type and id
	QRegExp rxTypeAndID("@(\\w+)\\{([^=]+),");
	int idxTypeAndID = rxTypeAndID.indexIn(content);
	if(idxTypeAndID > -1)
	{
		result.setType(rxTypeAndID.cap(1));
        result.setKey (rxTypeAndID.cap(2));
	}

	// fields
	QStringList validFields = UserSetting::getInstance()->getFields();
	QRegExp rxField("\\s*(\\w+)\\s*=\\s*\\{([^=]+)\\},?\\s*\\n\\s*");
	int idxField = rxField.indexIn(content, idxTypeAndID + rxTypeAndID.matchedLength());
	while(idxField > -1)
	{
		QString fieldName = rxField.cap(1).toLower();
		if(validFields.contains(fieldName))
			result.addField(fieldName, rxField.cap(2).simplified());
		idxField = rxField.indexIn(content, idxField + rxField.matchedLength());
	}

	return result;
}

int BibParser::findRecordStart(const QString& content, int startFrom) const
{
	QRegExp expRecordStart("@\\w+\\{");
	return content.indexOf(expRecordStart, startFrom);
}

int BibParser::findRecordEnd(const QString& content, int startFrom) const
{
	// if nextRecordStart == -1, lastIndexOf returns the last '}', correct!
	int nextRecordStart = findRecordStart(content, startFrom);
	return content.lastIndexOf("}", nextRecordStart);
}
