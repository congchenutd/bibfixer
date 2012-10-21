#include "MainWindow.h"
#include "DlgSettings.h"
#include "Commands.h"
#include <QStyle>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
    createActions();
    initActionStatuses();
	ui.teOutput->setFont(Setting::getInstance()->getFont());

	connect(ui.actionNew,          SIGNAL(triggered()), this, SLOT(onNewFile()));
    connect(ui.actionOpen,         SIGNAL(triggered()), this, SLOT(onOpen()));
    connect(ui.teOutput,           SIGNAL(pasted()),    this, SLOT(onPaste()));
    connect(ui.actionSave,         SIGNAL(triggered()), this, SLOT(onSave()));
    connect(ui.actionSettings,     SIGNAL(triggered()), this, SLOT(onSettings()));
    connect(ui.actionRunAll,       SIGNAL(triggered()), this, SLOT(onRunAll()));
    connect(ui.actionClean,        SIGNAL(triggered()), this, SLOT(onClean()));
    connect(ui.actionCapitalize,   SIGNAL(triggered()), this, SLOT(onCapitalize()));
    connect(ui.actionProtect,      SIGNAL(triggered()), this, SLOT(onProtect()));
    connect(ui.actionAbbreviate,   SIGNAL(triggered()), this, SLOT(onAbbreviate()));
    connect(ui.actionGenerateKeys, SIGNAL(triggered()), this, SLOT(onGenerateKeys()));
    connect(ui.actionAbout,        SIGNAL(triggered()), this, SLOT(onAbout()));
}

void MainWindow::initActionStatuses()
{
    setActionStatus(Init,        true);
    setActionStatus(Open,        false);
    setActionStatus(Clean,       false);
    setActionStatus(Capitalize,  false);
    setActionStatus(Protect,     false);
    setActionStatus(Abbreviate,  false);
    setActionStatus(GenerateKey, false);
    setActionStatus(RunAll,      false);
    setActionStatus(Save,        false);
}

void MainWindow::setActionStatus(MainWindow::ActionName status, bool value)
{
    actionStatuses[status] = value;
    ui.actionOpen        ->setEnabled(isOpenEnabled());
    ui.actionClean       ->setEnabled(isCleanEnabled());
    ui.actionCapitalize  ->setEnabled(isCapitalizeEnabled());
    ui.actionProtect     ->setEnabled(isProtectEnabled());
    ui.actionAbbreviate  ->setEnabled(isAbbreviateEnabled());
    ui.actionGenerateKeys->setEnabled(isGenerateKeysEnabled());
    ui.actionSave        ->setEnabled(isSaveEnabled());
    ui.actionRunAll      ->setEnabled(isRunAllEnabled());
}

void MainWindow::createActions()
{
    ui.actionOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));

    QAction* undoAction = undoStack.createUndoAction(this, tr("Undo"));
    undoAction->setIcon(QIcon(":/Images/Undo.png"));
    undoAction->setShortcuts(QKeySequence::Undo);
    ui.mainToolBar->insertAction(0, undoAction);

    QAction* redoAction = undoStack.createRedoAction(this, tr("Redo"));
    redoAction->setIcon(QIcon(":/Images/Redo.png"));
    redoAction->setShortcuts(QKeySequence::Redo);
    ui.mainToolBar->insertAction(0, redoAction);
}

void MainWindow::open(const QString& filePath)
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

    undoStack.clear();
    initActionStatuses();
    setActionStatus(Open, true);
}

void MainWindow::onNewFile()
{
	ui.teOutput->clear();
	undoStack.clear();
	initActionStatuses();
}

void MainWindow::onOpen()
{
    QString filePath = QFileDialog::getOpenFileName(this,
		tr("Open Bibtex file"), ".", tr("Bibtex (*.bib);;All files (*.*)"));
    if(filePath.isEmpty())
		return;

    open(filePath);
}

void MainWindow::onPaste() {
    setActionStatus(Open, !ui.teOutput->toPlainText().isEmpty());
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
    if(isCleanEnabled())
        undoStack.push(new CleanCommand(this));
}

void MainWindow::onCapitalize() {
    if(isCapitalizeEnabled())
        undoStack.push(new CapitalizeCommand(this));
}

void MainWindow::onProtect() {
    if(isProtectEnabled())
        undoStack.push(new ProtectCommand(this));
}

void MainWindow::onAbbreviate() {
    if(isAbbreviateEnabled())
        undoStack.push(new AbbreviateCommand(this));
}

void MainWindow::onGenerateKeys() {
    if(isGenerateKeysEnabled())
        undoStack.push(new GenerateKeysCommand(this));
}

void MainWindow::onRunAll()
{
    onClean();
    onCapitalize();
    onProtect();
    onAbbreviate();
    onGenerateKeys();
}

void MainWindow::onAbout() {
	QMessageBox::about(this, "About",
		tr("<h3><b>BibFixer: Fixing BibTex files</b></h3>"
		   "<p>Built on %1</p>"
		   "<p><a href=mailto:CongChenUTD@Gmail.com>CongChenUTD@Gmail.com</a></p>")
					   .arg(Setting::getInstance()->getCompileDate()));

}
