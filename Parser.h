#ifndef PARSER_H
#define PARSER_H

#include "Reference.h"

namespace BibFixer {

// Parse the content of a reference (string), and produce a ReferenceList
class IParser
{
public:
    virtual ~IParser() {}
	virtual ReferenceList parse(const QString& content) const = 0;
};

class BibParser : public IParser
{
public:
    BibParser(const QStringList& validFields);            // invalid fields will be ignored
    ReferenceList parse(const QString& content) const;    // may contain multi references

private:
	int findRecordStart(const QString& content, int startFrom = 0) const;
	int findRecordEnd  (const QString& content, int startFrom = 0) const;
    Reference parseRecord(const QString& content) const;  // parse one reference's content

private:
    QStringList _validFields;
};

}

#endif // PARSER_H
