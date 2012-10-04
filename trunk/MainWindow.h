#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "Reference.h"
#include <QUndoStack>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	TextEdit* getTextEdit() const { return ui.teOutput; }

    typedef enum {Init, Opened, Cleaned, Capitalized, Protected,
                  Abbreviated, KeysGenerated, RunAll, Save} ActionStatus;
    void setActionStatus(ActionStatus status, bool value);

    void open(const QString& filePath);

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
    bool isCleanEnabled()        const { return actionStatuses[Opened]      && !actionStatuses[Cleaned];       }
    bool isCapitalizeEnabled()   const { return actionStatuses[Cleaned]     && !actionStatuses[Capitalized];   }
    bool isProtectEnabled()      const { return actionStatuses[Capitalized] && !actionStatuses[Protected];     }
    bool isAbbreviateEnabled()   const { return actionStatuses[Cleaned]     && !actionStatuses[Abbreviated];   }
    bool isGenerateKeysEnabled() const { return actionStatuses[Cleaned]     && !actionStatuses[KeysGenerated]; }
    bool isSaveEnabled()         const { return actionStatuses[Opened]; }
    bool isRunAllEnabled()       const { return actionStatuses[Opened] && !actionStatuses[RunAll] &&
                                        (!actionStatuses[Capitalized] ||
                                         !actionStatuses[Protected]   ||
                                         !actionStatuses[Abbreviated] ||
                                         !actionStatuses[KeysGenerated]); }

private:
	Ui::MainWindow ui;
	QUndoStack undoStack;
    bool actionStatuses[Save+1];
};

#endif // MAINWINDOW_H
