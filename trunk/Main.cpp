#include "MainWindow.h"
#include <QtSingleApplication>

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);

    // Check another running instance.
    // Returns true if the message (url of the bibfile) is received
    if(app.sendMessage(argc > 1 ? argv[1] : QString()))
        return 0;

    BibFixer::MainWindow wnd;
    app.setActivationWindow(&wnd);   // make sure this app is on the top of the desktop
    wnd.resize(1000, 600);
    wnd.show();
    if(argc > 1)
        wnd.openBibFile(argv[1]);

    // receives message (url of the bibfile) from another instance
    QObject::connect(&app, SIGNAL(messageReceived(const QString&)),
                     &wnd, SLOT(openBibFile(const QString&)));

	return app.exec();
}
