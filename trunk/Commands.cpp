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
CleanCommand::CleanCommand(const QString& text, MainWindow* mainWindow, QUndoCommand* parent)
	: Command(mainWindow, parent), originalText(text)
{
	setText("Clean");
}

void CleanCommand::undo()
{
	output(originalText);
	mainWnd->setActionStatus(true, true, false, false, false, false);
}

void CleanCommand::redo()
{
	BibParser parser;
    refCurrent = parser.parse(originalText);
    refCurrent.clearChangedValues();
	output(refCurrent.toString());
	mainWnd->setActionStatus(true, false, true, false, true, true);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
CapitalizeCommand::CapitalizeCommand(MainWindow* mainWindow, QUndoCommand* parent)
	: Command(mainWindow, parent)
{
	setText("Capitalize");
}

void CapitalizeCommand::undo()
{
	Command::undo();
	mainWnd->setActionStatus(true, false, true, false, true, true);
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
	mainWnd->setActionStatus(true, false, false, true, true, true);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
ProtectCommand::ProtectCommand(MainWindow* mainWindow, QUndoCommand* parent)
	: Command(mainWindow, parent)
{
	setText("Protect");
}

void ProtectCommand::undo()
{
	Command::undo();
	mainWnd->setActionStatus(true, false, false, true, true, true);
}

void ProtectCommand::redo()
{
    refBackup = refCurrent;
    refCurrent.clearChangedValues();
    refCurrent.protect("title");
	refCurrent.setHighlightColor(Qt::cyan);
	output(refCurrent.toString());
    highlightChanged();
	mainWnd->setActionStatus(true, false, false, false, true, true);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
AbbreviateCommand::AbbreviateCommand(MainWindow* mainWindow, QUndoCommand* parent)
	: Command(mainWindow, parent)
{
	setText("Abbreviate");
}

void AbbreviateCommand::undo()
{
	Command::undo();
	mainWnd->setActionStatus(true, false, false, false, true, true);
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
	mainWnd->setActionStatus(true, false, true, false, false, true);
}
