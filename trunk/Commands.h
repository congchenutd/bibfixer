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

    // run the convertor on the fields of _reference
    void run(IConvertor* convertor, const QStringList& fields);

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

protected:
    MainWindow::ActionName getActionName()     const { return MainWindow::Clean; }
    QColor                 getHighlightColor() const { return Qt::yellow; }

private:
    static QString _originalText;  // for backup
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

private:
    QStringList _rules;
};

class ShortenNamesCommand : public AbstractCommand
{
public:
    ShortenNamesCommand(MainWindow* mainWindow);
    void undo();
    void redo();

protected:
    MainWindow::ActionName getActionName()     const { return MainWindow::ShortenNames; }
    QColor                 getHighlightColor() const { return Qt::lightGray; }
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
