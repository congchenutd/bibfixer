#ifndef COMMANDS_H
#define COMMANDS_H

#include "Reference.h"
#include "MainWindow.h"

namespace BibFixer {

class TextEdit;

// Undo/redo commands (operations)
class ICommand
{
public:
    virtual ~ICommand() {}
    virtual void undo() = 0;
    virtual void redo() = 0;
    virtual MainWindow::ActionName getActionName()     const = 0; // associated action
    virtual QColor                 getHighlightColor() const = 0; // for highlighting the changes
};

class AbstractCommand : public QObject, public ICommand
{
public:
    AbstractCommand(MainWindow* mainWindow = 0);
    virtual void undo();
    virtual void redo();

protected:
	void highlight();
	void output(const QString& text);

protected:
    MainWindow* _mainWnd;

    static ReferenceList _reference;  // the reference all the commands work on
};

class CleanCommand : public AbstractCommand
{
public:
    CleanCommand(MainWindow* mainWindow);
    void undo();
    void redo();

    // must set original text before redo is called
    static void setOriginalText(const QString& originalText) { _originalText = originalText; }

protected:
    MainWindow::ActionName getActionName()     const { return MainWindow::Clean; }
    QColor                 getHighlightColor() const { return Qt::yellow; }

protected:
    static QString _originalText;
};

class CapitalizeCommand : public AbstractCommand
{
public:
    CapitalizeCommand(MainWindow* mainWindow);
    void undo();
    void redo();

protected:
    MainWindow::ActionName getActionName()     const { return MainWindow::Capitalize; }
    QColor                 getHighlightColor() const { return Qt::yellow; }
    void run(IConvertor* convertor);
};

class ProtectCommand : public AbstractCommand
{
public:
    ProtectCommand(MainWindow* mainWindow);
    void undo();
    void redo();

protected:
    MainWindow::ActionName getActionName()     const { return MainWindow::Protect; }
    QColor                 getHighlightColor() const { return Qt::cyan; }
    void run(IConvertor* convertor);
};

class AbbreviateCommand : public AbstractCommand
{
public:
    AbbreviateCommand(MainWindow* mainWindow);
    void undo();
    void redo();

protected:
    MainWindow::ActionName getActionName()     const { return MainWindow::Abbreviate; }
    QColor                 getHighlightColor() const { return Qt::green; }
    void run(IConvertor* convertor);

private:
    QStringList _rules;
};

class GenerateKeysCommand : public AbstractCommand
{
public:
    GenerateKeysCommand(MainWindow* mainWindow);
    void undo() {}
    void redo();

protected:
    MainWindow::ActionName getActionName()     const { return MainWindow::GenerateKeys; }
    QColor                 getHighlightColor() const { return Qt::magenta; }
};

}

#endif // COMMANDS_H
