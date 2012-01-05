#ifndef DLGABBREVIATION_H
#define DLGABBREVIATION_H

#include "ui_WidgetAbbreviation.h"
#include <QStandardItemModel>

// edit abbreviation rules
class UserSetting;
class WidgetAbbreviation : public QWidget
{
	Q_OBJECT

public:
	WidgetAbbreviation(QWidget* parent = 0);
	QStringList getSelectedRules() const;  // fullName;abbreviatedName
	void save();

private slots:
	void onCurrentRowChanged(const QModelIndex& idx);
	void onAdd();
	void onDel();

private:
	void load();    // data saved in Abbreviation.txt

private:
	Ui::WidgetAbbreviation ui;
	QStandardItemModel model;
	int currentRow;
	enum {FULL, ABBR, SELECT};
	UserSetting* setting;
};

#endif // DLGABBREVIATION_H
