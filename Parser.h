#ifndef PARSER_H
#define PARSER_H

#include "Reference.h"

// Parse the content of a reference file, and produce a ReferenceList
class Parser
{
public:
	virtual ~Parser() {}
	virtual ReferenceList parse(const QString& content) const = 0;
};

class BibParser : public Parser
{
public:
    void setValidFields(const QStringList& fields);
	virtual ReferenceList parse(const QString& content) const;

private:
	ReferenceRecord parseRecord(const QString& content) const;
	int findRecordStart(const QString& content, int startFrom = 0) const;
	int findRecordEnd  (const QString& content, int startFrom = 0) const;

private:
    QStringList validFields;
};

#endif // PARSER_H
