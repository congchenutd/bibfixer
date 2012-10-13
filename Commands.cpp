#include "Commands.h"
#include "TextEdit.h"
#include "Parser.h"
#include "MainWindow.h"
#include "DlgSettings.h"

Command::Command(MainWindow* mainWindow, QUndoCommand* parent)
	: QUndoCommand(parent), mainWnd(mainWindow) {}

void Command::undo()
{
	refCurrent = refBackup;                              // restore backup
	output(refCurrent.toString());                       // restore output
    highlight();                                         // restore highlighting
	mainWnd->setActionStatus(getActionStatus(), false);  // restore action status
}

void Command::redo()
{
	refBackup = refCurrent;                              // backup
	refCurrent.clearChangedValues();                     // clear highlighting

	runCommand();                                        // template method

	output(refCurrent.toString());                       // output
	refCurrent.setHighlightColor(getHighlightColor());   // highlight
    highlight();

	mainWnd->setActionStatus(getActionStatus(), true);   // update action status
}

void Command::output(const QString& text) {
	mainWnd->getTextEdit()->setPlainText(text);
}

void Command::highlight()
{
	TextEdit* edit = mainWnd->getTextEdit();
	edit->unHighlight();
    QStringList toBeHighlighted = refCurrent.getChangedValues();
    foreach(const QString& text, toBeHighlighted)
        edit->addHighlightedText(text, refCurrent.getHighlightColor());
	edit->highlight();
}

ReferenceList Command::refCurrent;


//////////////////////////////////////////////////////////////////////////////////////////
CleanCommand::CleanCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent)
{
    originalText = mainWindow->getTextEdit()->toPlainText();
    setText("Clean");
}

// different than other commands, because it's the first command and has no backups
void CleanCommand::undo()
{
	output(originalText);
    mainWnd->setActionStatus(MainWindow::Cleaned, false);
}

void CleanCommand::runCommand()
{
    BibParser parser;
    parser.setValidFields(Setting::getInstance("Rules.ini")->getFields());
    refCurrent = parser.parse(originalText);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
CapitalizeCommand::CapitalizeCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Capitalize");
}

void CapitalizeCommand::runCommand()
{
    refCurrent.capitalize("title");
    refCurrent.capitalize("journal");
    refCurrent.capitalize("booktitle");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
ProtectCommand::ProtectCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Protect");
}

void ProtectCommand::runCommand() {
    refCurrent.protect("title");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
AbbreviateCommand::AbbreviateCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Abbreviate");
}

void AbbreviateCommand::runCommand()
{
    refCurrent.abbreviate("journal");
    refCurrent.abbreviate("booktitle");
}


/////////////////////////////////////////////////////////////////////////////
GenerateKeysCommand::GenerateKeysCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Generate keys");
}

void GenerateKeysCommand::runCommand() {
    refCurrent.generateKeys();
}
