#ifndef CHUMBYCOMMANDHANDLER_H
#define CHUMBYCOMMANDHANDLER_H

#include <QObject>
#include <QStringList>
#include <QVariantMap>

// "Interface" for command handlers. As command handlers must be QObjects
// in order to be exposed to JavaScript, it is rather convenient to make
// this a concrete class that handlers must inherit from.
class ChumbyCommandHandler : public QObject
{
    Q_OBJECT

public:
    ChumbyCommandHandler(QObject *parent=0)
            :QObject(parent)
    {}

    // Execute a system command as requested from the JavaSCript environment
    // of a browser page.
    //   cmd is the command name, received from JavaScript;
    //   args is a list of arguments, received from JavaScript.
    //
    // Returns a QVariantMap that will be converted into a JSON object by the
    // bridge layer. What it contains is up to the command handler.
    //
    // JavaScript will be calling this function "directly":
    //   var result = ChumbyCommandHandler.run("foo", ["bar", "baz"]);
    virtual Q_INVOKABLE QVariantMap run(const QString &cmd, const QStringList &args) = 0;
};

// Very basic command handler that runs commands by executing files of the
// same name located in a specific directory.
//
// Not very secure, no sandboxing.
class SimpleCommandHandler : public ChumbyCommandHandler
{
public:
    // Construct a SimpleCommandHandler that will attempt to execute commands
    // located in the directory specified by path.
    SimpleCommandHandler(QString path, QObject *parent=0)
        : ChumbyCommandHandler(parent), path(path)
    {}

    Q_INVOKABLE QVariantMap run(const QString &cmd, const QStringList &args);

protected:
    QString path;
};


// Dummy handler that ignores commands.
class DummyCommandHandler : public ChumbyCommandHandler
{
public:
    DummyCommandHandler(QObject *parent=0)
        : ChumbyCommandHandler(parent)
    {}

    // Dummy run() that always returns an empty map.
    Q_INVOKABLE QVariantMap run(const QString &cmd, const QStringList &args)
    {
        qDebug("DummyCommandHandler::run: %s %s", qPrintable(cmd), qPrintable(args.join(" ")));
        return QVariantMap();
    }
};

#endif // CHUMBYCOMMANDHANDLER_H
