#include "WidgetKeyGen.h"
#include "DlgSettings.h"

namespace BibFixer {

WidgetKeyGen::WidgetKeyGen(QWidget *parent) :
    QWidget(parent)
{
    _ui.setupUi(this);
    _ui.leRule->setText(Setting::getInstance("Rules.ini")->getKeyGenRule());
}

void WidgetKeyGen::save() {
    Setting::getInstance("Rules.ini")->setKeyGenRule(_ui.leRule->text().simplified());
}

}
