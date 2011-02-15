// Copyright (c) 2011  Stefan A. van der Meer
// See "LICENSE" file included with source code for terms and conditions.

#include <cstdio>

#include <QtGui/QApplication>
#include "mainwindow.h"

#include <QHash>
#include <QHashIterator>

#include "chumbybuttonhandler.h"
#include "chumbycmdhandler.h"
#include "chumbypage.h"

void printUsage()
{
    printf("Usage: chbrowser --url=http://www.example.com [--command-path=./example/path/]\n\n");

    printf("  --url           The URL to load after starting the browser.\n");
    printf("  --command-path  The JS commands path. If set, allows JavaScript to run\n");
    printf("                  executables found at the path. If not set, this feature\n");
    printf("                  is be disabled. See README for details.\n");
}

// Parse a list of commandline arguments for ones we want, like --url.
//   Argument       -> Function parameter
//   --url          ->   url
//   --command-path ->   cmdpath
//
// Returns true if all required args were found, false otherwise.
bool parseArguments(QStringList &args, QUrl &url, QString &cmdpath)
{
    // This is some clunky parsing, but with only two cmdline params adding a
    // parser dependency makes no sense.

    QRegExp url_re("^--url=(.*)");
    QRegExp cmd_re("^--command-path=(.*)");

    // Map from arg -> found state. If one not found, print usage.
    QHash<QString, bool> arg_found;
    arg_found.insert("--url", false);
    arg_found.insert("--command-path", true); // not required

    foreach (QString arg, args) {
        if (url_re.exactMatch(arg)) {
            url.setEncodedUrl(url_re.cap(1).toAscii());
            arg_found.insert("--url", true);

        } else if (cmd_re.exactMatch(arg)) {
            cmdpath = cmd_re.cap(1);
            arg_found.insert("--command-path", true);
        }
    }

    // If show an error for every missing arg (if any)
    bool misses_args = false;
    QHashIterator<QString, bool> arg(arg_found);
    while (arg.hasNext()) {
        arg.next();
        if (arg.value() == false) {
            qCritical("Missing required parameter %s", qPrintable(arg.key()));
            misses_args = true;
        }
    }

    return misses_args == false;
}


int main(int argc, char *argv[])
{
    // Create app as server, equivalent to starting with -qws
    QApplication app(argc, argv, QApplication::GuiServer);

    QStringList args = app.arguments();
    QUrl url;
    QString command_path;

    bool parse_result = parseArguments(args, url, command_path);
    if (parse_result == false) {
        printUsage();
        return EXIT_FAILURE;
    }

    // Page responsible for all special chumby-related browsing behaviour.
    // If no --command-path was given, don't create a command handler.
    ChumbyPage *page = new ChumbyPage( command_path.isNull() ? 0 :
                                       new SimpleCommandHandler(command_path));

    // The button handler reads Chumby's button sockets and converts their
    // events into keypress events, which are sent to its parent. We want to
    // use the key events in Javascript, so we parent it to the page, which
    // will handle them like any other keypress (sending them to the DOM etc).
    ChumbyButtonHandler *cbh = new ChumbyButtonHandler(page);

    // The MainWindow will tell its QWebView component to use the page class we
    // inject, and load the given URL.
    MainWindow w(url, page);
    w.show();

    return app.exec();
}
