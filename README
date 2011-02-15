ChumbyBrowser is a straightforward "kiosk-style" browser application designed
for [Chumby One devices](http://www.chumby.com/).
It is built using Qt's integrated WebKit browsing widgets.

#### OVERVIEW

The browser runs full-screen on the Chumby. Users can navigate pages using
the touchscreen. Input from the hardware buttons on the device is converted
into keypress events that can be handled via JavaScript.

By creating and serving your own HTML+CSS+JavaScript content, you can show the
content you want, with the behaviour you want. It can effectively replace the
Flash-based widget structure that runs on the Chumby by default.

Advanced functionality can be added to pages using the basic
JavaScript<->System communication available through a special object made
available to JavaScript by the browser through Qt's WebKit bridge.
More on this below in the usage section.


#### REQUIREMENTS

 - Chumby One (Falconwing) device
 - Qt 4.6.X or 4.7.X cross-compiled for Chumby (4.7.1 tested, 4.5 may work)
 - Cross-compilation environment for compiling the browser


#### COMPILATION

Like any other Qt application. The Qt Creator IDE can be convenient, as you
can open the .pro project file, select the appropriate location of the Qt
libraries that you have cross-compiled for Chumby, and hit "Build project".


#### USAGE

Assuming you have Qt up and running on the Chumby (ie. demo apps work), this
is straightforward. In the directory where the chbrowser binary is, run:
    $ ./chbrowser --url=http://www.example.com

I recommend you run a small web server on your local network that serves the
content for the Chumby. Websites not designed for the Chumby mostly work, but
are not very pleasant to read or navigate.

In writing pages for your Chumby to display, you can get a lot done in JScript
(performance is quite good). However, for some specific uses it is necessary
to have access to the native applications and scripts on the device. This is
facilitated by the ChumbyBrowser through an object that allows JavaScript to
semi-indirectly run specific executables and receive their output.

To give JavaScript access to a certain executable file, you must specify a
"command path" to the browser. This is a path to a directory that contains
executables/"commands" that JavaScript may run. For example, in the directory
where you have placed the chbrowser executable:

    $ mkdir commands
    $ ln -s /usr/chumby/scripts/signal_strength commands/wifi
    $ ./chbrowser --url=http://myserver/ --command-path=./commands/

This makes a Chumby script, which returns some XML with information about the
device's wireless connection, available to JavaScript as the command "wifi".
You access it as follows in a script on a page loaded in the browser:
   
    var result = ChumbyCommandHandler.run("wifi", []);

    console.log("Exit code:", result.exitcode);
    console.log("stdout:",    result.stdout);
    console.log("stderr:",    result.stderr);

You can then treat the result as any other object and parse the output, etc.
Note the second argument to the run method, here an empty list. This should be
a list of strings: the command-line arguments to use when running the command.

The ChumbyCommandHandler object is in fact a Qt object exposed to the page by
the browser. It checks if the command is available and executes it in a
subprocess with the given arguments (if any), before returning the results back
to JavaScript.

Clearly this can give JavaScript a lot of power if you expose certain
executables to it as commands. Use with caution if you are loading remote
(script) content on your Chumby. It is quite easy to modify the browser to
use a different command handler that uses more sandboxing and security, if
you feel like writing one.


#### KNOWN ISSUES

 - Memory-intensive content is problematic. Many large images on a single page
   can crash the application. There is simply not enough memory available on
   the device to handle that kind of content. Not a problem for most uses.

 - Audio/Video tags can only work if you compile Qt with Phonon support. Even
   then, the Chumby appears to lack the http plugin for GStreamer, making it
   impossible to play remote content, unless you also compile said plugin
   (souphttpsrc). For sound, it may be better to utilize the `aplay` and
   `btplayd` applications available on the Chumby via the command handler.
   Other HTML5 tags like canvas do work.


#### CREDIT

Stefan A. van der Meer (stefanvandermeer@gmail.com)
See also the included LICENSE document.

Note that I am not affiliated with Chumby Industries in any way.

