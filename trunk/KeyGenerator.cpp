#include "KeyGenerator.h"
#include "Reference.h"
#include "EnglishName.h"

namespace BibFixer {

KeyGenerator::KeyGenerator(const Reference* reference)
    : ref(reference) {}

QString KeyGenerator::generate(const QString& rule)
{
    QString key;
    QStringList patterns = rule.split(";");
    foreach(const QString& pattern, patterns)
    {
        if(pattern == "lastname")
            key += getLastName();
        else if(pattern == "firstname")
            key += getFirstName();
        else if(pattern == "firstletter")
            key += getFirstLetter();
        else if(pattern == "firstword")
            key += getFirstWord();
        else if(pattern == "year")
            key += getYear();
    }
    return key;
}

QString KeyGenerator::getFirstAuthor() const
{
    QString authors = ref->getFieldValue("author");
    if(authors.isEmpty())
        return QString();

    QStringList authorList = authors.split(" and ");
    if(authorList.size() < 1)
        return QString();

    return authorList.front();
}

QString KeyGenerator::getLastName() const {
    return EnglishName(getFirstAuthor()).getLastName().remove(" ");   // no space
}

QString KeyGenerator::getFirstName() const {
    return EnglishName(getFirstAuthor()).getFirstName().remove(" ");  // no space
}

QString KeyGenerator::getFirstLetter() const
{
    QString firstName = getFirstName();
    return firstName.isEmpty() ? QString() : firstName.at(0);
}

QString KeyGenerator::getFirstWord() const
{
    QString title = ref->getFieldValue("title");
    if(title.isEmpty())
        return QString();

    return title.split(" ").front();   // get the first word
}

QString KeyGenerator::getYear() const
{
    QString year = ref->getFieldValue("year");
    return year.isEmpty() ? QString() : year;
}

}
