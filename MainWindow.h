#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

namespace BibFixer {

class AbstractCommand;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    typedef enum {Open, Save, RunAll, Clean, Capitalize, Protect,
                  Abbreviate, ShortenNames, GenerateKeys, Init, Size} ActionName;
public:
	MainWindow(QWidget* parent = 0);
	TextEdit* getTextEdit() const { return ui.teOutput; }

    // called when a command is executed
    void updateActionStatus(ActionName actionName, bool triggered);

public slots:
    void openBibFile(const QString& filePath);     // called by external program

private slots:
	void onNewFile();
	void onOpen();
    void onPaste();
    void onClean       (bool redo = true);
    void onCapitalize  (bool redo = true);
    void onProtect     (bool redo = true);
    void onAbbreviate  (bool redo = true);
    void onShortenNames();
    void onGenerateKeys();
    void onRunAll();
	void onSave();
	void onSettings();
	void onAbout();

private:
    void resetActionStatus();
    void initActions();
    void setActionTriggered(ActionName actionName, bool triggered);
    bool isTriggered(ActionName actionName) const { return _triggered[actionName]; }

    bool canRunAll() const { return _triggered[Open] &&
                                    (!_triggered[Clean]      ||
                                     !_triggered[Capitalize] ||
                                     !_triggered[Protect]    ||
                                     !_triggered[Abbreviate] ||
                                     !_triggered[GenerateKeys]); }

    void runCommand(ActionName actionName, bool redo, AbstractCommand* command);
    QString getContent() const;

private:
	Ui::MainWindow ui;
    bool _triggered[Size];   // triggered[i]: if this action has been triggered
                             // not using states because enabling some actions
                             // cannot be determined by single state
                             // synced with action's checked() state
    QList<QAction*> _actions;
};

} // namespace BibFixer

#endif // MAINWINDOW_H
