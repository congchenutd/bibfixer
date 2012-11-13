#include "WidgetValidFields.h"
#include "DlgSettings.h"
#include "ImageColorBoolProxy.h"
#include "ImageColorBoolDelegate.h"
#include <QHeaderView>

namespace BibFixer {

WidgetValidFields::WidgetValidFields(QWidget* parent) :	QWidget(parent)
{
    _ui.setupUi(this);

	load();
    _ui.tvFields->setModel(&_model);
    _ui.tvFields->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

    ImageColorBoolProxy* proxy = new ImageColorBoolProxy(this);
    proxy->setColumnType(SELECTED, ImageColorBoolProxy::BoolColumn);
    proxy->setSourceModel(&_model);

    _ui.tvFields->setModel(proxy);
    ImageColorBoolDelegate* delegate = new ImageColorBoolDelegate(proxy, _ui.tvFields);
    delegate->setEditTrigger(QEvent::MouseButtonPress);
    delegate->setCheckedImage  (QPixmap(":/Images/Checked.png"));
    delegate->setUncheckedImage(QPixmap(":/Images/UnChecked.png"));
    _ui.tvFields->setItemDelegate(delegate);

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
    _model.setData(_model.index(lastRow, SELECTED), true);
    _ui.tvFields->scrollToBottom();
    _ui.tvFields->edit(_model.index(lastRow, FIELD_NAME));
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
    _model.setColumnCount(2);
    _model.setHeaderData(FIELD_NAME, Qt::Horizontal, tr("Field name"));
    _model.setHeaderData(SELECTED,   Qt::Horizontal, tr("Selected"));

    QStringList fields = Setting::getInstance("Rules.ini")->getFields();
    _model.setRowCount(fields.size());
	for(int row = 0; row < fields.size(); ++ row)
    {
        QStringList sections = fields.at(row).split(";");
        if(sections.size() == 2)
        {
            _model.setData(_model.index(row, FIELD_NAME), sections[FIELD_NAME]);
            _model.setData(_model.index(row, SELECTED),   sections[SELECTED]);
        }
    }
}

void WidgetValidFields::save()
{
    _model.sort(FIELD_NAME);
	QStringList fields;
    for(int row = 0; row < _model.rowCount(); ++ row)
    {
        QString field    = _model.data(_model.index(row, FIELD_NAME)).toString();
        QString selected = _model.data(_model.index(row, SELECTED))  .toString();
        if(!field.isEmpty() && !selected.isEmpty())
            fields << (QStringList() << field << selected).join(";");
    }
    Setting::getInstance("Rules.ini")->setFields(fields);
}

} // namespace
