#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include <QUndoStack>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
    typedef enum {Init, Open, Clean, Capitalize, Protect,
                  Abbreviate, GenerateKey, RunAll, Save, Size} ActionName;
public:
	MainWindow(QWidget* parent = 0);
	TextEdit* getTextEdit() const { return ui.teOutput; }

    void setActionStatus(ActionName status, bool value);
    void open(const QString& filePath);   // called by external program

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
    void initActionStatuses();
    void createActions();

    bool isOpenEnabled()         const { return actionStatuses[Init]; }
    bool isCleanEnabled()        const { return actionStatuses[Open]       && !actionStatuses[Clean];       }
    bool isCapitalizeEnabled()   const { return actionStatuses[Clean]      && !actionStatuses[Capitalize];   }
    bool isProtectEnabled()      const { return actionStatuses[Capitalize] && !actionStatuses[Protect];     }
    bool isAbbreviateEnabled()   const { return actionStatuses[Clean]      && !actionStatuses[Abbreviate];   }
    bool isGenerateKeysEnabled() const { return actionStatuses[Clean]      && !actionStatuses[GenerateKey]; }
    bool isSaveEnabled()         const { return actionStatuses[Open]; }
    bool isRunAllEnabled()       const { return actionStatuses[Open] && !actionStatuses[RunAll] &&
                                        (!actionStatuses[Capitalize] ||
                                         !actionStatuses[Protect]    ||
                                         !actionStatuses[Abbreviate] ||
                                         !actionStatuses[GenerateKey]); }

private:
	Ui::MainWindow ui;
	QUndoStack undoStack;
    bool actionStatuses[Size];
};

#endif // MAINWINDOW_H
