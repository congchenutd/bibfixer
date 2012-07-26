#ifndef CASECONVERTOR_H
#define CASECONVERTOR_H

#include <QStringList>

class Convertor
{
public:
	virtual ~Convertor() {}
    virtual QString convert(const QString& input) const = 0;
};

class CaseConvertor : public Convertor
{
public:
	CaseConvertor();
	QString convert(const QString& input) const;

private:
	QString toFirstCharUpperCase(const QString& word) const;

private:
	QStringList lowercaseWords;
};

class ProtectionConvertor : public Convertor
{
public:
	QString convert(const QString& input) const;

private:
	QString toFirstCharProtected(const QString& word) const;
};

class AbbreviationConvertor : public Convertor
{
public:
	QString convert(const QString& input) const;

private:
	QStringList rules;
};

#endif // CASECONVERTOR_H
