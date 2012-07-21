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
	void setActionStatus(bool open, bool clean, bool capitalize, bool protect, bool abbreviate, bool save);
	TextEdit* getTextEdit() const { return ui.teOutput; }

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

private:
	Ui::MainWindow ui;
	QUndoStack undoStack;
};

#endif // MAINWINDOW_H
