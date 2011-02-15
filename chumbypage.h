// Copyright (c) 2011  Stefan A. van der Meer
// See "LICENSE" file included with source code for terms and conditions.

#ifndef CHUMBYPAGE_H
#define CHUMBYPAGE_H

#include <QObject>
#include <QWebPage>

#include "chumbycmdhandler.h"

// ChumbyPage is a QWebPage that has some Chumby-specific behaviours:
//   - JavaScript console messages (console.log et al) are turned into qDebug
//     output.
//   - A ChumbyCommandHandler is exposed to JavaScript through the DOM.
class ChumbyPage : public QWebPage
{
    Q_OBJECT

public:
    // Construct a ChumbyPage. If handler is null, no CommandHandler is
    // exposed to JavaScript.
    ChumbyPage(ChumbyCommandHandler *handler=0, QObject *parent=0);
    ~ChumbyPage() {}

public slots:
    // Exposes a ChumbyCommandHandler to the page's mainFrame().
    void addCommandHandlerToFrame();

protected:
    // Logs otherwise invisible JavaScript console output.
   void javaScriptConsoleMessage ( const QString & message, int lineNumber, const QString & sourceID );

private:
   // ChumbyCommandHandler that will be exposed to the page after every page load.
   ChumbyCommandHandler *command_handler;
};

#endif // CHUMBYPAGE_H
