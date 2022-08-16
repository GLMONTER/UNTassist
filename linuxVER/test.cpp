#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>
#include<X11/Xutil.h>
#include<iostream>
Display *display;

int main()
{
    display = XOpenDisplay(NULL);

    int screen = DefaultScreen(display);
    Window rootWin = RootWindow(display, screen);
    Window qRoot;
    Window qChild;
    int qRootX;
    int qRootY;
    int qChildX;
    int qChildY;
    unsigned int qMask;
// Press the keyCode key

    static unsigned int keycodeConverted;
    keycodeConverted = XKeysymToKeycode(display, XK_space);
   // XTestFakeKeyEvent(display, keycodeConverted, True, CurrentTime);
   // XTestFakeKeyEvent(display, keycodeConverted, False, CurrentTime);if(keys_return[identity] != '\0')
    XFlush(display);

    char keys_return[32];
    XQueryKeymap(display, keys_return);

    for(int i = 0; i != 31; i++)
    {
        if(keys_return[i] != '\0')
        {
            std::cout<<i<<std::endl;
        }
    }


    usleep(1000 / 0.01);
    XQueryPointer(display, rootWin, &qRoot, &qChild, &qRootX, &qRootY, &qChildX, &qChildY, &qMask);
    std::cout<<qMask<<std::endl;


    usleep(5000);
    return 0;
}