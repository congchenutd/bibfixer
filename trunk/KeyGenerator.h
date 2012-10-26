#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QString>

namespace BibFixer {

class Reference;
class KeyGenerator
{
public:
    explicit KeyGenerator(const Reference* reference);
    QString generate(const QString& rule);

private:
    QString getFirstAuthor() const;    // first author's full name
    QString getLastName()    const;    // first author's last name
    QString getFirstName()   const;    // first author's first name
    QString getFirstLetter() const;    // the first letter of first author's first name
    QString getFirstWord()   const;    // the first word of the title
    QString getYear()        const;

private:
    const Reference* ref;
};

}

#endif // KEYGENERATOR_H
