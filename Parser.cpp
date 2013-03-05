#include "Parser.h"
#include "Convertor.h"
#include "DlgSettings.h"
#include <QRegExp>

namespace BibFixer {

//////////////////////////////////////////////////////////////////////////////////
BibParser::BibParser(const QStringList& validFields)
    : _validFields(validFields) {}

ReferenceList BibParser::parse(const QString& content) const
{
	ReferenceList result;
	int idxStart = findRecordStart(content);
	int idxEnd   = findRecordEnd  (content, idxStart + 1);
	while(idxStart > -1)
	{
        QString record = content.mid(idxStart, idxEnd - idxStart + 1);  // get record
        result.addRecord(parseRecord(record));                          // parse record
        idxStart = findRecordStart(content, idxEnd);                    // next record
		idxEnd   = findRecordEnd  (content, idxStart + 1);
	}
    return result;
}

Reference BibParser::parseRecord(const QString& content) const
{
	Reference result;

	// type and id
    QRegExp rxTypeAndID("@(\\w+)\\{([^=]*),");
	int idxTypeAndID = rxTypeAndID.indexIn(content);
	if(idxTypeAndID < 0)
		return result;

	result.setType(rxTypeAndID.cap(1));
	result.setKey (rxTypeAndID.cap(2));

	// fields
    QRegExp rxField("\\s*(\\w+)\\s*=\\s*\\{([^=]+)\\}[,\\s\\r\\n]");
	int idxField = rxField.indexIn(content, idxTypeAndID + rxTypeAndID.matchedLength());
	while(idxField > -1)
	{
		QString fieldName  = rxField.cap(1).toLower();
        if(_validFields.contains(fieldName))   // is a valid field
        {
            QString fieldValue = ProtectionConvertor().undo(  // remove protective {}
                                      rxField.cap(2).simplified());
            result.addField(fieldName, fieldValue);
        }
		idxField = rxField.indexIn(content, idxField + rxField.matchedLength());  // next field
	}

    // generate key
    if(result.getKey().isEmpty())
        result.generateKey(Setting::getInstance("Rules.ini")->getKeyGenRule());

	return result;
}

int BibParser::findRecordStart(const QString& content, int startFrom) const {
    return QRegExp("@\\w+\\{").indexIn(content, startFrom);
}

int BibParser::findRecordEnd(const QString& content, int startFrom) const
{
    // if nextRecordStart == -1, lastIndexOf() returns the last '}', still correct!
	int nextRecordStart = findRecordStart(content, startFrom);
	return content.lastIndexOf("}", nextRecordStart);
}

}
