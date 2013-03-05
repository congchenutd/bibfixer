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
    _mainWnd->updateActionStatus(getActionName(), false);  // set this action unchecked
    output(_reference.toString());
    highlight();
}

void AbstractCommand::redo()
{
    _mainWnd->updateActionStatus(getActionName(), true);  // set this action unchecked
    output(_reference.toString());
    highlight();
}

void AbstractCommand::output(const QString& text) {
	_mainWnd->getTextEdit()->setPlainText(text);
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
    _mainWnd->updateActionStatus(getActionName(), false);
    output(_originalText);  // don't use _reference.toString() because it's not inited yet
}

void CleanCommand::redo()
{
    QStringList validFields = Setting::getInstance("Rules.ini")->getSelectedFields();
    _reference = BibParser(validFields).parse(_originalText);
    _mainWnd->updateActionStatus(getActionName(), true);
    output(_reference.toString());
    // no highlight needed
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
CapitalizeCommand::CapitalizeCommand(MainWindow* mainWindow)
    : AbstractCommand(mainWindow) {}

void CapitalizeCommand::undo()
{
    run(new UnConvertor(new CaseConvertor(this)));
    AbstractCommand::undo();
}

void CapitalizeCommand::redo()
{
    run(new CaseConvertor(this));
    AbstractCommand::redo();
}

void CapitalizeCommand::run(IConvertor* convertor)
{
    _reference.clearChangedText();
    _reference.convertField(convertor, "title");
    _reference.convertField(convertor, "journal");
    _reference.convertField(convertor, "booktitle");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
ProtectCommand::ProtectCommand(MainWindow* mainWindow)
    : AbstractCommand(mainWindow) {}

void ProtectCommand::undo()
{
    run(new UnConvertor(new ProtectionConvertor(this)));
    AbstractCommand::undo();
}

void ProtectCommand::redo()
{
    run(new ProtectionConvertor(this));
    AbstractCommand::redo();
}

void ProtectCommand::run(IConvertor* convertor)
{
    _reference.clearChangedText();
    _reference.convertField(convertor, "title");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
AbbreviateCommand::AbbreviateCommand(MainWindow* mainWindow)
    : AbstractCommand(mainWindow) {
    _rules = Setting::getInstance("Rules.ini")->getSelectedAbbreviationRules();
}

void AbbreviateCommand::undo()
{
    run(new UnConvertor(new AbbreviationConvertor(_rules, this)));
    AbstractCommand::undo();
}

void AbbreviateCommand::redo()
{
    run(new AbbreviationConvertor(_rules, this));
    AbstractCommand::redo();
}

void AbbreviateCommand::run(IConvertor* convertor)
{
    _reference.clearChangedText();                     // clear highlighting
    _reference.convertField(convertor, "journal");
    _reference.convertField(convertor, "booktitle");
}


/////////////////////////////////////////////////////////////////////////////
GenerateKeysCommand::GenerateKeysCommand(MainWindow* mainWindow)
    : AbstractCommand(mainWindow) {}

void GenerateKeysCommand::redo()
{
    _reference.generateKeys(Setting::getInstance("Rules.ini")->getKeyGenRule());
    AbstractCommand::redo();
}


}
