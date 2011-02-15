// Copyright (c) 2011  Stefan A. van der Meer
// See "LICENSE" file included with source code for terms and conditions.

#include "mainwindow.h"
#include "ui_mainwindow.h" // Generated from .ui by qmake

#include <QKeyEvent>
#include <QUrl>
#include <QApplication>

MainWindow::MainWindow(const QUrl &url, QWebPage *custom_page, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    // Set up the interface compiled from the .ui file
    ui->setupUi(this);

    // Remove title bar and edges: we are a fullscreen/kiosk app
    setWindowFlags(Qt::FramelessWindowHint);

    // Setup the webkit browser frame
    QWebView *webview = findChild<QWebView *>("webView");
    if (webview) {
        // We can specify a custom page class for additional functionality.
        if (custom_page) {
            // Make child of webview, so it will dealloc the page on exit
            custom_page->setParent(webview);
            webview->setPage(custom_page);
        }

        qDebug("Loading URL: %s", qPrintable(url.toString()));
        webview->load(url);
        webview->show();

    } else {
        qFatal("Failed to find required 'webView' element in ui.");
    }

    QProgressBar *progress = findChild<QProgressBar *>("progressBar");
    if (progress) {
        // Progress bar should not show until we are loading something.
        progress->hide();

        // All the progress bar functions we need to use are slots,
        // so we can hook them up directly to the webview's loading signals.

        // Show progress percentage in bar
        connect(webview, SIGNAL(loadProgress(int)), progress, SLOT(setValue(int)));

        // Reset bar percentage and show
        connect(webview, SIGNAL(loadStarted()),     progress, SLOT(reset()));
        connect(webview, SIGNAL(loadStarted()),     progress, SLOT(show()));

        // Hide when page load is finished
        connect(webview, SIGNAL(loadFinished(bool)),progress, SLOT(hide()));
    } else {
        // Technically we can do without.
        qWarning("Failed to find 'progressBar' element in ui.");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}
