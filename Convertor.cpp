#include "Convertor.h"
#include <QRegExp>
#include <QtAlgorithms>

namespace BibFixer {

//////////////////////////////////////////////////////////////////
CaseConvertor::CaseConvertor()
{
	// Prepositions
	_lowercaseWords << "about" << "above" << "across" << "after" <<
                       "against" << "along" << "among" << "around" <<
                       "at" << "before" << "behind" << "below" <<
                       "beneath" << "beside" << "between" << "beyond" <<
                       "but" << "by" << "despite" << "down" <<
                       "during" << "except" << "for" << "from" <<
                       "in" << "inside" << "into" << "like" <<
                       "near" << "of" << "off" << "on" <<
                       "onto" << "out" << "outside" << "over" <<
                       "past" << "since" << "through" << "throughout" <<
                       "till" << "to" << "toward" << "under" <<
                       "underneath" << "until" << "up" << "upon" <<
                       "with" << "within" << "without";

	// Conjunctions
	_lowercaseWords << "after" << "although" << "as" << "because" <<
                       "before" << "how" << "if" << "once" <<
                       "since" << "than" << "that" << "though" <<
                       "till" << "until" << "when" << "where" <<
                       "whether" << "while" << "and";

	// Articles
	_lowercaseWords << "the" << "a" << "an";
}

QString CaseConvertor::convert(const QString& input) const
{
    if(input.isEmpty())
        return QString();

	QStringList convertedWords;
    QStringList words = input.simplified().split(' ');
	QString lastWord;
	foreach(QString word, words)
	{
        if(_lowercaseWords.contains(word, Qt::CaseInsensitive)
                && !containsPunctuation(lastWord))
            convertedWords << word.toLower();  // lower case unless it follows a punctuation
		else
			convertedWords << toFirstCharUpperCase(word);   // convert
		lastWord = word;
	}

	// the first char of a sentence must be upper case
    return toFirstCharUpperCase(convertedWords.join(" "));
}

QString CaseConvertor::toFirstCharUpperCase(const QString& word) const {
    return word.isEmpty() ? word
                          : word.at(0).toUpper() + word.right(word.length() - 1);
}

bool CaseConvertor::containsPunctuation(const QString& word) const
{
    foreach(const QChar& ch, word)
        if(ch.isPunct())
            return true;
    return false;
}

/////////////////////////////////////////////////////////////////////////////////
QString UnprotectionConvertor::convert(const QString& input) const
{
    if(input.isEmpty())
        return QString();

    // get rid of the outer {}
    QString line = input.simplified();
    QRegExp rxAllProtected("^\\{(.+)\\}$");
    if(rxAllProtected.indexIn(line) > -1)
        line = rxAllProtected.cap(1);

    // split into words and remove the {} for the first letter
    QStringList wordList = line.split(' ');
    QStringList result;
    foreach(QString word, wordList)
    {
        QRegExp rxFirstProtected("^\\{\\w\\}");
        if(rxFirstProtected.indexIn(word) > -1)
        {
            word.remove(0, 1);
            word.remove(1, 1);
        }
        result << word;
    }

    return result.join(" ");
}

//////////////////////////////////////////////////////////////////////////////////
QString AllProtectionConvertor::convert(const QString& input) const {
    return input.isEmpty() ? QString()
                           : "{" + input.simplified() + "}";
}

//////////////////////////////////////////////////////////////////////////////////
QString FirstLetterProtectionConvertor::convert(const QString& input) const
{
    if(input.isEmpty())
        return QString();

    QStringList convertedWords;
    QStringList words = input.split(' ');
    foreach(QString word, words)
        convertedWords << toFirstCharProtected(word);   // convert

    return convertedWords.join(" ");
}

QString FirstLetterProtectionConvertor::toFirstCharProtected(const QString& word) const
{
    if(word.isEmpty())
        return word;

    // skip {XXX}
    QRegExp rxAllProtected("^\\{.+\\}$");
    if(rxAllProtected.indexIn(word) > -1)
        return word;

    // skip {X}xxx
    QRegExp rxFirstProtected("^\\{\\w\\}");
    if(rxFirstProtected.indexIn(word) > -1)
        return word;

    // skip non-letter chars
    QString result = word;
    int idx = 0;
    while(idx < result.length() && !result.at(idx).isLetter())
        ++ idx;

    // X... -> {X...}
    if(idx < result.length() && result.at(0).isUpper())
    {
        result.insert(idx, '{');
        result.insert(idx + 2, '}');
    }
    return result;
}

///////////////////////////////////////////////////////////////////////
QString AbbreviationConvertor::convert(const QString& input) const
{
    if(input.isEmpty())
        return QString();

    QString result = input;
    foreach(const QString& rule, _rules)
    {
        QStringList sections = rule.split(';');  // two parts of a rule
        if(sections.size() == 2)
        {
            QString fullName        = sections[0];
            QString abbreviatedName = sections[1];

            // handle optional words in the fullname
            // e.g., fullname = Journal (of)
            // longFullName = Journal of
            // shortFullName = Journal
            QRegExp rxBracket("\\([^\\)]+\\)");
            if(rxBracket.indexIn(fullName) > -1)
            {
                QString longFullName = fullName.simplified();
                longFullName.remove('(');
                longFullName.remove(')');

                QString shortFullName = fullName.remove(rxBracket).simplified();

                // try longFullName first, then shortFullName
                result.replace(longFullName,  abbreviatedName, Qt::CaseInsensitive);
                result.replace(shortFullName, abbreviatedName, Qt::CaseInsensitive);
            }
            else
                result.replace(fullName, abbreviatedName, Qt::CaseInsensitive);
        }
    }

    return result;
}

void AbbreviationConvertor::setRules(const QStringList& rules)
{
    _rules = rules;

    // reverse order: to ensure long rule is applied first
    // e.g., Journal (of) before Journal
    qSort(_rules.begin(), _rules.end(), qGreater<QString>());
}

}
