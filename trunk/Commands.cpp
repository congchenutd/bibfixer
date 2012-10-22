#include "Commands.h"
#include "TextEdit.h"
#include "Parser.h"
#include "MainWindow.h"
#include "DlgSettings.h"

Command::Command(MainWindow* mainWindow, QUndoCommand* parent)
	: QUndoCommand(parent), mainWnd(mainWindow) {}

void Command::undo()
{
    currentSnapshot = backupSnapshot;                    // restore backup
    output(currentSnapshot.toString());                  // restore output
    highlight();                                         // restore highlighting
    mainWnd->setActionStatus(getActionName(), false);    // restore action status
}

void Command::redo()
{
    backupSnapshot = currentSnapshot;                       // backup
    currentSnapshot.clearChangedText();                     // clear highlighting

    runCommand();                                           // template method

    output(currentSnapshot.toString());                     // output
    currentSnapshot.setHighlightingColor(getHighlightColor()); // the color goes with ref for undo
    highlight();

    mainWnd->setActionStatus(getActionName(), true);        // update action status
}

void Command::output(const QString& text) {
	mainWnd->getTextEdit()->setPlainText(text);
}

void Command::highlight()
{
	TextEdit* edit = mainWnd->getTextEdit();
	edit->unHighlight();
    const QStringList toBeHighlighted = currentSnapshot.getChangedText();
    foreach(const QString& text, toBeHighlighted)
        edit->addHighlightedText(text, currentSnapshot.getHighlightingColor());
	edit->highlight();
}

//ReferenceList Command::currentSnapshot;


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
    mainWnd->setActionStatus(MainWindow::Clean, false);
}

void CleanCommand::runCommand()
{
    BibParser parser;
    parser.setValidFields(Setting::getInstance("Rules.ini")->getFields());
    currentSnapshot = parser.parse(originalText);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
CapitalizeCommand::CapitalizeCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Capitalize");
}

void CapitalizeCommand::runCommand()
{
    currentSnapshot.capitalize("title");
    currentSnapshot.capitalize("journal");
    currentSnapshot.capitalize("booktitle");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
ProtectCommand::ProtectCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Protect");
}

void ProtectCommand::runCommand() {
    currentSnapshot.protect("title");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
AbbreviateCommand::AbbreviateCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Abbreviate");
}

void AbbreviateCommand::runCommand()
{
    currentSnapshot.abbreviate("journal");
    currentSnapshot.abbreviate("booktitle");
}


/////////////////////////////////////////////////////////////////////////////
GenerateKeysCommand::GenerateKeysCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Generate keys");
}

void GenerateKeysCommand::runCommand() {
    currentSnapshot.generateKeys();
}
