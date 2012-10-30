#include "WidgetAbbreviation.h"
#include "DlgSettings.h"
#include "ImageColorBoolProxy.h"
#include "ImageColorBoolDelegate.h"
#include <QHeaderView>

namespace BibFixer {

WidgetAbbreviation::WidgetAbbreviation(QWidget* parent) :
	QWidget(parent), _currentRow(-1)
{
	_ui.setupUi(this);

	load();
	_ui.tvAbbreviations->setModel(&_model);
	_ui.tvAbbreviations->resizeColumnsToContents();
	_ui.tvAbbreviations->horizontalHeader()->setStretchLastSection(true);

	ImageColorBoolProxy* proxy = new ImageColorBoolProxy(this);
    proxy->setColumnType(SELECTED, ImageColorBoolProxy::BoolColumn);
	proxy->setSourceModel(&_model);

	_ui.tvAbbreviations->setModel(proxy);
	ImageColorBoolDelegate* delegate = new ImageColorBoolDelegate(proxy, _ui.tvAbbreviations);
    delegate->setEditTrigger(QEvent::MouseButtonPress);
	delegate->setCheckedImage  (QPixmap(":/Images/Checked.png"));
	delegate->setUncheckedImage(QPixmap(":/Images/UnChecked.png"));
	_ui.tvAbbreviations->setItemDelegate(delegate);

	connect(_ui.tvAbbreviations->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
			this, SLOT(onCurrentRowChanged(QModelIndex)));
	connect(_ui.btAdd, SIGNAL(clicked()), this, SLOT(onAdd()));
	connect(_ui.btDel, SIGNAL(clicked()), this, SLOT(onDel()));
}

QStringList WidgetAbbreviation::getSelectedRules() const
{
	QStringList result;
    for(int row = 0; row < _model.rowCount(); row ++)
        if(_model.data(_model.index(row, SELECTED)).toBool())
			result << _model.data(_model.index(row, FULL)).toString() + ";" +
                      _model.data(_model.index(row, SHORT)).toString();
	return result;
}

void WidgetAbbreviation::onCurrentRowChanged(const QModelIndex& idx)
{
	_currentRow = idx.isValid() ? idx.row() : -1;
	_ui.btDel->setEnabled(idx.isValid());
}

void WidgetAbbreviation::onAdd()
{
	int lastRow = _model.rowCount();
	_model.insertRow(lastRow);
    _model.setData(_model.index(lastRow, SELECTED), true);
	_ui.tvAbbreviations->scrollToBottom();
    _ui.tvAbbreviations->selectRow(lastRow);

    // FIXME: this doesn't work probably because of the delegate
    // _ui.tvAbbreviations->edit(_model.index(lastRow, FULL));
}

void WidgetAbbreviation::onDel()
{
    int backup = _currentRow;      // removeRow() will trigger onCurrentRowChanged()
    _model.removeRow(_currentRow);
    _currentRow = backup;
}

void WidgetAbbreviation::load()
{
	_model.setColumnCount(3);
    _model.setHeaderData(FULL,     Qt::Horizontal, tr("Fullname"));
    _model.setHeaderData(SHORT,    Qt::Horizontal, tr("Abbreviation"));
    _model.setHeaderData(SELECTED, Qt::Horizontal, tr("Selected"));
    QStringList rules = Setting::getInstance("Rules.ini")->getAbbreviationRules();
	_model.setRowCount(rules.size());
    for(int row = 0; row < rules.size(); ++ row)
    {
        QStringList sections = rules.at(row).split(";");
		if(sections.size() == 3)
		{
            _model.setData(_model.index(row, FULL),     sections[FULL]);
            _model.setData(_model.index(row, SHORT),    sections[SHORT]);
            _model.setData(_model.index(row, SELECTED), sections[SELECTED]);
		}
	}
}

void WidgetAbbreviation::save()
{
	_model.sort(FULL);
	QStringList rules;
	for(int row = 0; row < _model.rowCount(); ++ row)
	{
        QString fullName  = _model.data(_model.index(row, FULL))    .toString();
        QString shortName = _model.data(_model.index(row, SHORT))   .toString();
        QString selected  = _model.data(_model.index(row, SELECTED)).toString();
        if(!fullName.isEmpty() && !shortName.isEmpty() && !selected.isEmpty())
            rules << (QStringList() << fullName << shortName << selected).join(";");
	}
    Setting::getInstance("Rules.ini")->setAbbreviationRules(rules);
}

}
