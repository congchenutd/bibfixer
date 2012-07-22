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
    initActionStatuses();
	ui.teOutput->setFont(UserSetting::getInstance()->getFont());

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
}

void MainWindow::initActionStatuses()
{
    setActionStatus(Init,          true);
    setActionStatus(Opened,        false);
    setActionStatus(Cleaned,       false);
    setActionStatus(Capitalized,   false);
    setActionStatus(Protected,     false);
    setActionStatus(Abbreviated,   false);
    setActionStatus(KeysGenerated, false);
    setActionStatus(RunAll,        false);
    setActionStatus(Save,          false);
}

void MainWindow::setActionStatus(MainWindow::ActionStatus status, bool value)
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
    ui.mainToolBar->insertAction(ui.actionSettings, undoAction);

    QAction* redoAction = undoStack.createRedoAction(this, tr("Redo"));
    redoAction->setIcon(QIcon(":/Images/Redo.png"));
    redoAction->setShortcuts(QKeySequence::Redo);
    ui.mainToolBar->insertAction(ui.actionSettings, redoAction);
    ui.mainToolBar->insertSeparator(ui.actionSettings);
}

void MainWindow::open(const QString& filePath)
{
    QFile file(filePath);
    if(file.open(QFile::ReadOnly))
    {
        ui.teOutput->setPlainText(file.readAll());
        undoStack.clear();
        initActionStatuses();
        setActionStatus(Opened, true);
    }
}

void MainWindow::onOpen()
{
    QString filePath = QFileDialog::getOpenFileName(this,
		tr("Open Bibtex file"), ".", tr("Bibtex (*.bib);;All files (*.*)"));
    if(filePath.isEmpty())
		return;

    open(filePath);
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
					   .arg(UserSetting::getInstance()->getCompileDate()));

}
