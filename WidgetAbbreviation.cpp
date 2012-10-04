#include "WidgetAbbreviation.h"
#include "DlgSettings.h"
#include "../ImageColorBoolModel/ImageColorBoolProxy.h"
#include "../ImageColorBoolModel/ImageColorBoolDelegate.h"
#include <QFile>
#include <QTextStream>
#include <QHeaderView>

WidgetAbbreviation::WidgetAbbreviation(QWidget* parent) :
	QWidget(parent), currentRow(-1)
{
	ui.setupUi(this);
	setting = UserSetting::getInstance("Rules.ini");

	load();
	ui.tvAbbreviations->setModel(&model);
	ui.tvAbbreviations->resizeColumnsToContents();
	ui.tvAbbreviations->horizontalHeader()->setStretchLastSection(true);

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

QStringList WidgetAbbreviation::getSelectedRules() const
{
	QStringList result;
	int rowCount = model.rowCount();
	for(int row=0; row<rowCount; row++)
		if(model.data(model.index(row, SELECT)).toBool())
			result << model.data(model.index(row, FULL)).toString() + ";" +
					  model.data(model.index(row, ABBR)).toString();
	return result;
}

void WidgetAbbreviation::onCurrentRowChanged(const QModelIndex& idx)
{
	currentRow = idx.isValid() ? idx.row() : -1;
	ui.btDel->setEnabled(idx.isValid());
}

void WidgetAbbreviation::onAdd()
{
	int lastRow = model.rowCount();
	model.insertRow(lastRow);
	model.setData(model.index(lastRow, SELECT), true);
	ui.tvAbbreviations->selectRow(lastRow);
	ui.tvAbbreviations->scrollToBottom();
}

void WidgetAbbreviation::onDel()
{
	model.removeRow(currentRow);  // FIXME: combining the two lines won't work
	currentRow --;
}

void WidgetAbbreviation::load()
{
	model.setColumnCount(3);
	model.setHeaderData(FULL,   Qt::Horizontal, tr("Fullname"));
	model.setHeaderData(ABBR,   Qt::Horizontal, tr("Abbreviation"));
	model.setHeaderData(SELECT, Qt::Horizontal, tr("Select"));
	QStringList rules = setting->getAbbreviationRules();
	model.setRowCount(rules.size());
	for(int row = 0; row < rules.size(); ++ row)
	{
		QStringList sections = rules.at(row).split(";");
		if(sections.size() == 3)
		{
			model.setData(model.index(row, FULL),   sections[0]);
			model.setData(model.index(row, ABBR),   sections[1]);
			model.setData(model.index(row, SELECT), sections[2]);
		}
	}
}

void WidgetAbbreviation::save()
{
	model.sort(FULL);
	QStringList rules;
	for(int row = 0; row < model.rowCount(); ++ row)
	{
		QString fullName        = model.data(model.index(row, FULL)).toString();
		QString abbreviatedName = model.data(model.index(row, ABBR)).toString();
		QString selected        = model.data(model.index(row, SELECT)).toString();
		QStringList rule;
		if(!fullName.isEmpty() && !abbreviatedName.isEmpty() && !selected.isEmpty())
			rule << fullName << abbreviatedName << selected;
		rules << rule.join(";");
	}
	setting->setAbbreviationRules(rules);
}
