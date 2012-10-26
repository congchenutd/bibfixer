#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include "Reference.h"
#include "MainWindow.h"

namespace BibFixer {

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
	void highlight();
	void output(const QString& text);

    virtual MainWindow::ActionName getActionName() const = 0;  // associated action
	virtual QColor getHighlightColor() const = 0;              // for highlighting the change
	virtual void runCommand() = 0;                             // template method

protected:
	ReferenceList backupSnapshot;           // backup for retoring highlighting in undo
    MainWindow*   mainWnd;

	static ReferenceList currentSnapshot;   // current version
};

class CleanCommand : public Command
{
public:
    CleanCommand(MainWindow* mainWindow, QUndoCommand* parent = 0);
	void undo();

protected:
    MainWindow::ActionName getActionName() const { return MainWindow::Clean; }
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
    MainWindow::ActionName getActionName() const { return MainWindow::Capitalize; }
	QColor getHighlightColor() const { return Qt::yellow; }
	void runCommand();
};

class ProtectCommand : public Command
{
public:
    ProtectCommand(MainWindow* mainWindow, QUndoCommand* parent = 0);

protected:
    MainWindow::ActionName getActionName() const { return MainWindow::Protect; }
	QColor getHighlightColor() const { return Qt::cyan; }
	void runCommand();
};

class AbbreviateCommand : public Command
{
public:
    AbbreviateCommand(MainWindow* mainWindow, QUndoCommand* parent = 0);

protected:
    MainWindow::ActionName getActionName() const { return MainWindow::Abbreviate; }
	QColor getHighlightColor() const { return Qt::green; }
	void runCommand();
};

class GenerateKeysCommand : public Command
{
public:
    GenerateKeysCommand(MainWindow* mainWindow, QUndoCommand* parent = 0);

protected:
    MainWindow::ActionName getActionName() const { return MainWindow::GenerateKey; }
	QColor getHighlightColor() const { return Qt::magenta; }
	void runCommand();
};

}

#endif // COMMANDS_H
