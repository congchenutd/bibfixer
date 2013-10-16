#include "Convertor.h"
#include "EnglishName.h"
#include <QRegularExpression>
#include <QtAlgorithms>

namespace BibFixer {

UnConvertor::UnConvertor(const IConvertor* convertor, QObject* parent)
    : QObject(parent), _convertor(convertor) {}

QString UnConvertor::redo(const QString &input) const { return _convertor->undo(input); }
QString UnConvertor::undo(const QString &input) const { return _convertor->redo(input); }


////////////////////////////////////////////////////////////////////////////////////////
CaseConvertor::CaseConvertor(QObject* parent) : QObject(parent)
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
                       "whether" << "while" << "and" << "or";

	// Articles
	_lowercaseWords << "the" << "a" << "an";
}

QString CaseConvertor::redo(const QString& input) const
{
    if(input.isEmpty())
        return QString();

	QStringList convertedWords;
    QStringList wordList = input.simplified().split(' ');   // split the sentence
	QString lastWord;
    foreach(QString word, wordList)
	{
        if(_lowercaseWords.contains(word, Qt::CaseInsensitive) &&  // reserved
		   !endsWithPunctuation(lastWord))                         // follows a punctuation
            convertedWords << word.toLower();             // lower case
		else
            convertedWords << makeFirstCharUpper(word);   // upper case
		lastWord = word;
	}

	// the first char of a sentence must be upper case
    return makeFirstCharUpper(convertedWords.join(" "));
}

QString CaseConvertor::undo(const QString& input) const {
    return makeFirstCharUpper(input.toLower());
}

QString CaseConvertor::makeFirstCharUpper(const QString& word) const
{
	if(word.isEmpty())
		return word;

	// find 1st letter
	int i = 0;
	for(; i < word.length(); ++i)
		if(word.at(i).isLetter())
			break;
    return word.left(i) + word.at(i).toUpper() + word.right(word.length() - i - 1);
}

bool CaseConvertor::endsWithPunctuation(const QString& word) const {
    return word.at(word.length() - 1).isPunct();
}

//////////////////////////////////////////////////////////////////////////////////
QString ProtectionConvertor::redo(const QString& input) const
{
    if(input.isEmpty())
        return QString();

    QRegExp rxAllProtected("^\\{.+\\}$");
    if(rxAllProtected.indexIn(input) > -1)    // already protected
        return input.simplified();

    return "{" + input.simplified() + "}";
}

QString ProtectionConvertor::undo(const QString& input) const
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
        QRegExp rxFirstCharProtected("^\\{\\w\\}");
        if(rxFirstCharProtected.indexIn(word) > -1)
        {
            word.remove(0, 1);
            word.remove(1, 1);
        }
        result << word;
    }

    return result.join(" ");
}

////////////////////////////////////////////////////////////////////////////////////
QString ShortenNamesConvertor::_backup;

QString ShortenNamesConvertor::redo(const QString& input) const
{
    _backup = input;
    QStringList result;
    QStringList names = input.split(" and ");
    foreach(const QString& name, names)
        result << EnglishName(name).toString("L,;f;m");
    return result.join(" and ");
}

QString ShortenNamesConvertor::undo(const QString&) const {
    return _backup;
}

///////////////////////////////////////////////////////////////////////
AbbreviationConvertor::AbbreviationConvertor(const QStringList& rules, QObject* parent)
    : QObject(parent) {
    setRules(rules);
}

QString AbbreviationConvertor::redo(const QString& input) const
{
    if(input.isEmpty())
        return QString();

    QString result = input;
    foreach(const AbbreviationRule& rule, _rules)
        result.replace(rule._longTerm, rule._shortTerm, Qt::CaseInsensitive);
    return result;
}

QString AbbreviationConvertor::undo(const QString& input) const
{
    if(input.isEmpty())
        return QString();

    QString result = input;
    foreach(const AbbreviationRule& rule, _rules)
        result.replace(rule._shortTerm, rule._longTerm, Qt::CaseInsensitive);
    return result;
}

void AbbreviationConvertor::setRules(const QStringList& rules)
{
    _rules.clear();
    foreach(const QString& rule, rules)
        _rules << AbbreviationRule(rule).expand();

    // reverse order: to ensure long rule is applied first
    // e.g., Journal (of) before Journal
    qSort(_rules.begin(), _rules.end(), qGreater<AbbreviationRule>());
}

////////////////////////////////////////////////////////////////////////////////////
AbbreviationConvertor::AbbreviationRule::AbbreviationRule(const QString& ruleString)
{
    QStringList sections = ruleString.split(';');
    if(sections.size() <= 2)
    {
        _longTerm  = sections[0].simplified();
        _shortTerm = sections[1].simplified();
    }
}

AbbreviationConvertor::AbbreviationRule::AbbreviationRule(const QString& longTerm,
                                                          const QString& shortTerm)
    : _longTerm(longTerm),
      _shortTerm(shortTerm) {}

bool AbbreviationConvertor::AbbreviationRule::isValid() const {
    return !_longTerm.isEmpty() && !_shortTerm.isEmpty();
}

QList<AbbreviationConvertor::AbbreviationRule>
AbbreviationConvertor::AbbreviationRule::expand() const
{
    QList<AbbreviationRule> result;
    if(!isValid())
        return result;

    // find options, e.g.,  "(of|on)"
    QRegularExpression rxBracket("\\([^\\)]+\\)");
    QRegularExpressionMatch match = rxBracket.match(_longTerm);
    if(!match.hasMatch())   // no options, no expansion
    {
        result << *this;
        return result;
    }

    // fixed part, e.g., Conference in "Conference (of|on)"
    QString fixedPart = _longTerm;
    fixedPart.remove(rxBracket);
    fixedPart = fixedPart.simplified();

    // get options, e.g., of, on
    QString optionString = match.captured(0);
    optionString.remove("(");
    optionString.remove(")");
    QStringList options = optionString.split("|");

    // fixedPart + option makes a new rule
    foreach(const QString& option, options)
        result << AbbreviationRule(fixedPart + " " + option, _shortTerm);

    result << AbbreviationRule(fixedPart, _shortTerm);   // fixed rule

    return result;
}

bool AbbreviationConvertor::AbbreviationRule::operator <(const AbbreviationRule& other) const {
    return _longTerm < other._longTerm;
}

}
