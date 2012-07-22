#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "Reference.h"
#include "Commands.h"
#include <QUndoStack>

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = 0);
	TextEdit* getTextEdit() const { return ui.teOutput; }

    typedef enum {Init, Opened, Cleaned, Capitalized, Protected, Abbreviated, RunAll, Save} ActionStatus;
    void setActionStatus(ActionStatus status, bool value);

private slots:
	void onOpen();
	void onClean();
    void onCapitalize();
	void onProtect();
	void onAbbreviate();
    void onRunAll();
	void onSave();
	void onSettings();
	void onAbout();

private:
    void createActions();

private:
	Ui::MainWindow ui;
	QUndoStack undoStack;
    bool actionStatuses[Save+1];
};

#endif // MAINWINDOW_H
