#ifndef WIDGETVALIDFIELDS_H
#define WIDGETVALIDFIELDS_H

#include "ui_WidgetValidFields.h"
#include <QWidget>
#include <QStandardItemModel>

class UserSetting;
class WidgetValidFields : public QWidget
{
	Q_OBJECT

public:
	WidgetValidFields(QWidget* parent = 0);
	void save();    // save to setting


private slots:
	void onCurrentRowChanged(const QModelIndex& idx);
	void onAdd();
	void onDel();

private:
	void load();    // load data from setting

private:
	Ui::WidgetValidFields ui;
	UserSetting* setting;
	QStandardItemModel model;
	int currentRow;
};

#endif // WIDGETVALIDFIELDS_H
