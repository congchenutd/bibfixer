#include "Commands.h"
#include "TextEdit.h"
#include "Parser.h"
#include "MainWindow.h"

Command::Command(MainWindow* mainWindow, QUndoCommand* parent)
	: QUndoCommand(parent), mainWnd(mainWindow) {}

void Command::undo()
{
	refCurrent = refBackup;                              // restore backup
	output(refCurrent.toString());                       // restore output
	highlightChanged();                                  // restore highlighting
	mainWnd->setActionStatus(getActionStatus(), false);  // restore action status
}

void Command::redo()
{
	refBackup = refCurrent;                              // backup
	refCurrent.clearChangedValues();                     // clear highlighting

	runCommand();                                        // template method

	output(refCurrent.toString());                       // output
	refCurrent.setHighlightColor(getHighlightColor());   // highlight
	highlightChanged();

	mainWnd->setActionStatus(getActionStatus(), true);   // update action status
}

void Command::output(const QString& text) {
	mainWnd->getTextEdit()->setPlainText(text);
}

void Command::highlightChanged()
{
	TextEdit* edit = mainWnd->getTextEdit();
	edit->unHighlightLines();
    QStringList toBeHighlighted = refCurrent.getChangedValues();
    foreach(const QString& line, toBeHighlighted)
		edit->addHighlightedLine(line, refCurrent.getHighlightColor());
	edit->highlightLines();
}

ReferenceList Command::refCurrent;


//////////////////////////////////////////////////////////////////////////////////////////
CleanCommand::CleanCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent)
{
    originalText = mainWindow->getTextEdit()->toPlainText();
    setText("Clean");
}

void CleanCommand::undo()
{
	output(originalText);
    mainWnd->setActionStatus(MainWindow::Cleaned, false);
}

void CleanCommand::runCommand() {
	refCurrent = BibParser().parse(originalText);
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
