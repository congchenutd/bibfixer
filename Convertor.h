#ifndef CONVERTOR_H
#define CONVERTOR_H

#include <QStringList>

// string format convertors
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
    void setRules(const QStringList& r);   // fullname;abbreviatedname

private:
    QStringList rules;
};

#endif // CONVERTOR_H
