#include "Commands.h"
#include "Reference.h"
#include "TextEdit.h"

Command::Command(ReferenceList* references, TextEdit* edit, QUndoCommand* parent)
    : referenceList(references), textEdit(edit), QUndoCommand(parent)
{
    originalText = edit->toPlainText();
}

void Command::highlightChanged()
{
    QStringList toBeHighlighted = referenceList->getChangedValues();
    foreach(const QString& line, toBeHighlighted)
        textEdit->addHighlightedLine(line, Qt::yellow);
    textEdit->highlightLines();
}

//////////////////////////////////////////////////////////////////////////////////////////
CleanCommand::CleanCommand(ReferenceList* references, TextEdit* edit, QUndoCommand* parent)
    : Command(references, edit, parent)
{
    originalText = edit->toPlainText();
}

void CleanCommand::undo() {
    textEdit->setPlainText(originalText);
}

void CleanCommand::redo()
{
    textEdit->setPlainText(referenceList->toString());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
CapitalizeCommand::CapitalizeCommand(ReferenceList* references, TextEdit* edit, QUndoCommand* parent)
    : Command(references, edit, parent)
{
    originalText = edit->toPlainText();
}

void CapitalizeCommand::undo() {
    textEdit->setPlainText(originalText);
}

void CapitalizeCommand::redo()
{
    referenceList->capitalize("title");
    referenceList->capitalize("journal");
    referenceList->capitalize("booktitle");
    textEdit->setPlainText(referenceList->toString());
    highlightChanged();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
ProtectCommand::ProtectCommand(ReferenceList* references, TextEdit* edit, QUndoCommand* parent)
    : Command(references, edit, parent)
{
    originalText = edit->toPlainText();
}

void ProtectCommand::undo() {
    textEdit->setPlainText(originalText);
}

void ProtectCommand::redo()
{
    referenceList->clearChangedValues();
    referenceList->protect("title");
    textEdit->setPlainText(referenceList->toString());
    highlightChanged();
}

