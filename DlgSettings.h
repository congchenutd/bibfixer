#ifndef DLGSETTINGS_H
#define DLGSETTINGS_H

#include "ui_DlgSettings.h"
#include "MySetting.h"

namespace BibFixer {

class Setting;
class DlgSettings : public QDialog
{
	Q_OBJECT

public:
    DlgSettings(QWidget* parent = 0);
	virtual void accept();

private slots:
	void onFont();

private:
    QFont getFont() const;
    void setFont(const QFont &font);

private:
    Ui::DlgSettings _ui;
    Setting* _setting;
};


class Setting : public MySetting<Setting>
{
public:
	Setting(const QString& fileName);

	QFont getFont() const;
	void setFont(const QFont& font);

	QStringList getFields() const;
    QStringList getSelectedFields() const;
    void setFields(const QStringList& fields);  // format: fieldname;selected(bool)

	QStringList getAbbreviationRules() const;
	QStringList getSelectedAbbreviationRules() const;
    void setAbbreviationRules(const QStringList& rules);  // format: fullname;shortname;selected

	QString getCompileDate() const;

    QString getKeyGenRule() const;
    void setKeyGenRule(const QString& rule);

    bool getProtectFirstLetter() const;
    void setProtectFirstLetter(bool firstLetter);

private:
	void loadDefaults();
};

}

#endif // DLGSETTINGS_H
