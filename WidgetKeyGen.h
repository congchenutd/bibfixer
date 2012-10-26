#ifndef WIDGETKEYGEN_H
#define WIDGETKEYGEN_H

#include "ui_WidgetKeyGen.h"
#include <QWidget>

namespace BibFixer {

class WidgetKeyGen : public QWidget
{
    Q_OBJECT
    
public:
    explicit WidgetKeyGen(QWidget* parent = 0);
    void save();
    
private:
    Ui::WidgetKeyGen ui;
};

}

#endif // WIDGETKEYGEN_H
