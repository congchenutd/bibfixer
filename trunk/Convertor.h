#ifndef CONVERTOR_H
#define CONVERTOR_H

#include <QStringList>

namespace BibFixer {

// string format convertors
class Convertor
{
public:
	virtual ~Convertor() {}
    virtual QString convert(const QString& input) const = 0;
};

// all words' first letter in upper case
class CaseConvertor : public Convertor
{
public:
	CaseConvertor();
	QString convert(const QString& input) const;

private:
    QString toFirstCharUpperCase(const QString& word) const;
    bool    containsPunctuation(const QString& word) const;

private:
    QStringList _lowercaseWords;
};

// protect the first letter of each word by {}
class FirstLetterProtectionConvertor : public Convertor
{
public:
    QString convert(const QString& input) const;

private:
    QString toFirstCharProtected(const QString& word) const;
};

// protect the entire sentence by {}
class AllProtectionConvertor : public Convertor
{
public:
    QString convert(const QString& input) const;
};

// remove the protective {}
class UnprotectionConvertor : public Convertor
{
public:
    QString convert(const QString& input) const;
};

// abbreviate based on the rules, such as transactions -> trans.
class AbbreviationConvertor : public Convertor
{
public:
    QString convert(const QString& input) const;
    void setRules(const QStringList& rules);   // format: fullname;abbreviatedname

private:
    QStringList _rules;
};

}

#endif // CONVERTOR_H
