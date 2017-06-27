#ifndef WIDGETKEYGEN_H
#define WIDGETKEYGEN_H

#include "ui_WidgetKeyGen.h"
#include <QWidget>

namespace BibFixer {

// UI for editing keygen rules
class WidgetKeyGen : public QWidget
{
    Q_OBJECT
    
public:
    explicit WidgetKeyGen(QWidget* parent = 0);
    void save();
    
private:
    Ui::WidgetKeyGen _ui;
};

}

#endif // WIDGETKEYGEN_H
