#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

Display *display;
unsigned int keycode;

void keyPress()
{
    XTestFakeKeyEvent(display, keycode, True, CurrentTime);
    XTestFakeKeyEvent(display, keycode, False, CurrentTime);
    XFlush(display);
}
int main()
{
    display = XOpenDisplay(NULL);
    keycode = XKeysymToKeycode(display, XK_A);
    keyPress();
    return 0;
}