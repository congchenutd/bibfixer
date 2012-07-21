#include "MainWindow.h"
#include "DlgSettings.h"
#include "WidgetAbbreviation.h"
#include "Parser.h"
#include "Commands.h"
#include <QScrollBar>
#include <QStyle>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
    createActions();
	ui.teOutput->setFont(UserSetting::getInstance()->getFont());
	updateButtons(INIT);

	connect(ui.actionOpen,       SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui.actionSave,       SIGNAL(triggered()), this, SLOT(onSave()));
	connect(ui.actionSettings,   SIGNAL(triggered()), this, SLOT(onSettings()));
	connect(ui.actionRunAll,     SIGNAL(triggered()), this, SLOT(onRunAll()));
	connect(ui.actionClean,      SIGNAL(triggered()), this, SLOT(onClean()));
    connect(ui.actionCapitalize, SIGNAL(triggered()), this, SLOT(onCapitalize()));
	connect(ui.actionProtect,    SIGNAL(triggered()), this, SLOT(onProtect()));
	connect(ui.actionAbbreviate, SIGNAL(triggered()), this, SLOT(onAbbreviate()));
	connect(ui.actionAbout,      SIGNAL(triggered()), this, SLOT(onAbout()));
}

void MainWindow::createActions()
{
    ui.actionOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));

    QAction* undoAction = undoStack.createUndoAction(this, tr("Undo"));
    undoAction->setIcon(QIcon(":/Images/Undo.png"));
    undoAction->setShortcuts(QKeySequence::Undo);
    ui.mainToolBar->insertAction(ui.actionSettings, undoAction);

    QAction* redoAction = undoStack.createRedoAction(this, tr("Redo"));
    redoAction->setIcon(QIcon(":/Images/Redo.png"));
    redoAction->setShortcuts(QKeySequence::Redo);
    ui.mainToolBar->insertAction(ui.actionSettings, redoAction);
    ui.mainToolBar->insertSeparator(ui.actionSettings);
}

void MainWindow::onOpen()
{
	QString inputFileName = QFileDialog::getOpenFileName(this,
		tr("Open Bibtex file"), ".", tr("Bibtex (*.bib);;All files (*.*)"));
	if(inputFileName.isEmpty())
		return;

	QFile file(inputFileName);
	if(file.open(QFile::ReadOnly))
	{
		ui.teOutput->setPlainText(file.readAll());
		updateButtons(OPEN);
	}
}

void MainWindow::onRunAll()
{
	onClean();
    onCapitalize();
	onProtect();
	onAbbreviate();
}

void MainWindow::onSave()
{
	if(ui.teOutput->toPlainText().isEmpty())
		return;

	QString outputFileName = QFileDialog::getSaveFileName(this,
		tr("Save Bibtex file"), ".", tr("Bibtex (*.bib);;All files (*.*)"));
	if(outputFileName.isEmpty())
		return;

	QFile file(outputFileName);
	if(file.open(QFile::WriteOnly))
	{
		QTextStream os(&file);
		os << getContent();
	}
}

void MainWindow::onSettings()
{
    DlgSettings dlg(this);
    if(dlg.exec() == QDialog::Accepted)
        ui.teOutput->setFont(UserSetting::getInstance()->getFont());
}

void MainWindow::onClean()
{
	CleanCommand* cleanCommand = new CleanCommand(getContent(), &references, ui.teOutput);
    undoStack.push(cleanCommand);
	updateButtons(CLEAN);
}

void MainWindow::onCapitalize()
{
    CapitalizeCommand* capitalizeCommand = new CapitalizeCommand(&references, ui.teOutput);
    undoStack.push(capitalizeCommand);
    updateButtons(CAPITALIZE);
}

void MainWindow::onProtect()
{
    ProtectCommand* protectCommand = new ProtectCommand(&references, ui.teOutput);
    undoStack.push(protectCommand);
    updateButtons(PROTECT);
}

void MainWindow::onAbbreviate()
{
	AbbreviateCommand* abbreviateCommand = new AbbreviateCommand(&references, ui.teOutput);
	undoStack.push(abbreviateCommand);
    updateButtons(ABBREVIATE);
}

void MainWindow::onAbout() {
	QMessageBox::about(this, tr("About"),
					   tr("<h3><b>BibFixer: Fixing BibTex files</b></h3>"
						  "2012/1/4"
						  "<p><a href=mailto:CongChenUTD@Gmail.com>CongChenUTD@Gmail.com</a></p>"));
}

QString MainWindow::getContent() const {
	return ui.teOutput->toPlainText();
}

void MainWindow::updateOutput() {
	ui.teOutput->setPlainText(references.toString());
}

void MainWindow::updateButtons(OperationStatus status)
{
	ui.actionRunAll    ->setEnabled(status == OPEN);
	ui.actionClean     ->setEnabled(status == OPEN);
	ui.actionCapitalize->setEnabled(status >= CLEAN);
	ui.actionProtect   ->setEnabled(status >= CLEAN);
	ui.actionAbbreviate->setEnabled(status >= CLEAN);
    ui.actionSave      ->setEnabled(status >= CLEAN);

//    ui.actionCapitalize->setChecked(!canCapitalize());
}
