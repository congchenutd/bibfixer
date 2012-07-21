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
    Command(TextEdit* textEdit, QUndoCommand* parent = 0);
    void highlightChanged();
	void undo();

protected:
    ReferenceList refBackup;
    TextEdit*     edit;

    static ReferenceList refCurrent;
};

class CleanCommand : public Command
{
public:
    CleanCommand(const QString& text, TextEdit* edit, QUndoCommand* parent = 0);

	void undo();
    void redo();

private:
	QString originalText;
};

class CapitalizeCommand : public Command
{
public:
    CapitalizeCommand(TextEdit* edit, QUndoCommand* parent = 0);

    void redo();
};

class ProtectCommand : public Command
{
public:
    ProtectCommand(TextEdit* edit, QUndoCommand* parent = 0);

    void redo();
};

class AbbreviateCommand : public Command
{
public:
    AbbreviateCommand(TextEdit* edit, QUndoCommand* parent = 0);

	void redo();
};

#endif // COMMANDS_H
