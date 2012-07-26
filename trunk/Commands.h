#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include "Reference.h"
#include "MainWindow.h"

class TextEdit;

// Undo/redo commands (operations)
class Command : public QUndoCommand
{
public:
	Command(MainWindow* mainWindow, QUndoCommand* parent = 0);
	virtual ~Command() {}
	void undo();
	void redo();

protected:
	void highlightChanged();
	void output(const QString& text);

	virtual MainWindow::ActionStatus getActionStatus() const = 0;
	virtual QColor getHighlightColor() const = 0;
	virtual void runCommand() = 0;     // a template method for specific operation

protected:
	ReferenceList refBackup;           // each command object backups a version (for highlighting)
	MainWindow*   mainWnd;

	static ReferenceList refCurrent;   // current version
};

class CleanCommand : public Command
{
public:
    CleanCommand(MainWindow* mainWindow, QUndoCommand* parent = 0);
	void undo();

protected:
	MainWindow::ActionStatus getActionStatus() const { return MainWindow::Cleaned; }
	QColor getHighlightColor() const { return Qt::yellow; }
	void runCommand();

private:
	QString originalText;
};

class CapitalizeCommand : public Command
{
public:
	CapitalizeCommand(MainWindow* mainWindow, QUndoCommand* parent = 0);

protected:
	MainWindow::ActionStatus getActionStatus() const { return MainWindow::Capitalized; }
	QColor getHighlightColor() const { return Qt::yellow; }
	void runCommand();
};

class ProtectCommand : public Command
{
public:
	ProtectCommand(MainWindow* mainWindow, QUndoCommand* parent = 0);

protected:
	MainWindow::ActionStatus getActionStatus() const { return MainWindow::Protected; }
	QColor getHighlightColor() const { return Qt::cyan; }
	void runCommand();
};

class AbbreviateCommand : public Command
{
public:
	AbbreviateCommand(MainWindow* mainWindow, QUndoCommand* parent = 0);

protected:
	MainWindow::ActionStatus getActionStatus() const { return MainWindow::Abbreviated; }
	QColor getHighlightColor() const { return Qt::green; }
	void runCommand();
};

class GenerateKeysCommand : public Command
{
public:
    GenerateKeysCommand(MainWindow* mainWindow, QUndoCommand* parent = 0);

protected:
	MainWindow::ActionStatus getActionStatus() const { return MainWindow::KeysGenerated; }
	QColor getHighlightColor() const { return Qt::magenta; }
	void runCommand();
};

#endif // COMMANDS_H
