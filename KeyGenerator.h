#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QString>

namespace BibFixer {

class Reference;

// generate bibkey for a reference
namespace KeyGenerator {

// the rule is consist of multiple subrules (patterns), in the form of [r1][r2]
QString generateKey(const Reference& ref, const QString& rule);

QString parseSubRule(const Reference& ref, const QString& subRule);

QString getAuthor(const Reference& ref, int order = 1);
QString getFirstAuthor(const Reference& ref);    // first author's full name
QString getLastName   (const Reference& ref);    // first author's last name
QString getFirstName  (const Reference& ref);    // first author's first name
QString getFirstLetter(const Reference& ref);    // the first letter of first author's first name
QString getFirstWord  (const Reference& ref);    // the first word of the title
QString getYear       (const Reference& ref);

}  // KeyGenerator
}  // BibFixer

#endif // KEYGENERATOR_H