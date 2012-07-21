#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>

class ReferenceList;
class TextEdit;

class Command : public QUndoCommand
{
public:
    Command(ReferenceList* references, TextEdit* edit, QUndoCommand* parent = 0);
    void highlightChanged();

protected:
    QString        originalText;
    ReferenceList* referenceList;
    TextEdit*      textEdit;
};

class CleanCommand : public Command
{
public:
    CleanCommand(ReferenceList* references, TextEdit* edit, QUndoCommand* parent = 0);

    void undo();
    void redo();
};

class CapitalizeCommand : public Command
{
public:
    CapitalizeCommand(ReferenceList* references, TextEdit* edit, QUndoCommand* parent = 0);

    void undo();
    void redo();
};

class ProtectCommand : public Command
{
public:
    ProtectCommand(ReferenceList* references, TextEdit* edit, QUndoCommand* parent = 0);

    void undo();
    void redo();
};

#endif // COMMANDS_H
