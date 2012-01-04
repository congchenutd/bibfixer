#include "DlgAbbreviation.h"
#include "../ImageColorBoolModel/ImageColorBoolProxy.h"
#include "../ImageColorBoolModel/ImageColorBoolDelegate.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

DlgAbbreviation::DlgAbbreviation(QWidget *parent) :
	QDialog(parent), currentRow(-1)
{
	ui.setupUi(this);

	load();
	ui.tvAbbreviations->setModel(&model);
	ui.tvAbbreviations->resizeColumnsToContents();
	ui.tvAbbreviations->sortByColumn(FULL, Qt::AscendingOrder);

	ImageColorBoolProxy* proxy = new ImageColorBoolProxy(this);
	proxy->setColumnType(SELECT, ImageColorBoolProxy::BoolColumn);
	proxy->setSourceModel(&model);

	ui.tvAbbreviations->setModel(proxy);
	ImageColorBoolDelegate* delegate = new ImageColorBoolDelegate(proxy, ui.tvAbbreviations);
	delegate->setEditTrigger(QEvent::MouseButtonPress);
	delegate->setCheckedImage  (QPixmap(":/Images/Checked.png"));
	delegate->setUncheckedImage(QPixmap(":/Images/UnChecked.png"));
	ui.tvAbbreviations->setItemDelegate(delegate);

	connect(ui.tvAbbreviations->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
			this, SLOT(onCurrentRowChanged(QModelIndex)));
	connect(ui.btAdd, SIGNAL(clicked()), this, SLOT(onAdd()));
	connect(ui.btDel, SIGNAL(clicked()), this, SLOT(onDel()));
}

void DlgAbbreviation::accept()
{
	save();
	QDialog::accept();
}

QStringList DlgAbbreviation::getSelectedRules() const
{
	QStringList result;
	int rowCount = model.rowCount();
	for(int row=0; row<rowCount; row++)
		if(model.data(model.index(row, SELECT)).toBool())
			result << model.data(model.index(row, FULL)).toString() + ";" +
					  model.data(model.index(row, ABBR)).toString();
	return result;
}

void DlgAbbreviation::onCurrentRowChanged(const QModelIndex& idx)
{
	currentRow = idx.isValid() ? idx.row() : -1;
	ui.btDel->setEnabled(idx.isValid());
}

void DlgAbbreviation::onAdd()
{
	int lastRow = model.rowCount();
	model.insertRow(lastRow);
	model.setData(model.index(lastRow, SELECT), true);
	ui.tvAbbreviations->selectRow(lastRow);
	ui.tvAbbreviations->scrollToBottom();
}

void DlgAbbreviation::onDel()
{
	if(QMessageBox::warning(this, tr("Warning"), tr("Are you sure to delete?"),
							QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
	{
		model.removeRow(currentRow);  // FIXME: combining the two lines won't work
		currentRow --;
	}
}

void DlgAbbreviation::load()
{
	QFile file("Abbreviation.txt");
	if(file.open(QFile::ReadOnly))
	{
		model.setColumnCount(3);
		model.setHeaderData(FULL,   Qt::Horizontal, tr("Fullname"));
		model.setHeaderData(ABBR,   Qt::Horizontal, tr("Abbreviation"));
		model.setHeaderData(SELECT, Qt::Horizontal, tr("Select"));

		QTextStream is(&file);
		while(!is.atEnd())
		{
			QString line = is.readLine();
			QStringList sections = line.split('\t');
			if(sections.size() == 3)
			{
				int lastRow = model.rowCount();
				model.insertRow(lastRow);
				model.setData(model.index(lastRow, FULL),   sections[0]);
				model.setData(model.index(lastRow, ABBR),   sections[1]);
				model.setData(model.index(lastRow, SELECT), sections[2]);
			}
		}
	}
}

void DlgAbbreviation::save()
{
	QFile file("Abbreviation.txt");
	if(file.open(QFile::WriteOnly | QFile::Truncate))
	{
		QTextStream os(&file);
		int rowCount = model.rowCount();
		for(int row=0; row<rowCount; row++)
		{
			QString fullName        = model.data(model.index(row, FULL)).toString();
			QString abbreviatedName = model.data(model.index(row, ABBR)).toString();
			QString selected        = model.data(model.index(row, SELECT)).toString();
			if(!fullName.isEmpty() && !abbreviatedName.isEmpty() && !selected.isEmpty())
				os << fullName << '\t' << abbreviatedName << '\t' << selected << "\r\n";
		}
	}
}
