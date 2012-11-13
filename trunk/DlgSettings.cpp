#include "DlgSettings.h"
#include <QFontDialog>
#include <QApplication>
#include <QResource>
#include <QMessageBox>

namespace BibFixer {

DlgSettings::DlgSettings(QWidget *parent) :
	QDialog(parent)
{
	_ui.setupUi(this);

    // load settings
	_setting = Setting::getInstance();
	_font = _setting->getFont();
    _ui.cbProtectFirstLetter->setChecked(_setting->getProtectFirstLetter());

	connect(_ui.btFont, SIGNAL(clicked()), this, SLOT(onFont()));
}

void DlgSettings::accept()
{
	_setting->setFont(_font);
    _setting->setProtectFirstLetter(_ui.cbProtectFirstLetter->isChecked());
	_ui.tabValidFields      ->save();
	_ui.tabAbbreviationRules->save();
    _ui.tabKeyGen           ->save();

	QDialog::accept();
}

void DlgSettings::onFont()
{
	bool ok;
    QFont font = QFontDialog::getFont(&ok, _font, this);
	if(ok)
        _font = font;
}


//////////////////////////////////////////////////////////////////////////
// Setting
Setting::Setting(const QString& fileName)
    : MySetting<Setting>(fileName)
{
	if(QFile(this->fileName).size() == 0)   // no setting
		loadDefaults();
}

void Setting::loadDefaults()
{
	setFont(qApp->font());
    setKeyGenRule("lastname;firstletter;year");
}

QFont Setting::getFont() const
{
	QFont font;
	font.fromString(value("Font").toString());
	return font;
}

void Setting::setFont(const QFont& font) {
	setValue("Font", font);
}

QStringList Setting::getFields() const
{
	// empty string.split() will produce a stringlist with one empty entry
	QString content = value("Fields").toString();
    return content.isEmpty() ? QStringList() : content.split("#");
}

QStringList Setting::getSelectedFields() const
{
    QStringList allFields = getFields();
    QStringList result;
    foreach(const QString& field, allFields)
    {
        QStringList sections = field.split(";");
        if(sections.size() == 2 && sections[1].toLower() == "true")
            result << sections[0];
    }
    return result;
}

void Setting::setFields(const QStringList& fields) {
    setValue("Fields", fields.join("#").toLower());
}

QStringList Setting::getAbbreviationRules() const
{
	// empty string.split() will produce a stringlist with one empty entry
	QString content = value("AbbreviationRules").toString();
    return content.isEmpty() ? QStringList() : content.split("#");
}

QStringList Setting::getSelectedAbbreviationRules() const
{
	QStringList allRules = getAbbreviationRules();
    QStringList result;
	foreach(const QString& rule, allRules)
	{
		QStringList sections = rule.split(";");
		if(sections.size() == 3 && sections[2].toLower() == "true")
            result << sections[0] + ";" + sections[1];
	}
    return result;
}

void Setting::setAbbreviationRules(const QStringList& rules) {
	setValue("AbbreviationRules", rules.join("#"));
}

QString Setting::getCompileDate() const
{
	// this resource file is generated by running CompileDate.bat (CompileData on Mac)
	QResource resource(":/CompileDate.txt");
	QString result = (char*)resource.data();
    return result.isEmpty() ? "Unknown" : result;
}

QString Setting::getKeyGenRule() const {
    return value("KeyGenRule").toString();
}

void Setting::setKeyGenRule(const QString& rule) {
    setValue("KeyGenRule", rule);
}

bool Setting::getProtectFirstLetter() const {
    return value("ProtectFirstLetter").toBool();
}

void Setting::setProtectFirstLetter(bool firstLetter) {
    setValue("ProtectFirstLetter", firstLetter);
}

}
