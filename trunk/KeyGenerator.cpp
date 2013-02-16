#include "KeyGenerator.h"
#include "Reference.h"
#include "EnglishName.h"
#include "Convertor.h"
#include <QRegExp>

namespace BibFixer {

namespace KeyGenerator {

QString generateKey(const Reference& ref, const QString& rule)
{
    QString result;
    QRegExp rxSubRule("\\[([\\w\\.]+)\\]");
    int index = rxSubRule.indexIn(rule);
    while(index > -1)
    {
        QString subRule = rxSubRule.cap(1).toLower();
        result += parseSubRule(ref, subRule);
        index = rxSubRule.indexIn(rule, index + rxSubRule.matchedLength());
    }
    return result;
}

QString parseSubRule(const Reference& ref, const QString& subRule)
{
    if(subRule.startsWith("title"))
    {
        if(subRule == "title.firstword")
            return getFirstWord(ref);
        else if(subRule == "title.firstletter")
        {
            QString firstWord = getFirstWord(ref);
            return firstWord.isEmpty() ? QString() : firstWord.at(0);
        }
        else
            return ref.getFieldValue("title");
    }

    else if(subRule.startsWith("author"))
    {
        int order = 1;
        QRegExp rx("author(\\d)\\.", Qt::CaseInsensitive);
        if(rx.indexIn(subRule) > -1)
            order = rx.cap(1).toInt();

        QString author = getAuthor(ref, order);
        if(author.isEmpty())
            return author;

        if(subRule.endsWith(".lastname"))
            return EnglishName(author).getLastName();
        else if(subRule.endsWith(".firstname"))
            return EnglishName(author).getFirstName();
        else if(subRule.endsWith(".firstletter"))
            return author.at(0);
        else
            return author;
    }

    else if(subRule == "year")
        return getYear(ref);

    return QString("Invalid keygen rules");
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
QString getFirstAuthor(const Reference& ref) {
    return getAuthor(ref, 1);
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
    QString title =UnprotectionConvertor().convert(ref.getFieldValue("title"));
    return title.isEmpty() ? QString() : title.split(" ").front();
}

QString getYear(const Reference& ref)
{
    QString year = ref.getFieldValue("year");
    return year.isEmpty() ? QString() : year;
}

}  // KeyGenerator
}  // BibFixer
