#include "chumbypage.h"

#include <QWebFrame>

ChumbyPage::ChumbyPage(ChumbyCommandHandler *handler, QObject *parent)
        : QWebPage(parent), command_handler(handler)
{
    if (command_handler)
    {
        command_handler->setParent(this);

        // An object exposed to JavaScript must be re-added every time the DOM
        // is cleared (page load).
        mainFrame()->connect(mainFrame(), SIGNAL(javaScriptWindowObjectCleared()), this, SLOT(addCommandHandlerToFrame()));
    }
}

void ChumbyPage::addCommandHandlerToFrame()
{
    mainFrame()->addToJavaScriptWindowObject("ChumbyCommandHandler", command_handler);
}

// Show console messages and errors in debug output.
void ChumbyPage::javaScriptConsoleMessage ( const QString & message, int lineNumber, const QString & sourceID )
{
    qDebug("JS console: %s \t[%s:%d]", qPrintable(message), qPrintable(sourceID), lineNumber);
}
