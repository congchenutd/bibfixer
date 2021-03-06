#ifndef WIDGETVALIDFIELDS_H
#define WIDGETVALIDFIELDS_H

#include "ui_WidgetValidFields.h"
#include <QWidget>
#include <QStandardItemModel>

namespace BibFixer {

class Setting;

// UI for editing valid fields
class WidgetValidFields : public QWidget
{
	Q_OBJECT

public:
	WidgetValidFields(QWidget* parent = 0);
    void save();

private slots:
	void onCurrentRowChanged(const QModelIndex& idx);
	void onAdd();
	void onDel();

private:
    void loadValidFields();    // load data from setting

private:
    enum {FIELD_NAME, SELECTED};   // columns

    Ui::WidgetValidFields _ui;
    QStandardItemModel _model;
    int                _currentRow;
    const static char  _separator = ';';
};

}

#endif // WIDGETVALIDFIELDS_H
