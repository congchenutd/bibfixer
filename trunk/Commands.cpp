#include "Commands.h"
#include "TextEdit.h"
#include "Parser.h"
#include "MainWindow.h"

Command::Command(MainWindow* mainWindow, QUndoCommand* parent)
	: QUndoCommand(parent), mainWnd(mainWindow) {}

void Command::undo()
{
    refCurrent = refBackup;
	output(refCurrent.toString());
	highlightChanged();
}

void Command::output(const QString& text) {
	mainWnd->getTextEdit()->setPlainText(text);
}

void Command::highlightChanged()
{
	mainWnd->getTextEdit()->unHighlightLines();
    QStringList toBeHighlighted = refCurrent.getChangedValues();
    foreach(const QString& line, toBeHighlighted)
		mainWnd->getTextEdit()->addHighlightedLine(line, refCurrent.getHighlightColor());
	mainWnd->getTextEdit()->highlightLines();
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

void CleanCommand::redo()
{
	BibParser parser;
    refCurrent = parser.parse(originalText);
    refCurrent.clearChangedValues();
	output(refCurrent.toString());
    mainWnd->setActionStatus(MainWindow::Cleaned, true);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
CapitalizeCommand::CapitalizeCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Capitalize");
}

void CapitalizeCommand::undo()
{
	Command::undo();
    mainWnd->setActionStatus(MainWindow::Capitalized, false);
}

void CapitalizeCommand::redo()
{
    refBackup = refCurrent;
    refCurrent.clearChangedValues();
    refCurrent.capitalize("title");
    refCurrent.capitalize("journal");
    refCurrent.capitalize("booktitle");
    refCurrent.setHighlightColor(Qt::yellow);
	output(refCurrent.toString());
    highlightChanged();
    mainWnd->setActionStatus(MainWindow::Capitalized, true);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
ProtectCommand::ProtectCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Protect");
}

void ProtectCommand::undo()
{
	Command::undo();
    mainWnd->setActionStatus(MainWindow::Protected, false);
}

void ProtectCommand::redo()
{
    refBackup = refCurrent;
    refCurrent.clearChangedValues();
    refCurrent.protect("title");
	refCurrent.setHighlightColor(Qt::cyan);
	output(refCurrent.toString());
    highlightChanged();
    mainWnd->setActionStatus(MainWindow::Protected, true);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
AbbreviateCommand::AbbreviateCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Abbreviate");
}

void AbbreviateCommand::undo()
{
	Command::undo();
    mainWnd->setActionStatus(MainWindow::Abbreviated, false);
}

void AbbreviateCommand::redo()
{
    refBackup = refCurrent;
    refCurrent.clearChangedValues();
    refCurrent.abbreviate("journal");
    refCurrent.abbreviate("booktitle");
    refCurrent.setHighlightColor(Qt::green);
	output(refCurrent.toString());
    highlightChanged();
    mainWnd->setActionStatus(MainWindow::Abbreviated, true);
}


/////////////////////////////////////////////////////////////////////////////
GenerateKeysCommand::GenerateKeysCommand(MainWindow* mainWindow, QUndoCommand* parent)
    : Command(mainWindow, parent) {
    setText("Generate keys");
}

void GenerateKeysCommand::undo()
{
    Command::undo();
    mainWnd->setActionStatus(MainWindow::KeysGenerated, false);
}

void GenerateKeysCommand::redo()
{
    refBackup = refCurrent;
    refCurrent.clearChangedValues();
    refCurrent.generateKeys();
    refCurrent.setHighlightColor(Qt::magenta);
    output(refCurrent.toString());
    highlightChanged();
    mainWnd->setActionStatus(MainWindow::KeysGenerated, true);
}
