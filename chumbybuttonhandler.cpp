#include "chumbybuttonhandler.h"

#include <QApplication>
#include <QKeyEvent>

// Keyboard socket access
#include <QSocketNotifier>
#include <linux/input.h>
#include <fcntl.h>


// There should be no reason to change the sockets on Chumby One (Falconwing),
// but just in case: they can be easily modified with -D compile flags
#ifndef SOCKET_CHUMBY_TOPBUTTON
    #define SOCKET_CHUMBY_TOPBUTTON   "/dev/input/by-id/soc-noserial-event-kbd"
#endif
#ifndef SOCKET_CHUMBY_WHEELBUTTON
    #define SOCKET_CHUMBY_WHEELBUTTON "/dev/input/by-id/soc-noserial-event-joystick"
#endif

int ChumbyButtonHandler::fdevice_top   = -1;
int ChumbyButtonHandler::fdevice_wheel = -1;

ChumbyButtonHandler::ChumbyButtonHandler(QObject *parent)
        :QObject(parent)
{
    //
    // Button presses are detected by reading the socket in which the device
    // generates the input events. Simpler than a keyboard driver.
    //

    // Hook up key handler for top button events.
    fdevice_top = ::open(SOCKET_CHUMBY_TOPBUTTON, O_RDONLY|O_NDELAY, 0);

    if (fdevice_top >= 0) {
        QSocketNotifier *notifier_top = new QSocketNotifier(fdevice_top, QSocketNotifier::Read, this);
        connect(notifier_top, SIGNAL(activated(int)), this, SLOT(readButton()));

        qDebug("Connected fdevice_top slot.");
    } else {
        qWarning("Failed to connect to top button socket at %s", SOCKET_CHUMBY_TOPBUTTON);
    }

    // Hook up key handler for volume wheel events.
    fdevice_wheel = ::open(SOCKET_CHUMBY_WHEELBUTTON, O_RDONLY|O_NDELAY, 0);

    if (fdevice_wheel >= 0) {
        QSocketNotifier *notifier_wheel = new QSocketNotifier(fdevice_wheel, QSocketNotifier::Read, this);
        connect(notifier_wheel, SIGNAL(activated(int)), this, SLOT(readWheel()));

        qDebug("Connected fdevice_wheel slot");
    } else {
        qWarning("Failed to connect to top button socket at %s", SOCKET_CHUMBY_WHEELBUTTON);
    }
}

ChumbyButtonHandler::~ChumbyButtonHandler()
{
    if (fdevice_top >= 0) {
        ::close(fdevice_top);
        fdevice_top = -1;
    }

    if (fdevice_wheel >= 0) {
        ::close(fdevice_wheel);
        fdevice_wheel = -1;
    }
}

//
// Input handling
//
// Because Qt's keyboard driver infrastructure was a hassle, we manually read
// keypresses and generate events that we post to the browser widget. Ugly!
//

// Process a top button press/release. Generates a keypress event of the HOME
// key in the webview widget, where a loaded page can detect it as a DOM event.
void ChumbyButtonHandler::handleTopButtonPress(bool pressed) {
    QKeyEvent *ev = new QKeyEvent(pressed ? QEvent::KeyPress : QEvent::KeyRelease,
                                  Qt::Key_Home,
                                  0);

    qApp->postEvent(parent(), ev);
}

// Process a wheel rotation. The rotation parameter is positive for up, negative
// for down. Normal rotation gives a magnitude of 1, while very fast rotation
// gives magnitudes between 2-4.
//
// Currently this handler simply generates a keypress event in the webview of
// the arrow key that matches the rotation direction (up/down). This keypress
// can be detected as a DOM event in javascript loaded in the webview.
void ChumbyButtonHandler::handleWheelEvent(int rotation) {
    QKeyEvent *ev = new QKeyEvent(QEvent::KeyPress,
                                  rotation > 0 ? Qt::Key_Up : Qt::Key_Down,
                                  0);

    qApp->postEvent(parent(), ev);
}

// Read all events from the top button socket. For every button press or release,
// call handleTopButtonPress (there is usually only one event).
void ChumbyButtonHandler::readButton()
{
    struct input_event event;
    const int event_size = sizeof(struct input_event);

    int rb = 0;

    while (true) {
        rb = ::read(fdevice_top, &event, event_size);

        if (rb != event_size) break;
        if (event.type != EV_KEY) continue;

        // This magic number is the key code the Chumby top button generates.
        if (event.code == 0x82) {
            handleTopButtonPress(event.value == 1);
        }
    }
}


// Read all events from the wheel button socket. For every rotation event
// call handleWheelEvent.
//
// The event value varies with the rotation direction and
// speed.
void ChumbyButtonHandler::readWheel()
{
    struct input_event event;
    const int event_size = sizeof(struct input_event);

    int rb = 0;

    while (true) {
        rb = ::read(fdevice_wheel, &event, event_size);

        if (rb != event_size) break;
        if (event.type != EV_REL) continue;

        if (event.code == REL_WHEEL) {
            handleWheelEvent(event.value);
        }
    }
}


