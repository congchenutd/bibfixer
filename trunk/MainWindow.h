#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "Reference.h"
#include <QUndoStack>

class MainWindow : public QMainWindow
{
	Q_OBJECT
	typedef enum {INIT, OPEN, CLEAN, CAPITALIZE, PROTECT, ABBREVIATE} OperationStatus;

public:
	MainWindow(QWidget* parent = 0);

private slots:
	void onOpen();
	void onRunAll();
	void onClean();
    void onCapitalize();
	void onProtect();
	void onAbbreviate();
	void onSave();
	void onSettings();
	void onAbout();

private:
    void createActions();
	QString getContent() const;
	void updateOutput();
	void updateButtons(OperationStatus status);

private:
	Ui::MainWindow ui;
	ReferenceList references;
    QUndoStack undoStack;
};

#endif // MAINWINDOW_H
