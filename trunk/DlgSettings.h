#ifndef DLGSETTINGS_H
#define DLGSETTINGS_H

#include "ui_DlgSettings.h"
#include "MySetting.h"

class UserSetting;
class DlgSettings : public QDialog
{
	Q_OBJECT

public:
	DlgSettings(QWidget *parent = 0);
	virtual void accept();

private slots:
	void onFont();

private:
	Ui::DlgSettings ui;
	QFont font;
	UserSetting* setting;
};


class UserSetting : public MySetting<UserSetting>
{
public:
	UserSetting(const QString& fileName);

	QFont getFont() const;
	void setFont(const QFont& font);

	QStringList getFields() const;
	void setFields(const QStringList& fields);

	QStringList getAbbreviationRules() const;
	QStringList getSelectedAbbreviationRules() const;
	void setAbbreviationRules(const QStringList& rules);

private:
	void loadDefaults();
};

#endif // DLGSETTINGS_H
