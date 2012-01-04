#ifndef PARSER_H
#define PARSER_H

#include "Reference.h"
#include <QObject>

// Parse the content of a reference file, and produce a ReferenceList
class Parser : public QObject
{
public:
	virtual ~Parser() {}
	virtual ReferenceList parse(const QString& content) const = 0;
};

class BibParser : public Parser
{
public:
	virtual ReferenceList parse(const QString& content) const;

private:
	ReferenceRecord parseRecord(const QString& content) const;
	int findRecordStart(const QString& content, int startFrom = 0) const;
	int findRecordEnd  (const QString& content, int startFrom = 0) const;
};

#endif // PARSER_H
