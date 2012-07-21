#include "Commands.h"
#include "TextEdit.h"
#include "Parser.h"

Command::Command(ReferenceList* refs, TextEdit* textEdit, QUndoCommand* parent)
	: QUndoCommand(parent), refCurrent(refs), edit(textEdit) {}

void Command::undo()
{
	*refCurrent = refBackup;
	edit->setPlainText(refCurrent->toString());
}

void Command::highlightChanged(const QColor& color)
{
	QStringList toBeHighlighted = refCurrent->getChangedValues();
    foreach(const QString& line, toBeHighlighted)
		edit->addHighlightedLine(line, color);
	edit->highlightLines();
}

//////////////////////////////////////////////////////////////////////////////////////////
CleanCommand::CleanCommand(const QString& text, ReferenceList* refs, TextEdit* edit, QUndoCommand* parent)
	: Command(refs, edit, parent), originalText(text) {}

void CleanCommand::undo() {
	edit->setPlainText(originalText);
}

void CleanCommand::redo()
{
	BibParser parser;
	*refCurrent = parser.parse(originalText);
	edit->setPlainText(refCurrent->toString());
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
CapitalizeCommand::CapitalizeCommand(ReferenceList* refs, TextEdit* edit, QUndoCommand* parent)
	: Command(refs, edit, parent) {}

void CapitalizeCommand::redo()
{
	refBackup = *refCurrent;
	refCurrent->capitalize("title");
	refCurrent->capitalize("journal");
	refCurrent->capitalize("booktitle");
	edit->setPlainText(refCurrent->toString());
	highlightChanged(Qt::yellow);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
ProtectCommand::ProtectCommand(ReferenceList* refs, TextEdit* edit, QUndoCommand* parent)
	: Command(refs, edit, parent) {}

void ProtectCommand::redo()
{
	refBackup = *refCurrent;
	refCurrent->clearChangedValues();
	refCurrent->protect("title");
	edit->setPlainText(refCurrent->toString());
	highlightChanged(Qt::yellow);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
AbbreviateCommand::AbbreviateCommand(ReferenceList* refs, TextEdit* edit, QUndoCommand* parent)
	: Command(refs, edit, parent) {}

void AbbreviateCommand::redo()
{
	refBackup = *refCurrent;
	refCurrent->clearChangedValues();
	refCurrent->abbreviate("journal");
	refCurrent->abbreviate("booktitle");
	edit->setPlainText(refCurrent->toString());
	highlightChanged(Qt::green);
}
