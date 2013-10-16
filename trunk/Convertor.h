#ifndef CONVERTOR_H
#define CONVERTOR_H

#include <QStringList>
#include <QObject>

namespace BibFixer {

class UnConvertor;

// string format convertors
class IConvertor
{
public:
    virtual ~IConvertor() {}
    virtual QString redo(const QString& input) const = 0;
    virtual QString undo(const QString& input) const = 0;
};

// a special convertor that unconverts
class UnConvertor : public IConvertor, public QObject
{
public:
    UnConvertor(const IConvertor* convertor, QObject* parent = 0);
    QString redo(const QString& input) const;
    QString undo(const QString& input) const;

private:
    const IConvertor* _convertor;
};

// convert to title case
class CaseConvertor : public IConvertor, public QObject
{
public:
    CaseConvertor(QObject* parent = 0);
    QString redo(const QString& input) const;
    QString undo(const QString& input) const;

private:
    QString makeFirstCharUpper (const QString& word) const;
    bool    endsWithPunctuation(const QString& word) const;

private:
    QStringList _lowercaseWords;
};

// protect the entire sentence by {}
class ProtectionConvertor : public IConvertor, public QObject
{
public:
    ProtectionConvertor(QObject* parent = 0) : QObject(parent) {}
    QString redo(const QString& input) const;
    QString undo(const QString& input) const;
};

// shorten first and middle names
class ShortenNamesConvertor : public IConvertor, public QObject
{
public:
    ShortenNamesConvertor(QObject* parent = 0) : QObject(parent) {}
    QString redo(const QString& input) const;
    QString undo(const QString& input) const;

private:
    static QString _backup;
};

// abbreviate fields based on the rules, such as transactions -> trans.
class AbbreviationConvertor : public IConvertor, public QObject
{
public:
    AbbreviationConvertor(const QStringList& rules, QObject* parent = 0);
    QString redo(const QString& input) const;
    QString undo(const QString& input) const;
    void setRules(const QStringList& rules);   // format: longterm;shortterm

private:
    struct AbbreviationRule
    {
        QString _longTerm;
        QString _shortTerm;

        AbbreviationRule(const QString& ruleString);  // format: longterm;shortterm
        AbbreviationRule(const QString& longTerm, const QString& shortTerm);
        bool isValid() const;
        QList<AbbreviationRule> expand() const;       // expand rules with options

        bool operator<(const AbbreviationRule& other) const;  // for sorting
    };

    QList<AbbreviationRule> _rules;
};

}

#endif // CONVERTOR_H
