#include "Commands.h"
#include "TextEdit.h"
#include "Parser.h"

Command::Command(TextEdit* textEdit, QUndoCommand* parent)
    : QUndoCommand(parent), edit(textEdit) {}

void Command::undo()
{
    refCurrent = refBackup;
    edit->setPlainText(refCurrent.toString());
    highlightChanged();
}

void Command::highlightChanged()
{
    edit->unHighlightLines();
    QStringList toBeHighlighted = refCurrent.getChangedValues();
    foreach(const QString& line, toBeHighlighted)
        edit->addHighlightedLine(line, refCurrent.getHighlightColor());
	edit->highlightLines();
}

ReferenceList Command::refCurrent;


//////////////////////////////////////////////////////////////////////////////////////////
CleanCommand::CleanCommand(const QString& text, TextEdit* edit, QUndoCommand* parent)
    : Command(edit, parent), originalText(text) {}

void CleanCommand::undo() {
    edit->setPlainText(originalText);
}

void CleanCommand::redo()
{
	BibParser parser;
    refCurrent = parser.parse(originalText);
    refCurrent.clearChangedValues();
    edit->setPlainText(refCurrent.toString());
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
CapitalizeCommand::CapitalizeCommand(TextEdit* edit, QUndoCommand* parent)
    : Command(edit, parent) {}

void CapitalizeCommand::redo()
{
    refBackup = refCurrent;
    refCurrent.clearChangedValues();
    refCurrent.capitalize("title");
    refCurrent.capitalize("journal");
    refCurrent.capitalize("booktitle");
    refCurrent.setHighlightColor(Qt::yellow);
    edit->setPlainText(refCurrent.toString());
    highlightChanged();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
ProtectCommand::ProtectCommand(TextEdit* edit, QUndoCommand* parent)
    : Command(edit, parent) {}

void ProtectCommand::redo()
{
    refBackup = refCurrent;
    refCurrent.clearChangedValues();
    refCurrent.protect("title");
    refCurrent.setHighlightColor(Qt::yellow);
    edit->setPlainText(refCurrent.toString());
    highlightChanged();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
AbbreviateCommand::AbbreviateCommand(TextEdit* edit, QUndoCommand* parent)
    : Command(edit, parent) {}

void AbbreviateCommand::redo()
{
    refBackup = refCurrent;
    refCurrent.clearChangedValues();
    refCurrent.abbreviate("journal");
    refCurrent.abbreviate("booktitle");
    refCurrent.setHighlightColor(Qt::green);
    edit->setPlainText(refCurrent.toString());
    highlightChanged();
}
