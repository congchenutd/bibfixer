#ifndef DLGSETTINGS_H
#define DLGSETTINGS_H

#include "ui_DlgSettings.h"
#include "MySetting.h"

class DlgSettings : public QDialog
{
	Q_OBJECT

public:
	DlgSettings(QWidget *parent = 0);
	virtual void accept();

private slots:
	void onFont();

private:
	void loadFields();
	void saveFields();

private:
	Ui::DlgSettings ui;
	QFont font;
};


class UserSetting : public MySetting<UserSetting>
{
public:
	UserSetting(const QString& fileName);

	QFont getFont() const;
	void setFont(const QFont& font);

	QStringList getFields() const;
	void setFields(const QString& fileData);

private:
	void loadDefaults();
};

#endif // DLGSETTINGS_H
