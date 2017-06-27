#include "Parser.h"
#include "Convertor.h"
#include "DlgSettings.h"
#include <QRegExp>
#include <QRegularExpression>

namespace BibFixer {

IParser::~IParser() {}

//////////////////////////////////////////////////////////////////////////////////

BibParser* BibParser::_instance = 0;

BibParser* BibParser::getInstance()
{
    if (_instance == 0)
        _instance = new BibParser;
    return _instance;
}

void BibParser::setValidFields(const QStringList& validFields) {
    _validFields = validFields;
}

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

ReferenceList BibParser::import(const QString& latexFilePath) const
{
    ReferenceList result;
    QFile latexFile(latexFilePath);
    if (!latexFile.open(QFile::ReadOnly))
        return result;

    QString latexFileContent = latexFile.readAll();
    QRegularExpression rxReference("\\\\bibliography{(?<bibtex>\\w+)}");
    auto match = rxReference.match(latexFileContent);
    if (!match.hasMatch())
        return result;

    QSet<QString> keys;
    QRegularExpression rxCite("\\\\cite{(\\w+(,\\s*\\w+)*)}");
    QRegularExpressionMatchIterator it = rxCite.globalMatch(latexFileContent);
    while (it.hasNext()) {
        auto match = it.next();
        auto captured = match.captured(1).remove(' ');
        foreach (auto key, captured.split(',', QString::SkipEmptyParts))
            keys.insert(key);
    }
    if (keys.isEmpty())
        return result;

    auto sortedKeys = keys.toList();
    sortedKeys.sort(Qt::CaseInsensitive);

    auto bibtexFileName = match.captured("bibtex");
    auto bibtexFilePath = QFileInfo(latexFilePath).path() + QDir::separator() + bibtexFileName;
    if (!bibtexFileName.endsWith(".bib", Qt::CaseInsensitive))
        bibtexFilePath += ".bib";
    QFile bibtexFile(bibtexFilePath);
    if (!bibtexFile.open(QFile::ReadOnly))
        return result;

    QString bibtexFileContent = bibtexFile.readAll();
    auto allReferences = parse(bibtexFileContent);
    if (allReferences.count() == 0)
        return result;

    foreach (auto key, keys)
    {
        auto ref = allReferences.findReference(key);
        if (ref.isValid())
            result.addRecord(ref);
    }

    return result;
}

Reference BibParser::parseRecord(const QString& content) const
{
	Reference result;

	// type and id
    QRegExp rxTypeAndID("@(\\w+)\\{(\\w+)");
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
            result.setValue(fieldName, fieldValue);
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
