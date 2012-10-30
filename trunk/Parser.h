#ifndef PARSER_H
#define PARSER_H

#include "Reference.h"

namespace BibFixer {

// Parse the content of a reference (string), and produce a ReferenceList
class Parser
{
public:
	virtual ~Parser() {}
	virtual ReferenceList parse(const QString& content) const = 0;
};

class BibParser : public Parser
{
public:
    void setValidFields(const QStringList& validFields);        // invalid fields will be ignored
    virtual ReferenceList parse(const QString& content) const;  // may contain multi references
    Reference parseRecord(const QString& content) const;        // just one reference's content

private:
	int findRecordStart(const QString& content, int startFrom = 0) const;
	int findRecordEnd  (const QString& content, int startFrom = 0) const;

private:
    QStringList _validFields;
};

}

#endif // PARSER_H
