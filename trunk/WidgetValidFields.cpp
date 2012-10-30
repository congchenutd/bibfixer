#include "WidgetValidFields.h"
#include "DlgSettings.h"
#include <QHeaderView>

namespace BibFixer {

WidgetValidFields::WidgetValidFields(QWidget* parent) :	QWidget(parent)
{
    _ui.setupUi(this);

	load();
    _ui.tvFields->setModel(&_model);
    _ui.tvFields->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    connect(_ui.tvFields->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
			this, SLOT(onCurrentRowChanged(QModelIndex)));
    connect(_ui.btAdd, SIGNAL(clicked()), this, SLOT(onAdd()));
    connect(_ui.btDel, SIGNAL(clicked()), this, SLOT(onDel()));
}

void WidgetValidFields::onCurrentRowChanged(const QModelIndex& idx)
{
    _currentRow = idx.isValid() ? idx.row() : -1;
    _ui.btDel->setEnabled(idx.isValid());
}

void WidgetValidFields::onAdd()
{
    int lastRow = _model.rowCount();
    _model.insertRow(lastRow);
    _ui.tvFields->scrollToBottom();
    _ui.tvFields->edit(_model.index(lastRow, 0));
}

void WidgetValidFields::onDel()
{
    int backup = _currentRow;      // removeRow() will trigger onCurrentRowChanged()
    _model.removeRow(_currentRow);
    _currentRow = backup;
    _ui.tvFields->selectRow(_currentRow);
}

void WidgetValidFields::load()
{
    _model.setColumnCount(1);
    _model.setHeaderData(0, Qt::Horizontal, tr("Field name"));

    QStringList fields = Setting::getInstance("Rules.ini")->getFields();
    _model.setRowCount(fields.size());
	for(int row = 0; row < fields.size(); ++ row)
        _model.setData(_model.index(row, 0), fields.at(row));
}

void WidgetValidFields::save()
{
    _model.sort(0);
	QStringList fields;
    for(int row = 0; row < _model.rowCount(); ++ row)
    {
        QString field = _model.data(_model.index(row, 0)).toString();
        if(!field.isEmpty())
            fields << field;
    }
    Setting::getInstance("Rules.ini")->setFields(fields);
}

}
