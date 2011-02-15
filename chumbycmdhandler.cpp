// Copyright (c) 2011  Stefan A. van der Meer
// See "LICENSE" file included with source code for terms and conditions.

#include "chumbycmdhandler.h"

#include <QProcess>
#include <QDir>

// Test if a command is valid. This is true if an executable file with the same
// name exists in the command directory.
//   cmd is the command/file name;
//   pwd is the directory in which the file must exist.
bool isValidCommand(const QString &cmd, const QDir &pwd)
{
    QStringList commands = pwd.entryList(QDir::Files | QDir::Executable | QDir::NoDotAndDotDot);
    return commands.contains(cmd);
}

// (See chumbycmdhandler.h, ChumbyCommandHandler::run)
// Attempt to execute the command by running an executable file with the same
// name located on the "command path" (which is passed in via the command line).
// Returns a map of information concerning the execution, including its stdout
// and stderr output.
QVariantMap SimpleCommandHandler::run(const QString &cmd, const QStringList &args)
{
    qDebug("SimpleCommandHandler::run: %s %s", qPrintable(cmd), qPrintable(args.join(" ")));

    QVariantMap result;
    result.insert("exitcode", 1);

    // Simple validation step
    QDir pwd(path);
    if ( !pwd.makeAbsolute()) {
        // Given path must exist
        qCritical("Invalid path for command handler: %s", qPrintable(path));
        return result;
    } else if ( !isValidCommand(cmd, pwd)) {
        // Command must be an executable file located on the given command path
        qWarning("Invalid command: %s", qPrintable(cmd));
        return result;
    }

    // Run the command in a process and wait for it to complete. Then, store
    // the output in the map that will be returned to JavaScript.

    QProcess *syscall = new QProcess(this);
    syscall->setWorkingDirectory(pwd.absolutePath());
    syscall->start(pwd.absoluteFilePath(cmd), args);

    if (!syscall->waitForFinished()) return result;

    QString stdout(syscall->readAllStandardOutput());
    QString stderr(syscall->readAllStandardError());
    QVariant exitcode(syscall->exitCode());

    result.insert("stdout", stdout);
    result.insert("stderr", stderr);
    result.insert("exitcode", exitcode);

    return result;
}
