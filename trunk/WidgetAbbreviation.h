#ifndef DLGABBREVIATION_H
#define DLGABBREVIATION_H

#include "ui_WidgetAbbreviation.h"
#include <QStandardItemModel>

namespace BibFixer {

// edit abbreviation rules
class Setting;
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
    void load();

private:
	Ui::WidgetAbbreviation ui;
	QStandardItemModel model;
	int currentRow;

    enum {FULL, ABBR, SELECTED};
};

}

#endif // DLGABBREVIATION_H
