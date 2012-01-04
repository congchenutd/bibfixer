#include "DlgSettings.h"
#include <QFontDialog>
#include <QApplication>

DlgSettings::DlgSettings(QWidget *parent) :
	QDialog(parent)
{
	ui.setupUi(this);
	font = UserSetting::getInstance()->getFont();
	loadFields();

	connect(ui.btFont, SIGNAL(clicked()), this, SLOT(onFont()));
}

void DlgSettings::accept()
{
	UserSetting::getInstance()->setFont(font);
	saveFields();

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
	ui.teFields->setPlainText(UserSetting::getInstance()->getFields().join("\r\n"));
}

void DlgSettings::saveFields() {
	UserSetting::getInstance()->setFields(ui.teFields->toPlainText());
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
