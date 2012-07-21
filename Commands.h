#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QColor>
#include "Reference.h"

class ReferenceList;
class TextEdit;

class Command : public QUndoCommand
{
public:
	Command(ReferenceList* refs, TextEdit* textEdit, QUndoCommand* parent = 0);
	void highlightChanged(const QColor& color);
	void undo();

protected:
	ReferenceList* refCurrent;
	ReferenceList  refBackup;
	TextEdit*      edit;
};

class CleanCommand : public Command
{
public:
	CleanCommand(const QString& text, ReferenceList* refs, TextEdit* edit, QUndoCommand* parent = 0);

	void undo();
    void redo();

private:
	QString originalText;
};

class CapitalizeCommand : public Command
{
public:
	CapitalizeCommand(ReferenceList* refs, TextEdit* edit, QUndoCommand* parent = 0);

    void redo();
};

class ProtectCommand : public Command
{
public:
	ProtectCommand(ReferenceList* refs, TextEdit* edit, QUndoCommand* parent = 0);

    void redo();
};

class AbbreviateCommand : public Command
{
public:
	AbbreviateCommand(ReferenceList* refs, TextEdit* edit, QUndoCommand* parent = 0);

	void redo();
};

#endif // COMMANDS_H
