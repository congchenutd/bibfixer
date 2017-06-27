#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QString>
#include <QObject>

namespace BibFixer {

class Reference;

// generate bibkey for a reference
class KeyGenerator : public QObject
{
public:
    // the rule consists of multiple subrules (patterns), in the form of [r1][r2]
    KeyGenerator(const QString& rule);
    QString generateKey(const Reference& ref) const;

private:
    QString parseSubRule(const Reference& ref, const QString& subRule) const;

    QString getAuthor     (const Reference& ref, int order = 1) const;  // orderth author's fullname
    QString getFirstAuthor(const Reference& ref) const;    // first author's full name
    QString getLastName   (const Reference& ref) const;    // first author's last name
    QString getFirstName  (const Reference& ref) const;    // first author's first name
    QString getFirstLetter(const Reference& ref) const;    // the first letter of first author's first name
    QString getFirstWord  (const Reference& ref) const;    // the first word of the title
    QString getYear       (const Reference& ref) const;

private:
    QString _rule;
};  // KeyGenerator

}  // namespace BibFixer

#endif // KEYGENERATOR_H
