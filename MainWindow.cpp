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
    ui.teOutput->setFont(Setting::getInstance()->getFont());

    initActions();
    resetActionStatus();

    connect(ui.actionNew,          SIGNAL(triggered()), this, SLOT(onNewFile()));
    connect(ui.actionOpen,         SIGNAL(triggered()), this, SLOT(onOpen()));
    connect(ui.actionSave,         SIGNAL(triggered()), this, SLOT(onSave()));
    connect(ui.actionSettings,     SIGNAL(triggered()), this, SLOT(onSettings()));
    connect(ui.actionRunAll,       SIGNAL(triggered()), this, SLOT(onRunAll()));
    connect(ui.actionClean,        SIGNAL(triggered(bool)), this, SLOT(onClean     (bool)));
    connect(ui.actionCapitalize,   SIGNAL(triggered(bool)), this, SLOT(onCapitalize(bool)));
    connect(ui.actionProtect,      SIGNAL(triggered(bool)), this, SLOT(onProtect   (bool)));
    connect(ui.actionAbbreviate,   SIGNAL(triggered(bool)), this, SLOT(onAbbreviate(bool)));
    connect(ui.actionGenerateKeys, SIGNAL(triggered()), this, SLOT(onGenerateKeys()));
    connect(ui.actionAbout,        SIGNAL(triggered()), this, SLOT(onAbout()));
    connect(ui.teOutput,           SIGNAL(pasted()),    this, SLOT(onPaste()));
}

void MainWindow::resetActionStatus()
{
    updateActionStatus(Init,         true);
    updateActionStatus(Open,         false);
    updateActionStatus(Clean,        false);
    updateActionStatus(Capitalize,   false);
    updateActionStatus(Protect,      false);
    updateActionStatus(Abbreviate,   false);
    updateActionStatus(GenerateKeys, false);
    updateActionStatus(RunAll,       false);
    updateActionStatus(Save,         false);
}

void MainWindow::setActionTriggered(MainWindow::ActionName actionName, bool triggered)
{
    // _triggered is synced with action's checked() status
    // use _triggered because some actions are not checkable, such as RunAll, GenerateKeys
    _triggered[actionName] = triggered;
    _actions  [actionName]->setChecked(triggered);
}

void MainWindow::updateActionStatus(MainWindow::ActionName actionName, bool triggered)
{
    setActionTriggered(actionName, triggered);

    // Clean affects other 4 actions
    if(!ui.actionClean->isChecked())
    {
        setActionTriggered(Capitalize,  false);
        setActionTriggered(Protect,     false);
        setActionTriggered(Abbreviate,  false);
        setActionTriggered(GenerateKeys,false);
    }

    ui.actionSave        ->setEnabled(isTriggered(Open));
    ui.actionClean       ->setEnabled(isTriggered(Open));
    ui.actionRunAll      ->setEnabled(canRunAll());
    ui.actionCapitalize  ->setEnabled(isTriggered(Clean));
    ui.actionProtect     ->setEnabled(isTriggered(Clean));
    ui.actionAbbreviate  ->setEnabled(isTriggered(Clean));
    ui.actionGenerateKeys->setEnabled(isTriggered(Clean));
}

void MainWindow::initActions()
{
    ui.actionOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    _actions << ui.actionOpen  << ui.actionSave       << ui.actionRunAll
             << ui.actionClean << ui.actionCapitalize << ui.actionProtect
             << ui.actionAbbreviate << ui.actionGenerateKeys
             << new QAction(this);   // placeholder for Init
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

    resetActionStatus();
    updateActionStatus(Open, true);
}

void MainWindow::onNewFile()
{
	ui.teOutput->clear();
    resetActionStatus();
}

void MainWindow::onOpen()
{
    QString filePath = QFileDialog::getOpenFileName(this,
		tr("Open Bibtex file"), ".", tr("Bibtex (*.bib);;All files (*.*)"));
    if(filePath.isEmpty())
		return;

    openBibFile(filePath);
}

// same as onOpen()
void MainWindow::onPaste()
{
    resetActionStatus();
    updateActionStatus(Open, !getContent().isEmpty());
}

void MainWindow::onSave()
{
    if(getContent().isEmpty())
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
        ui.teOutput->setFont(Setting::getInstance()->getFont());
}

void MainWindow::onClean(bool redo)
{
    if(redo && !isTriggered(Clean))
        CleanCommand::setOriginalText(getContent());
    runCommand(Clean, redo, new CleanCommand(this));
}
void MainWindow::onCapitalize(bool redo) {
    runCommand(Capitalize, redo, new CapitalizeCommand(this));
}
void MainWindow::onProtect(bool redo) {
    runCommand(Protect, redo, new ProtectCommand(this));
}
void MainWindow::onAbbreviate(bool redo) {
    runCommand(Abbreviate, redo, new AbbreviateCommand(this));
}
void MainWindow::onGenerateKeys() {
    runCommand(GenerateKeys, true, new GenerateKeysCommand(this));
}

void MainWindow::runCommand(MainWindow::ActionName actionName, bool redo, AbstractCommand* command)
{
    if(redo) {
        if(!isTriggered(actionName))
            command->redo();
    }
    else
        command->undo();
}

QString MainWindow::getContent() const {
    return getTextEdit()->toPlainText();
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

}  // namespace BibFixer
