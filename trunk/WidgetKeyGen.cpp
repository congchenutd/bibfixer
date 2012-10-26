#include "WidgetKeyGen.h"
#include "DlgSettings.h"

namespace BibFixer {

WidgetKeyGen::WidgetKeyGen(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    ui.leRule->setText(Setting::getInstance("Rules.ini")->getKeyGenRule());
}

void WidgetKeyGen::save()
{
    Setting::getInstance("Rules.ini")->setKeyGenRule(ui.leRule->text().simplified());
}

}
