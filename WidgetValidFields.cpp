#include "WidgetValidFields.h"
#include "DlgSettings.h"
#include <QHeaderView>

WidgetValidFields::WidgetValidFields(QWidget* parent) :	QWidget(parent)
{
	ui.setupUi(this);

	load();
	ui.tvFields->setModel(&model);
	ui.tvFields->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	connect(ui.tvFields->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
			this, SLOT(onCurrentRowChanged(QModelIndex)));
	connect(ui.btAdd, SIGNAL(clicked()), this, SLOT(onAdd()));
	connect(ui.btDel, SIGNAL(clicked()), this, SLOT(onDel()));
}

void WidgetValidFields::onCurrentRowChanged(const QModelIndex& idx)
{
	currentRow = idx.isValid() ? idx.row() : -1;
	ui.btDel->setEnabled(idx.isValid());
}

void WidgetValidFields::onAdd()
{
	int lastRow = model.rowCount();
	model.insertRow(lastRow);
	ui.tvFields->selectRow(lastRow);
	ui.tvFields->scrollToBottom();
}

void WidgetValidFields::onDel()
{
	model.removeRow(currentRow);  // FIXME: combining the two lines won't work
	currentRow --;
}

void WidgetValidFields::load()
{
	model.setColumnCount(1);
	model.setHeaderData(0, Qt::Horizontal, tr("Field name"));

    QStringList fields = Setting::getInstance("Rules.ini")->getFields();
	model.setRowCount(fields.size());
	for(int row = 0; row < fields.size(); ++ row)
		model.setData(model.index(row, 0), fields.at(row));
}

void WidgetValidFields::save()
{
	model.sort(0);
	QStringList fields;
	for(int row = 0; row < model.rowCount(); ++ row)
		fields << model.data(model.index(row, 0)).toString();
    Setting::getInstance("Rules.ini")->setFields(fields);
}
