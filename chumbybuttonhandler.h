#ifndef CHUMBYBUTTONHANDLER_H
#define CHUMBYBUTTONHANDLER_H

#include <QObject>

// Reads events from sockets specific to the Chumby, in order to send keypress
// events to its parent object.
//
// The QKeyEvents are:
//   Qt::Key_Home  for the top button on the device;
//   Qt::Key_Up    when rotating the wheel button upwards;
//   Qt::Key_Down  when rotating the wheel button downwards.
class ChumbyButtonHandler : public QObject
{
    Q_OBJECT

public:
    ChumbyButtonHandler(QObject *parent=0);
    ~ChumbyButtonHandler();

private slots:
    // Read from top button socket
    void readButton();

    // Read from wheel button socket
    void readWheel();

private:
    // Generate appropriate key event for a top button press and send to parent
    void handleTopButtonPress(bool pressed);

    // Generate key event for wheel rotation and send to parent.
    void handleWheelEvent(int rotation);

    // Socket identifiers
    static int fdevice_top;
    static int fdevice_wheel;
};

#endif // CHUMBYBUTTONHANDLER_H
