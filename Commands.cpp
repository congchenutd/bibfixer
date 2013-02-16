#include "Commands.h"
#include "TextEdit.h"
#include "Parser.h"
#include "MainWindow.h"
#include "DlgSettings.h"

namespace BibFixer {

Command::Command(MainWindow* mainWindow, QUndoCommand* parent)
	: QUndoCommand(parent), _mainWnd(mainWindow) {}

void Command::undo()
{
    _currentSnapshot = _backupSnapshot;                  // restore backup
    output(_currentSnapshot.toString());                 // restore output
    highlight();                                         // restore highlighting
    _mainWnd->setTriggered(getActionName(), false);      // restore action status
}

void Command::redo()
{
    _backupSnapshot = _currentSnapshot;                      // backup
    _currentSnapshot.clearChangedText();                     // clear highlighting

    runCommand();                                            // template method

    output(_currentSnapshot.toString());                     // output
    _currentSnapshot.setHighlightingColor(getHighlightColor()); // the color goes with ref for undo
    highlight();

    QString result = _mainWnd->getTextEdit()->toPlainText();
    _mainWnd->setTriggered(getActionName(), true);        // update action status
}

void Command::output(const QString& text) {
	_mainWnd->getTextEdit()->setPlainText(text);
}

void Command::highlight()
{
	TextEdit* edit = _mainWnd->getTextEdit();
	edit->unHighlight();
    const QStringList toBeHighlighted = _currentSnapshot.getChangedText();
    foreach(const QString& text, toBeHighlighted)
        edit->addHighlighting(text, _currentSnapshot.getHighlightingColor());
	edit->highlight();
}

ReferenceList Command::_currentSnapshot;


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
    _mainWnd->setTriggered(MainWindow::Clean, false);
}

void CleanCommand::runCommand()
{
    BibParser parser;
    parser.setValidFields(Setting::getInstance("Rules.ini")->getSelectedFields());
    _currentSnapshot = parser.parse(originalText);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
CapitalizeCommand::CapitalizeCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Capitalize");
}

void CapitalizeCommand::runCommand()
{
    _currentSnapshot.capitalize("title");
    _currentSnapshot.capitalize("journal");
    _currentSnapshot.capitalize("booktitle");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
ProtectCommand::ProtectCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Protect");
}

void ProtectCommand::runCommand() {
    _currentSnapshot.protect("title");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
AbbreviateCommand::AbbreviateCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Abbreviate");
}

void AbbreviateCommand::runCommand()
{
    _currentSnapshot.abbreviate("journal");
    _currentSnapshot.abbreviate("booktitle");
}


/////////////////////////////////////////////////////////////////////////////
GenerateKeysCommand::GenerateKeysCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Generate keys");
}

void GenerateKeysCommand::runCommand() {
    _currentSnapshot.generateKeys();
}

}
