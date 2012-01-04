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
	loadFields();
	loadAbbreviationRules();

	connect(ui.btFont, SIGNAL(clicked()), this, SLOT(onFont()));
}

void DlgSettings::accept()
{
	setting->setFont(font);
	saveFields();
	saveAbbreviationRules();

	QDialog::accept();
}

void DlgSettings::onFont()
{
	bool ok;
	QFont f = QFontDialog::getFont(&ok, font, this);
	if(ok)
		font = f;
}

void DlgSettings::loadFields() {
	ui.teFields->setPlainText(setting->getFields().join("\r\n"));
}

void DlgSettings::saveFields() {
	setting->setFields(ui.teFields->toPlainText());
}

void DlgSettings::loadAbbreviationRules() {
	ui.teAbbreviation->setPlainText(setting->getAbbreviationRules().join("\r\n"));
}

void DlgSettings::saveAbbreviationRules() {
	setting->setAbbreviationRules(ui.teAbbreviation->toPlainText());
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
	QStringList result = value("Fields").toString().split("\n");
	result.sort();
	return result;
}

void UserSetting::setFields(const QString& fileData) {
	setValue("Fields", fileData.toLower());
}

QStringList UserSetting::getAbbreviationRules() const
{
	QStringList rules = value("AbbreviationRules").toString().split('#');
	QStringList result;
	foreach(QString rule, rules)
	{
		QStringList sections = rule.split('\t');
		if(sections.size() == 3 && sections[2].toLower() == "true")
			result << rule;
	}
	return result;
}

void UserSetting::setAbbreviationRules(const QString& data)
{
	QString input = data;
	QMap<QString, QString> rules;
	QStringList ruleNames;
	QTextStream is(&input);
	while(!is.atEnd())
	{
		QStringList sections = is.readLine().split('\t');
		if(sections.size() == 3)
		{
			rules.insert(sections[0], sections.join("\t"));
			ruleNames << sections[0];
		}
	}

	// qmap does not support reverse traverse
	QStringList result;
	for(int i = ruleNames.size() - 1; i >= 0; --i)
		result << rules[ruleNames.at(i)];

	setValue("AbbreviationRules", result.join("\n"));
}
