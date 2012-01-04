#include "MainWindow.h"
#include "DlgSettings.h"
#include "DlgAbbreviation.h"
#include "Parser.h"
#include <QScrollBar>
#include <QStyle>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	ui.actionOpen->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
	ui.teOutput->setFont(UserSetting::getInstance()->getFont());
	updateButtons(INIT);

	connect(ui.actionOpen,       SIGNAL(triggered()), this, SLOT(onOpen()));
	connect(ui.actionSave,       SIGNAL(triggered()), this, SLOT(onSave()));
	connect(ui.actionSettings,   SIGNAL(triggered()), this, SLOT(onSettings()));
	connect(ui.actionClean,      SIGNAL(triggered()), this, SLOT(onClean()));
	connect(ui.actionCapitalize, SIGNAL(triggered()), this, SLOT(onCapitalize()));
	connect(ui.actionProtect,    SIGNAL(triggered()), this, SLOT(onProtect()));
	connect(ui.actionAbbreviate, SIGNAL(triggered()), this, SLOT(onAbbreviate()));
	connect(ui.actionAbout,      SIGNAL(triggered()), this, SLOT(onAbout()));
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

void MainWindow::onClean()
{
	BibParser parser;
	references = parser.parse(getContent());
	updateOutput();
	updateButtons(CLEAN);
}

void MainWindow::onSettings()
{
	DlgSettings dlg(this);
	if(dlg.exec() == QDialog::Accepted)
		ui.teOutput->setFont(UserSetting::getInstance()->getFont());
}

void MainWindow::onCapitalize()
{
	references.capitalize("title");
	references.capitalize("journal");
	references.capitalize("booktitle");

	// highlight changed lines
	QStringList toBeHighlighted = references.getChangedValues();
	foreach(const QString& line, toBeHighlighted)
		ui.teOutput->addHighlightedLine(line, Qt::yellow);

	updateOutput();
	ui.teOutput->highlightLines();
	updateButtons(CAPITALIZE);
}

void MainWindow::onProtect()
{
	references.clearChangedValues();
	references.protect("title");

	// highlight changed lines
	QStringList toBeHighlighted = references.getChangedValues();
	foreach(const QString& line, toBeHighlighted)
		ui.teOutput->addHighlightedLine(line, Qt::yellow);

	updateOutput();
	ui.teOutput->highlightLines();
	updateButtons(PROTECT);
}

void MainWindow::onAbbreviate()
{
	DlgAbbreviation dlg(this);
	if(dlg.exec() != QDialog::Accepted)
		return;

	references.clearChangedValues();
	references.abbreviate("journal");
	references.abbreviate("booktitle");

	// highlight changed lines
	QStringList toBeHighlighted = references.getChangedValues();
	foreach(const QString& line, toBeHighlighted)
		ui.teOutput->addHighlightedLine(line, Qt::green);

	updateOutput();
	ui.teOutput->highlightLines();
	updateButtons(ABBREVIATE);
}

void MainWindow::onAbout() {
	QMessageBox::about(this, tr("About"),
					   tr("<h3><b>BibFixer: Fixing BibTex output</b></h3>"
						  "2011/12/23"
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
	ui.actionClean     ->setEnabled(status == OPEN);
	ui.actionCapitalize->setEnabled(status >= CLEAN);
	ui.actionProtect   ->setEnabled(status >= CLEAN);
	ui.actionAbbreviate->setEnabled(status >= CLEAN);
	ui.actionSave      ->setEnabled(status >= CLEAN);
}
