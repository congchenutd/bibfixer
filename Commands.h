#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include "Reference.h"

class MainWindow;
class TextEdit;

class Command : public QUndoCommand
{
public:
	Command(MainWindow* mainWindow, QUndoCommand* parent = 0);
	virtual ~Command() {}
    void highlightChanged();
	void undo();

protected:
	void output(const QString& text);

protected:
	ReferenceList refBackup;           // each command object keeps a version
	MainWindow*   mainWnd;

	static ReferenceList refCurrent;   // current version
};

class CleanCommand : public Command
{
public:
    CleanCommand(MainWindow* mainWindow, QUndoCommand* parent = 0);

	void undo();
    void redo();

private:
	QString originalText;
};

class CapitalizeCommand : public Command
{
public:
	CapitalizeCommand(MainWindow* mainWindow, QUndoCommand* parent = 0);

	void undo();
    void redo();
};

class ProtectCommand : public Command
{
public:
	ProtectCommand(MainWindow* mainWindow, QUndoCommand* parent = 0);

	void undo();
    void redo();
};

class AbbreviateCommand : public Command
{
public:
	AbbreviateCommand(MainWindow* mainWindow, QUndoCommand* parent = 0);

	void undo();
	void redo();
};

#endif // COMMANDS_H
