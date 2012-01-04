#ifndef DLGABBREVIATION_H
#define DLGABBREVIATION_H

#include "ui_DlgAbbreviation.h"
#include <QStandardItemModel>

// edit abbreviation rules
class DlgAbbreviation : public QDialog
{
	Q_OBJECT

public:
	DlgAbbreviation(QWidget *parent = 0);
	virtual void accept();
	QStringList getSelectedRules() const;  // fullName;abbreviatedName

private slots:
	void onCurrentRowChanged(const QModelIndex& idx);
	void onAdd();
	void onDel();

private:
	void load();    // data saved in Abbreviation.txt
	void save();

private:
	Ui::DlgAbbreviation ui;
	QStandardItemModel model;
	int currentRow;
	enum {FULL, ABBR, SELECT};
};

#endif // DLGABBREVIATION_H
