#include "DlgSettings.h"
#include <QFontDialog>
#include <QApplication>
#include <QDebug>

DlgSettings::DlgSettings(QWidget *parent) :
	QDialog(parent)
{
	ui.setupUi(this);
	setting = UserSetting::getInstance();
	font = setting->getFont();

	connect(ui.btFont, SIGNAL(clicked()), this, SLOT(onFont()));
}

void DlgSettings::accept()
{
	setting->setFont(font);
	ui.tabValidFields      ->save();
	ui.tabAbbreviationRules->save();

	QDialog::accept();
}

void DlgSettings::onFont()
{
	bool ok;
	QFont f = QFontDialog::getFont(&ok, font, this);
	if(ok)
		font = f;
}


//////////////////////////////////////////////////////////////////////////
// Setting
UserSetting::UserSetting(const QString& fileName)
: MySetting<UserSetting>(fileName)
{
	if(QFile(this->fileName).size() == 0)   // no setting
		loadDefaults();
}

void UserSetting::loadDefaults() {
	setFont(qApp->font());
}

QFont UserSetting::getFont() const
{
	QFont font;
	font.fromString(value("Font").toString());
	return font;
}

void UserSetting::setFont(const QFont& font) {
	setValue("Font", font);
}

QStringList UserSetting::getFields() const
{
	// empty string.split() will produce a stringlist with one empty entry
	QString content = value("Fields").toString();
	return content.isEmpty() ? QStringList() : content.split(";");
}

void UserSetting::setFields(const QStringList& fields) {
	setValue("Fields", fields.join(";").toLower());
}

QStringList UserSetting::getAbbreviationRules() const
{
	// empty string.split() will produce a stringlist with one empty entry
	QString content = value("AbbreviationRules").toString();
	return content.isEmpty() ? QStringList() : content.split("#");
}

QStringList UserSetting::getSelectedAbbreviationRules() const
{
	QStringList allRules = getAbbreviationRules();
	QStringList selectedRules;
	foreach(const QString& rule, allRules)
	{
		QStringList sections = rule.split(";");
		if(sections.size() == 3 && sections[2].toLower() == "true")
			selectedRules << sections[0] + ";" + sections[1];
	}

	// reverse the order, ensuring the longer is before the shorter
	QStringList result;
	for(int i = selectedRules.size() - 1; i >= 0; -- i)
		result << selectedRules.at(i);
	return result;
}

void UserSetting::setAbbreviationRules(const QStringList& rules) {
	setValue("AbbreviationRules", rules.join("#"));
}