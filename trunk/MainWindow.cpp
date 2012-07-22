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

    setActionStatus(Init,        true);
    setActionStatus(Opened,      false);
    setActionStatus(Cleaned,     false);
    setActionStatus(Capitalized, false);
    setActionStatus(Protected,   false);
    setActionStatus(Abbreviated, false);
    setActionStatus(RunAll,      false);
    setActionStatus(Save,        false);

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

void MainWindow::setActionStatus(MainWindow::ActionStatus status, bool value)
{
    actionStatuses[status] = value;
    ui.actionOpen      ->setEnabled(actionStatuses[Init]);
    ui.actionClean     ->setEnabled(actionStatuses[Opened]      && !actionStatuses[Cleaned]);
    ui.actionCapitalize->setEnabled(actionStatuses[Cleaned]     && !actionStatuses[Capitalized]);
    ui.actionProtect   ->setEnabled(actionStatuses[Capitalized] && !actionStatuses[Protected]);
    ui.actionAbbreviate->setEnabled(actionStatuses[Cleaned]     && !actionStatuses[Abbreviated]);
    ui.actionSave      ->setEnabled(actionStatuses[Opened]);
    ui.actionRunAll    ->setEnabled(actionStatuses[Opened] && !actionStatuses[RunAll] &&
                                    (!actionStatuses[Capitalized] ||
                                     !actionStatuses[Protected]   ||
                                     !actionStatuses[Abbreviated]));
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
        setActionStatus(Opened, true);
	}
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
        os << ui.teOutput->toPlainText();
	}
}

void MainWindow::onSettings()
{
    DlgSettings dlg(this);
    if(dlg.exec() == QDialog::Accepted)
        ui.teOutput->setFont(UserSetting::getInstance()->getFont());
}

void MainWindow::onClean() {
    undoStack.push(new CleanCommand(this));
}

void MainWindow::onCapitalize() {
    undoStack.push(new CapitalizeCommand(this));
}

void MainWindow::onProtect() {
    undoStack.push(new ProtectCommand(this));
}

void MainWindow::onAbbreviate() {
    undoStack.push(new AbbreviateCommand(this));
}

void MainWindow::onRunAll()
{
    onClean();
    onCapitalize();
    onProtect();
    onAbbreviate();
}

void MainWindow::onAbout() {
	QMessageBox::about(this, "About",
		tr("<h3><b>BibFixer: Fixing BibTex files</b></h3>"
		   "<p>Built on %1</p>"
		   "<p><a href=mailto:CongChenUTD@Gmail.com>CongChenUTD@Gmail.com</a></p>")
					   .arg(UserSetting::getInstance()->getCompileDate()));

}
