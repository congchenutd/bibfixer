#include "KeyGenerator.h"
#include "Reference.h"
#include "EnglishName.h"

namespace BibFixer {

namespace KeyGenerator {

QString generateKey(const Reference& ref, const QString& rule)
{
    QString key;
    QStringList patterns = rule.split(";");
    foreach(const QString& pattern, patterns)
    {
        if(pattern == "lastname")
            key += getLastName(ref);
        else if(pattern == "firstname")
            key += getFirstName(ref);
        else if(pattern == "firstletter")
            key += getFirstLetter(ref);
        else if(pattern == "firstword")
            key += getFirstWord(ref);
        else if(pattern == "year")
            key += getYear(ref);
    }
    return key;
}

//////////////////////////////////////////////////////////////
QString getFirstAuthor(const Reference& ref)
{
    QString authors = ref.getFieldValue("author");
    if(authors.isEmpty())
        return QString();

    QStringList authorList = authors.split(" and ");
    return authorList.isEmpty() ? QString() : authorList.front();
}

QString getLastName(const Reference& ref) {
    return EnglishName(getFirstAuthor(ref)).getLastName().remove(" ");   // no space
}

QString getFirstName(const Reference& ref) {
    return EnglishName(getFirstAuthor(ref)).getFirstName().remove(" ");  // no space
}

QString getFirstLetter(const Reference& ref)
{
    QString firstName = getFirstName(ref);
    return firstName.isEmpty() ? QString() : firstName.at(0);
}

QString getFirstWord(const Reference& ref)
{
    QString title = ref.getFieldValue("title");
    return title.isEmpty() ? QString() : title.split(" ").front();
}

QString getYear(const Reference& ref)
{
    QString year = ref.getFieldValue("year");
    return year.isEmpty() ? QString() : year;
}

}  // KeyGenerator
}  // BibFixer
