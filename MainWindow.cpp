#include "MainWindow.h"
#include "DlgSettings.h"
#include "Commands.h"
#include <QStyle>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

namespace BibFixer {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

    createActions();
    resetTriggered();   // init action status
	ui.teOutput->setFont(Setting::getInstance()->getFont());

    connect(ui.actionNew,          SIGNAL(triggered()), this, SLOT(onNewFile()));
    connect(ui.actionOpen,         SIGNAL(triggered()), this, SLOT(onOpen()));
    connect(ui.actionSave,         SIGNAL(triggered()), this, SLOT(onSave()));
    connect(ui.actionSettings,     SIGNAL(triggered()), this, SLOT(onSettings()));
    connect(ui.actionRunAll,       SIGNAL(triggered()), this, SLOT(onRunAll()));
    connect(ui.actionClean,        SIGNAL(triggered()), this, SLOT(onClean()));
    connect(ui.actionCapitalize,   SIGNAL(triggered()), this, SLOT(onCapitalize()));
    connect(ui.actionProtect,      SIGNAL(triggered()), this, SLOT(onProtect()));
    connect(ui.actionAbbreviate,   SIGNAL(triggered()), this, SLOT(onAbbreviate()));
    connect(ui.actionGenerateKeys, SIGNAL(triggered()), this, SLOT(onGenerateKeys()));
    connect(ui.actionAbout,        SIGNAL(triggered()), this, SLOT(onAbout()));
    connect(ui.teOutput,           SIGNAL(pasted()),    this, SLOT(onPaste()));
}

void MainWindow::resetTriggered()
{
    // initial statuses
    setTriggered(Init,         true);
    setTriggered(Open,         false);
    setTriggered(Clean,        false);
    setTriggered(Capitalize,   false);
    setTriggered(Protect,      false);
    setTriggered(Abbreviate,   false);
    setTriggered(GenerateKeys, false);
    setTriggered(RunAll,       false);
    setTriggered(Save,         false);
}

void MainWindow::setTriggered(MainWindow::ActionName actionName, bool triggered)
{
    _triggered[actionName] = triggered;
    ui.actionOpen        ->setEnabled(canOpen());
//    ui.actionClean       ->setEnabled(canClean());
    ui.actionCapitalize  ->setEnabled(canCapitalize());
    ui.actionProtect     ->setEnabled(canProtect());
    ui.actionAbbreviate  ->setEnabled(canAbbreviate());
    ui.actionGenerateKeys->setEnabled(canGenerateKeys());
    ui.actionSave        ->setEnabled(canSave());
    ui.actionRunAll      ->setEnabled(canRunAll());
}

void MainWindow::createActions()
{
    ui.actionOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));

    QAction* undoAction = _undoStack.createUndoAction(this, tr("Undo"));
    undoAction->setIcon(QIcon(":/Images/Undo.png"));
    undoAction->setShortcuts(QKeySequence::Undo);
    ui.mainToolBar->insertAction(0, undoAction);  // append to the toolbar

    QAction* redoAction = _undoStack.createRedoAction(this, tr("Redo"));
    redoAction->setIcon(QIcon(":/Images/Redo.png"));
    redoAction->setShortcuts(QKeySequence::Redo);
    ui.mainToolBar->insertAction(0, redoAction);  // append to the toolbar
}

void MainWindow::openBibFile(const QString& filePath)
{
    if(filePath.isEmpty())
        return;

    if(QFileInfo(filePath).isFile())
    {
        QFile file(filePath);
        if(file.open(QFile::ReadOnly))
            ui.teOutput->setPlainText(file.readAll());
    }
    else {   // filePath stores content when called by external process
        ui.teOutput->setPlainText(filePath);
    }

    _undoStack.clear();
    resetTriggered();
    setTriggered(Open, true);
}

void MainWindow::onNewFile()
{
	ui.teOutput->clear();
	_undoStack.clear();
    resetTriggered();
}

void MainWindow::onOpen()
{
    QString filePath = QFileDialog::getOpenFileName(this,
		tr("Open Bibtex file"), ".", tr("Bibtex (*.bib);;All files (*.*)"));
    if(filePath.isEmpty())
		return;

    openBibFile(filePath);
}

void MainWindow::onPaste() {
    setTriggered(Open, !ui.teOutput->toPlainText().isEmpty());
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
        ui.teOutput->setFont(Setting::getInstance()->getFont());
}

void MainWindow::onClean() {
    if(canClean())
        _undoStack.push(new CleanCommand(this));
}

void MainWindow::onCapitalize() {
    if(canCapitalize())
        _undoStack.push(new CapitalizeCommand(this));
}

void MainWindow::onProtect() {
    if(canProtect())
        _undoStack.push(new ProtectCommand(this));
}

void MainWindow::onAbbreviate() {
    if(canAbbreviate())
        _undoStack.push(new AbbreviateCommand(this));
}

void MainWindow::onGenerateKeys() {
    if(canGenerateKeys())
        _undoStack.push(new GenerateKeysCommand(this));
}

void MainWindow::onRunAll()
{
    if(canRunAll())
    {
        onClean();
        onCapitalize();
        onProtect();
        onAbbreviate();
        onGenerateKeys();
    }
}

void MainWindow::onAbout() {
	QMessageBox::about(this, "About",
		tr("<h3><b>BibFixer: Fixing BibTex files</b></h3>"
		   "<p>Built on %1</p>"
		   "<p><a href=mailto:CongChenUTD@Gmail.com>CongChenUTD@Gmail.com</a></p>")
					   .arg(Setting::getInstance()->getCompileDate()));

}

}
