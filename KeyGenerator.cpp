#include "KeyGenerator.h"
#include "Reference.h"
#include "EnglishName.h"
#include "Convertor.h"
#include <QRegExp>

namespace BibFixer {

KeyGenerator::KeyGenerator(const QString& rule)
    : _rule (rule.toLower()) {}

QString KeyGenerator::generateKey(const Reference& ref) const
{
    QString result;
    QRegExp rxSubRule("\\[([\\w\\.]+)\\]");
    int index = rxSubRule.indexIn(_rule);
    while(index > -1)
    {
        QString subRule = rxSubRule.cap(1);
        result += parseSubRule(ref, subRule);
        index = rxSubRule.indexIn(_rule, index + rxSubRule.matchedLength());
    }
    return result;
}

QString KeyGenerator::parseSubRule(const Reference& ref, const QString& subRule) const
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
        QRegExp rx("author(\\d)\\.", Qt::CaseInsensitive);  // e.g., author1
        if(rx.indexIn(subRule) > -1)
            order = rx.cap(1).toInt();

        QString author = getAuthor(ref, order);  // fullname
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

QString KeyGenerator::getAuthor(const Reference& ref, int order) const
{
    QString authors = ref.getFieldValue("author");
    if(authors.isEmpty())
        return QString();

    QStringList authorList = authors.split(" and ");
    if(authorList.isEmpty())
        return QString();

    if(order > authorList.size())
        return QString();

    return order <= 0 ? authorList.last() : authorList.at(order - 1);
}

QString KeyGenerator::getFirstAuthor(const Reference& ref) const {
    return getAuthor(ref, 1);
}

QString KeyGenerator::getLastName(const Reference& ref) const {
    return EnglishName(getFirstAuthor(ref)).getLastName().simplified();
}

QString KeyGenerator::getFirstName(const Reference& ref) const {
    return EnglishName(getFirstAuthor(ref)).getFirstName().simplified();
}

QString KeyGenerator::getFirstLetter(const Reference& ref) const
{
    QString firstName = getFirstName(ref);
    return firstName.isEmpty() ? QString() : firstName.at(0);
}

QString KeyGenerator::getFirstWord(const Reference& ref) const
{
    QString title = ProtectionConvertor().undo(ref.getFieldValue("title"));
    return title.isEmpty() ? QString() : title.split(" ").front().simplified();
}

QString KeyGenerator::getYear(const Reference& ref) const
{
    QString year = ref.getFieldValue("year");
    return year.isEmpty() ? QString() : year;
}

}  // BibFixer
