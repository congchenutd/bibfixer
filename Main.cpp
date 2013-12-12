#include "MainWindow.h"
#include <QtSingleApplication>
#include <QDir>

// workaround for a bug on Mavericks
// Finder returns / as the working path of an app bundle
// but if the app is run from terminal, the path is correct
// This method calcluates the path of the bundle from the application's path
QString getCurrentPath()
{
    QDir dir(QApplication::applicationDirPath());
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    return dir.absolutePath();
}

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);

#ifdef Q_OS_OSX
    QDir::setCurrent(getCurrentPath());
#endif

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
