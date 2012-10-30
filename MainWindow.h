#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include <QUndoStack>

namespace BibFixer {

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    typedef enum {Init, Open, Clean, Capitalize, Protect,
                  Abbreviate, GenerateKeys, RunAll, Save, Size} ActionName;
public:
	MainWindow(QWidget* parent = 0);
	TextEdit* getTextEdit() const { return ui.teOutput; }

    void setTriggered(ActionName actionName, bool triggered);  // set action triggered
    void openBibFile(const QString& filePath);                 // to be called by external program

private slots:
	void onNewFile();
	void onOpen();
    void onPaste();
	void onClean();
    void onCapitalize();
	void onProtect();
	void onAbbreviate();
    void onGenerateKeys();
    void onRunAll();
	void onSave();
	void onSettings();
	void onAbout();

private:
    void resetTriggered();
    void createActions();

    bool canOpen()         const { return _triggered[Init]; }
    bool canClean()        const { return _triggered[Open]       && !_triggered[Clean];        }
    bool canCapitalize()   const { return _triggered[Clean]      && !_triggered[Capitalize];   }
    bool canProtect()      const { return _triggered[Capitalize] && !_triggered[Protect];      }
    bool canAbbreviate()   const { return _triggered[Clean]      && !_triggered[Abbreviate];   }
    bool canGenerateKeys() const { return _triggered[Clean]      && !_triggered[GenerateKeys]; }
    bool canSave()         const { return _triggered[Open]; }
    bool canRunAll()       const { return _triggered[Open] && !_triggered[RunAll] &&
                                        (!_triggered[Capitalize] ||
                                         !_triggered[Protect]    ||
                                         !_triggered[Abbreviate] ||
                                         !_triggered[GenerateKeys]); }
    bool isReadOnly() const { return _triggered[Clean]; }

private:
	Ui::MainWindow ui;
    QUndoStack _undoStack;
    bool       _triggered[Size];   // triggered[i]: if this action has been triggered
                                   // not using states because enabling some actions
                                   // cannot be determined by single state
};

}

#endif // MAINWINDOW_H
