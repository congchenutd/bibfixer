#include "KeyGenerator.h"
#include "Reference.h"
#include "EnglishName.h"
#include <QRegExp>

namespace BibFixer {

namespace KeyGenerator {

QString generateKey(const Reference& ref, const QString& rule)
{
    QString key;
//    QStringList patterns = rule.split(";");
//    foreach(const QString& pattern, patterns)
//    {
//        if(pattern == "lastname")
//            key += getLastName(ref);
//        else if(pattern == "firstname")
//            key += getFirstName(ref);
//        else if(pattern == "firstletter")
//            key += getFirstLetter(ref);
//        else if(pattern == "firstword")
//            key += getFirstWord(ref);
//        else if(pattern == "year")
//            key += getYear(ref);
//    }

    QRegExp rx("\\[([\\w\\.]+)\\]");
    int index = rx.indexIn(rule);
    while(index > -1)
    {
        QString entity = rx.cap(1).toLower();
        key += parseEntity(ref, entity);
        index = rx.indexIn(rule, index + rx.matchedLength());
    }
    return key;
}

QString parseEntity(const Reference& ref, const QString& entity)
{
    if(entity.startsWith("title"))
    {
        if(entity == "title.firstword")
            return getFirstWord(ref);
        else if(entity == "title.firstletter")
        {
            QString firstWord = getFirstWord(ref);
            return firstWord.isEmpty() ? QString() : firstWord.at(0);
        }
        else
            return ref.getFieldValue("title");
    }

    else if(entity.startsWith("author"))
    {
        int order = 1;
        QRegExp rx("author(\\d)\\.", Qt::CaseInsensitive);
        if(rx.indexIn(entity) > -1)
            order = rx.cap(1).toInt();

        QString author = getAuthor(ref, order);
        if(author.isEmpty())
            return author;

        if(entity.endsWith(".lastname"))
            return EnglishName(author).getLastName();
        else if(entity.endsWith(".firstname"))
            return EnglishName(author).getFirstName();
        else if(entity.endsWith(".firstletter"))
            return author.at(0);
        else
            return author;
    }

    else if(entity == "year")
        return getYear(ref);

    return QString("Invalid pattern");
}

QString getAuthor(const Reference& ref, int order)
{
    QString authors = ref.getFieldValue("author");
    if(authors.isEmpty())
        return QString();

    QStringList authorList = authors.split(" and ");
    if(authorList.isEmpty())
        return QString();

    if(order > authorList.size())
        return QString();

    return order == 0 ? authorList.last() : authorList.at(order - 1);
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
