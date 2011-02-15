// Copyright (c) 2011  Stefan A. van der Meer
// See "LICENSE" file included with source code for terms and conditions.

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QProgressBar>
#include <QtWebKit/QWebView>

namespace Ui
{
    class MainWindowClass;
}

// MainWindow is the root window of the app, containing the QWebView etc.
// Its only task defined here is the initial setup of those widgets.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Construct a MainWindow in which the given url will be loaded. A custom
    // page can be injected, which will be passed on to the QWebView widget.
    MainWindow(const QUrl &url, QWebPage *custom_page = 0, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindowClass *ui;
};

#endif // MAINWINDOW_H
