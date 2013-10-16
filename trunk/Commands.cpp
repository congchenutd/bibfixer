#include "Commands.h"
#include "TextEdit.h"
#include "Parser.h"
#include "MainWindow.h"
#include "DlgSettings.h"
#include "Convertor.h"

namespace BibFixer {

AbstractCommand::AbstractCommand(MainWindow* mainWindow)
    : QObject(mainWindow), _mainWnd(mainWindow) {}

void AbstractCommand::undo()
{
    _mainWnd->updateActionStatus(getActionName(), false);  // set the action unchecked
    output(_reference.toString());
    highlight();
}

void AbstractCommand::redo()
{
    _mainWnd->updateActionStatus(getActionName(), true);   // set the action checked
    output(_reference.toString());
    highlight();
}

void AbstractCommand::output(const QString& text) {
    _mainWnd->getTextEdit()->setPlainText(text);
}

void AbstractCommand::run(IConvertor* convertor, const QStringList& fields)
{
    if(convertor != 0)
    {
        _reference.clearChangedText();
        foreach(const QString& field, fields)
            _reference.convertField(convertor, field);
    }
}

void AbstractCommand::highlight()
{
	TextEdit* edit = _mainWnd->getTextEdit();
	edit->unHighlight();
    foreach(const QString& text, _reference.getChangedText())
        edit->addHighlighting(text, getHighlightColor());
	edit->highlight();
}

ReferenceList AbstractCommand::_reference;

//////////////////////////////////////////////////////////////////////////////////////////
QString CleanCommand::_originalText;

CleanCommand::CleanCommand(MainWindow* mainWindow)
    : AbstractCommand(mainWindow) {}

void CleanCommand::undo()
{
    _mainWnd->updateActionStatus(getActionName(), false);  // set the action unchecked
    output(_originalText);                                 // restore
}

void CleanCommand::redo()
{
    _originalText = _mainWnd->getTextEdit()->toPlainText();  // backup

    QStringList validFields = Setting::getInstance("Rules.ini")->getSelectedFields();
    _reference = BibParser(validFields).parse(_originalText);

    _mainWnd->updateActionStatus(getActionName(), true);     // set the action checked
    output(_reference.toString());
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
CapitalizeCommand::CapitalizeCommand(MainWindow* mainWindow)
    : AbstractCommand(mainWindow) {}

void CapitalizeCommand::undo()
{
    run(new UnConvertor(new CaseConvertor(this)),
        QStringList() << "title" << "journal" << "booktitle");
    AbstractCommand::undo();
}

void CapitalizeCommand::redo()
{
    run(new CaseConvertor(this),
        QStringList() << "title" << "journal" << "booktitle");
    AbstractCommand::redo();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
ProtectCommand::ProtectCommand(MainWindow* mainWindow)
    : AbstractCommand(mainWindow) {}

void ProtectCommand::undo()
{
    run(new UnConvertor(new ProtectionConvertor(this)), QStringList() << "title");
    AbstractCommand::undo();
}

void ProtectCommand::redo()
{
    run(new ProtectionConvertor(this), QStringList() << "title");
    AbstractCommand::redo();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
AbbreviateCommand::AbbreviateCommand(MainWindow* mainWindow)
    : AbstractCommand(mainWindow) {
    _rules = Setting::getInstance("Rules.ini")->getSelectedAbbreviationRules();
}

void AbbreviateCommand::undo()
{
    run(new UnConvertor(new AbbreviationConvertor(_rules, this)),
        QStringList() << "journal" << "booktitle");
    AbstractCommand::undo();
}

void AbbreviateCommand::redo()
{
    run(new AbbreviationConvertor(_rules, this),
        QStringList() << "journal" << "booktitle");
    AbstractCommand::redo();
}

////////////////////////////////////////////////////////////////////////////
ShortenNamesCommand::ShortenNamesCommand(MainWindow* mainWindow)
    : AbstractCommand(mainWindow) {}

void ShortenNamesCommand::undo()
{

}

void ShortenNamesCommand::redo()
{
    run(new ShortenNamesConvertor(this), QStringList() << "author");
    AbstractCommand::redo();
}

/////////////////////////////////////////////////////////////////////////////
GenerateKeysCommand::GenerateKeysCommand(MainWindow* mainWindow)
    : AbstractCommand(mainWindow) {}

void GenerateKeysCommand::redo()
{
    _reference.clearChangedText();
    _reference.generateKeys(Setting::getInstance("Rules.ini")->getKeyGenRule());
    AbstractCommand::redo();
}


}
