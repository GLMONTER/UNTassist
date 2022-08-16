// keyboardEMU.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define monter
#include <iostream>

#include<vector>

#ifdef linux
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include<X11/Xutil.h>
#endif


//so I do not have to rewrite all sleeps for linux
void Sleep(unsigned int mili)
{
    usleep(mili / 0.001);
}
void holdKey(Display *display, const unsigned int keyCode)
{
    // Press the keyCode key
    static unsigned int keycodeConverted;
    keycodeConverted = XKeysymToKeycode(display, keyCode);
    XTestFakeKeyEvent(display, keycodeConverted, True, CurrentTime);
}
void releaseKey(Display *display, const unsigned int keyCode)
{
    // Press the keyCode key
    static unsigned int keycodeConverted;
    keycodeConverted = XKeysymToKeycode(display, keyCode);
    XTestFakeKeyEvent(display, keycodeConverted, False, CurrentTime);
}
void emulateKey(Display *display, const unsigned int keyCode)
{
    // Press the keyCode key
    static unsigned int keycodeConverted;
    keycodeConverted = XKeysymToKeycode(display, keyCode);
    XTestFakeKeyEvent(display, keycodeConverted, True, CurrentTime);
    XTestFakeKeyEvent(display, keycodeConverted, False, CurrentTime);
}
void emulateWord(Display *display, std::vector<int> input)
{
    for (int i = 0; i != input.size(); i++)
    {
        emulateKey(display, input[i]);
    }
    //Sleep(150);
}
void emulateCapKey(Display *display, const unsigned int keyCode)
{
    Sleep(150);
    holdKey(display, XK_Shift_L);
    holdKey(display, keyCode);
    Sleep(100);
    releaseKey(display, keyCode);
    releaseKey(display, keyCode);
    emulateKey(display, XK_Shift_L);
}

//color structure
struct col
{
    int red;
    int green;
    int blue;
    std::string desc;
};
std::vector<col> blacklist;
//check if a color is in the blacklist
bool inBlackList(const col& structure)
{
    for (col c : blacklist)
    {
        if (structure.red == c.red && structure.green == c.green && structure.blue == c.blue)
            return true;
    }
    return false;
}
//make the color blacklist for inventory items
void initBlacklist()
{
    blacklist.push_back(col{ 56,112,57,"Dark Green Clothing" });
    blacklist.push_back(col{ 56,113,57,"Dark Green Helmet" });

    blacklist.push_back(col{ 58,115,59,"Dark Green Vest" });
    blacklist.push_back(col{ 58,116,59,"Dark Green Vest" });
    blacklist.push_back(col{ 58,117,59,"Dark Green Vest" });
    blacklist.push_back(col{ 47,100,49,"Dark Green Vest" });
    blacklist.push_back(col{ 47,101,49,"Dark Green Vest" });
    blacklist.push_back(col{ 45,95,47,"Dark Green Vest" });

    blacklist.push_back(col{ 97,114,65,"Light Green Clothing" });
    blacklist.push_back(col{ 99,117,67,"Light Green Vest" });

    blacklist.push_back(col{ 54,54,54,"Black Hood" });

    blacklist.push_back(col{ 40,86, 40, "Green Daypack" });
    blacklist.push_back(col{ 46,98, 46, "Green Daypack" });
    blacklist.push_back(col{ 46,99, 46, "Green Daypack" });
    blacklist.push_back(col{ 46,100, 46, "Green Daypack" });
    blacklist.push_back(col{ 46,101, 46, "Green Daypack" });
    blacklist.push_back(col{ 48,102, 48, "Green Daypack" });
    blacklist.push_back(col{ 49,107, 49, "Green Daypack" });

    blacklist.push_back(col{ 122,122, 122, "Military Mag" });
    blacklist.push_back(col{ 123,122, 123, "Military Mag2" });
    blacklist.push_back(col{ 119,119, 119, "Military Mag2" });
    blacklist.push_back(col{ 121,119, 121, "Military Mag2" });
    blacklist.push_back(col{ 126,129, 126, "Military Mag2" });
    blacklist.push_back(col{ 113,116, 113, "Military Mag2" });
    blacklist.push_back(col{ 115,115, 115, "Military Mag2" });
    blacklist.push_back(col{ 117,118, 117, "Military Mag2" });
    blacklist.push_back(col{ 126,125, 126, "Military Mag2" });
    blacklist.push_back(col{ 117,116, 117, "Military Mag2" });

    blacklist.push_back(col{ 121,121, 121, "Military Drum" });
    blacklist.push_back(col{ 145,145, 145, "Locker" });
    /*
    blacklist.push_back(col{ 161,37, 22, "Shotgun Ammo" });
    blacklist.push_back(col{ 149,43, 26, "Shotgun Ammo2" });
    blacklist.push_back(col{ 169,41, 18, "Shotgun Ammo2" });
    blacklist.push_back(col{ 160,44, 24, "Shotgun Ammo" });
    */
    blacklist.push_back(col{ 70,72, 70, "Surefire Mag" });
    blacklist.push_back(col{ 70,75, 70, "Surefire Mag" });
    blacklist.push_back(col{ 73,74, 73, "Surefire Mag" });
    blacklist.push_back(col{ 71,75, 71, "Surefire Mag" });
    blacklist.push_back(col{ 70,74, 70, "Surefire Mag" });

    blacklist.push_back(col{ 50,51, 50, "HK Mag" });
    blacklist.push_back(col{ 51,53, 51, "HK Mag" });
    blacklist.push_back(col{ 51,54, 51, "HK Mag" });
    blacklist.push_back(col{ 52,54, 52, "HK Mag" });
    blacklist.push_back(col{ 52,53, 52, "HK Mag" });

    blacklist.push_back(col{ 79,81, 79, "Grizzly Mag" });
    blacklist.push_back(col{ 50,50, 50, "Scalar Mag" });
    blacklist.push_back(col{ 52,52, 52, "Scalar Mag" });
    blacklist.push_back(col{ 53,53, 53, "Scalar Mag" });

    blacklist.push_back(col{ 96,96, 96, "Ranger Drum" });
    blacklist.push_back(col{ 46,46, 46, "SV 98 Mag" });

    blacklist.push_back(col{ 94,94, 94, "Ranger Mag" });
    blacklist.push_back(col{ 88,88, 88, "Ranger Mag2" });
    blacklist.push_back(col{ 88,87, 88, "Ranger Mag2" });
    blacklist.push_back(col{ 88,89, 88, "Ranger Mag2" });
    blacklist.push_back(col{ 86,87, 86, "Ranger Mag2" });
    blacklist.push_back(col{ 85,84, 85, "Ranger Mag2" });
    blacklist.push_back(col{ 85,83, 85, "Ranger Mag2" });
    blacklist.push_back(col{ 95,94, 95, "Ranger Mag2" });
    blacklist.push_back(col{ 91,96, 91, "Ranger Mag2" });
    blacklist.push_back(col{ 83,85, 83, "Ranger Mag2" });
    blacklist.push_back(col{ 84,82, 84, "Ranger Mag2" });

    blacklist.push_back(col{ 58,62, 58, "MPX Mag" });
    blacklist.push_back(col{ 59,61, 59, "MPX Mag" });
    blacklist.push_back(col{ 59,60, 60, "MPX Mag" });
    blacklist.push_back(col{ 61,62, 61, "MPX Mag" });
    blacklist.push_back(col{ 60,61, 60, "MPX Mag2" });
    blacklist.push_back(col{ 60,63, 60, "MPX Mag2" });
    blacklist.push_back(col{ 62,63, 62, "MPX Mag2" });
    blacklist.push_back(col{ 60,62, 60, "MPX Mag2" });
    blacklist.push_back(col{ 64,66, 64, "MPX Mag2" });

    blacklist.push_back(col{ 55,57, 55, "X Drum" });

    blacklist.push_back(col{ 69,71, 69, "AKMS MAG" });
    blacklist.push_back(col{ 69,72, 69, "AKMS MAG" });
    blacklist.push_back(col{ 68,72, 68, "AKMS MAG" });
    blacklist.push_back(col{ 70,71, 70, "AKMS MAG" });
    blacklist.push_back(col{ 70,72, 70, "AKMS MAG" });
    blacklist.push_back(col{ 73,73, 73, "AKMS MAG" });
    blacklist.push_back(col{ 74,76, 75, "AKMS MAG" });
    blacklist.push_back(col{ 73,75, 73, "AKMS MAG" });

    blacklist.push_back(col{ 66,62, 61, "RPK MAG" });
    blacklist.push_back(col{ 67,64, 61, "RPK MAG" });
    blacklist.push_back(col{ 66,63, 61, "RPK MAG" });
    blacklist.push_back(col{ 64,63, 60, "RPK MAG" });
    blacklist.push_back(col{ 65,63, 60, "RPK MAG" });
    blacklist.push_back(col{ 64,63, 61, "RPK MAG" });
    blacklist.push_back(col{ 65,63, 61, "RPK MAG" });
    blacklist.push_back(col{ 70,67, 65, "RPK MAG" });
    blacklist.push_back(col{ 67,65, 63, "RPK MAG" });
    blacklist.push_back(col{ 66,63, 60, "RPK MAG" });
    blacklist.push_back(col{ 58,60, 58, "RPK MAG" });

    blacklist.push_back(col{ 52,52, 52, "NiGHT VISION" });
    blacklist.push_back(col{ 0,255, 0, "NiGHT VISION" });

    blacklist.push_back(col{ 70,70, 70, "Bizon MAG" });
    blacklist.push_back(col{ 58,58, 58, "Bizon MAG" });
    blacklist.push_back(col{ 61,61, 61, "Bizon MAG" });
}

void scroll(Display* display, const bool up_down)
{
    if(!up_down)
    {
        XTestFakeButtonEvent(display, 5, True, CurrentTime);
        XTestFakeButtonEvent(display, 5, False, CurrentTime);
        XFlush(display);
    }
    else
    {
        XTestFakeButtonEvent(display, 4, True, CurrentTime);
        XTestFakeButtonEvent(display, 4, False, CurrentTime);
        XFlush(display);
    }
}
void click(Display* display)
{
    XTestFakeButtonEvent(display, 1, True, CurrentTime);
    XTestFakeButtonEvent(display, 1, False, CurrentTime);
    XFlush(display);
}
void SetCursorPos(Display* display, int x, int y)
{
    XWarpPointer(display, None, RootWindow (display, DefaultScreen (display)), 0, 0, 0, 0, 500, 500);
    XFlush(display);
}

Window qRoot;
Window qChild;
int qRootX;
int qRootY;
int qChildX;
int qChildY;
unsigned int qMask;

void getMousePos(Display* display, int* x, int* y)
{
    XQueryPointer(display, RootWindow (display, DefaultScreen (display)), &qRoot, &qChild, &qRootX, &qRootY, &qChildX, &qChildY, &qMask);
    *x = qRootX;
    *y = qRootY;
}
XColor GetPixel(Display* display)
{
    static XColor color;
    static XImage *image;

    image = XGetImage (display, RootWindow (display, DefaultScreen (display)), 0, 0, 1920, 1080, AllPlanes, XYPixmap);
    static int x ,y;
    getMousePos(display, &x, &y);
    color.pixel = XGetPixel(image, x -5 , y -5);
    XQueryColor (display, XDefaultColormap(display, XDefaultScreen (display)), &color);
    XDestroyImage(image);
    return color;
}
bool isMouseButtonPressed(Display* display, unsigned int bitflag)
{
    XQueryPointer(display, RootWindow (display, DefaultScreen (display)), &qRoot, &qChild, &qRootX, &qRootY, &qChildX, &qChildY, &qMask);
    if(qMask == bitflag)
        return true;
    return false;
}
bool isKeyPressed(Display* display, unsigned int identity)
{
    char keys_return[32];
    XQueryKeymap(display, keys_return);
    if(keys_return[identity] != '\0')
        return true;
    return false;
}
void dropItems(Display* display)
{
    Screen*  s = DefaultScreenOfDisplay(display);

    //Check pixels for the correct color
    XColor color;

    int screenx = s->width;
    int screeny = s->height;

    //start at 23% screen width and end at 54 percent, size of inventory on screen
    //move by 1 percent of screen



    float Start = 0.23;
    float End = 0.54;
    float Down = 0.0015;


    for (int i = screenx * Start; i < screenx * End; i += screenx * 0.01)
    {
        //8 is f2
        if (isKeyPressed(display, 8))
            return;
        for (int j = 0; j < screeny; j += screenx * Down)
        {
            if (isKeyPressed(display, 8))
                return;
            color = GetPixel(display);

            SetCursorPos(display, i+5, j+5);
            col structure{ color.red / 256, color.green / 256, color.blue / 256, "empty" };
            // std::cout << structure.red << " " << structure.green << " " << structure.blue << std::endl;
            if (inBlackList(structure))
            {
                Sleep(250);
                holdKey(display, XK_Control_L);
                click(display);
                Sleep(50);
                releaseKey(display, XK_Control_L);
                Sleep(250);
            }
        }
    }
}
void dropUneeded(Display* display)
{
    //go into inventory and then scroll all the way up
    emulateKey(display, 'G');
    //so we can scroll
    SetCursorPos(display, 800, 800);

    Sleep(1000);
    const int minWheelMovement = 120;
    for (int i = 0; i != 10; i++)
    {
        scroll(display, false);

        Sleep(25);
    }

    dropItems(display);
    //so we can scroll

    SetCursorPos(display, 800, 800);
    for (int i = 0; i != 20; i++)
    {
        scroll(display, false);


        Sleep(25);
    }
    dropItems(display);

    SetCursorPos(display, 800, 800);
#ifdef calvin
    for (int i = 0; i != 13; i++)
    {
        scroll(display, false);


        Sleep(25);
    }
#endif
#ifdef monter
    for (int i = 0; i != 20; i++)
    {
        scroll(display, false);
        Sleep(25);
    }
#endif
    dropItems(display);

    //leave inventory
    emulateKey(display, 'G');
    Sleep(1000);
}
void sellAllKits(Display* display)
{
    //Kit Starter
    emulateKey(display, 'L');
    Sleep(100);

    std::vector<int> input = { '/', 'K', 'I', 'T', ' ', 'S', 'T', 'A', 'R', 'T', 'E', 'R' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if (isKeyPressed(display, 8))
        return;
    //Selling Kit Starter
    //Heartbreaker
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = {'/', 'S', 'E', 'L', 'L', ' ', '1', '0', '3', '7'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //Dressings
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '4', ' ', '6'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '1' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);


    //Kit NEWBIE
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'N', 'E', 'W', 'B', 'I', 'E'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit NEWBIE
    //MAPLE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '6', '3'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //SCALAR
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '1', '4', '4', '7' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //ALICE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //Kit MVP
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'M', 'V', 'P' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit MVP
    //M4A1
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'L', 'V', 'O', 'A' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MDR
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'D', 'R'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //ALICE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '6' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '4' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    //dropUneeded(ip, dc);

    //Kit PRO
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'P', 'R', 'O' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit Pro
    //MCX
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '4' ,'2', '3'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MP5K
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '6', '7', '6'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '4' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    //dropUneeded(ip, dc);


    //Kit SNIPER
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'S', 'N', 'I', 'P', 'E', 'R'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit SNIPER
    //SHOTGUN
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '3' ,'3', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //SNIPER
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '0', '8', '7' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '5' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //ALICE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //Kit SKILLED
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'S', 'K', 'I', 'L', 'L', 'E', 'D'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit SKILLED
    //NIGHRAIDER
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '1', '3', '7' ,'7'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);


    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '6' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '2' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    dropUneeded(display);


    //Kit MILITARY
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'M', 'I', 'L', 'I', 'T', 'A', 'R', 'Y'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit MILITARY
    //MPX
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '4' ,'4', '1'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //ALICE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '5' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '5' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //Kit BEGINNER
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'B', 'E', 'G', 'I', 'N', 'N', 'E', 'R'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit BEGINNER
    //GRIZZLY
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '2', '9', '7'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //PDW
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '1', '1', '6'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    //dropUneeded(ip, dc);

    //Kit RUSSIAN
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'R', 'U', 'S', 'S', 'I', 'A', 'N'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit RUSSIAN
    //ZUBEK
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '1', '2', '2' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //SCALAR
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '1', '4', '4', '7'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //ALICE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //Kit ROOKIE
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'R', 'O', 'O', 'K', 'I', 'E'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit ROOKIE
    //AKM
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '2', '4', '2'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);


    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '6' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '5' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //ALICE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    //dropUneeded(ip, dc);

    //Kit EXPERT
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'E', 'X', 'P', 'E', 'R', 'T' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit EXPERT
    //MIDWEST
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '5', '7', '9' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //PDW
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '1', '1', '6' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '5' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //ALICE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //Kit GODLIKE
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'G', 'O', 'D', 'L', 'I', 'K', 'E'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit GODLIKE
    //BARRET
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '5', '8', '2', '2', '2' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MP5K
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '6', '7', '6' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '5' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //ALICE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    //dropUneeded(ip, dc);

    //Kit MVP+
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'M', 'V', 'P'};
    emulateWord(display, input);
    emulateCapKey(display, '+');
    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit MVP+
    //AK74N
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '3', '6', '6' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BIZON
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '5', '8', '2', '0', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '5' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //ALICE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //Kit MASTER
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'M', 'A', 'S', 'T', 'E', 'R'};
    emulateWord(display, input);
    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit MASTER
    //RPK
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '0', '5', '6' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //SHOTGUN
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '3' ,'3', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '5' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //ALICE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //dropUneeded(ip, dc);

    //Kit LEGEND
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'L', 'E', 'G', 'E', 'N', 'D' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit LEGEND
    //MIDWEST
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '5', '7', '9' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BIZON
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '5', '8', '2', '0', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //MRE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //Kit ASSAULT
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'A', 'S', 'S', 'A', 'U', 'L', 'T'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit ASSAULT
    //M249
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '6', '2', '2' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '5' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    /*
    //Kit MEGA
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'K', 'I', 'T', ' ', 'M', 'E', 'G', 'A'};
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    if ((isKeyPressed(display, 8)))
        return;
    //Selling Kit MEGA
    //M249
    emulateKey(display, 'L');
    Sleep(100);

    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '3', '5', '9', '9' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //ALICE
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);

    //BLOODBAG
    emulateKey(display, 'L');
    Sleep(100);
    input.clear();
    input = { '/', 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '6' };
    emulateWord(display, input);

    Sleep(150);

    emulateKey(display, XK_KP_Enter);
    Sleep(150);
    */
    //last drop
    dropUneeded(display);


    //wait
    Sleep(150);
}
void afk(Display* display)
{
    while (!isKeyPressed(display, 8))
    {
        sellAllKits(display);
        //45 minute timer
        int timer = 2700;

        for (int i = 0; i != 2700; i++)
        {
            if (isKeyPressed(display, 8))
                break;
            Sleep(1000);
            timer--;
         //   system("cls");
            std::cout << "Minutes until another kit sell : " << timer / 60 << std::endl;
            std::cout << "To cancel afk and kit sell, hold F2 until the terminal is cleared" << std::endl;
        }
    }
    //system("cls");
}
void test()
{


    // Sleep(1000);
}

void getColor(Display* display)
{
    static XColor color;
    static XImage *image;

    image = XGetImage (display, RootWindow (display, DefaultScreen (display)), 0, 0, 1920, 1080, AllPlanes, XYPixmap);
    static int x ,y;
    getMousePos(display, &x, &y);
    color.pixel = XGetPixel(image, x -5 , y -5);
    XQueryColor (display, XDefaultColormap(display, XDefaultScreen (display)), &color);
    XDestroyImage(image);

   // std::cout<<color.red / 256<<" " <<color.green / 256<<" " <<color.blue / 256<<std::endl;
    //std::cout<<x<< " "<<y<<std::endl;
    //Sleep(1000);
}
int main()
{
    std::cout << "Press F1 to afk while selling kits" << std::endl;
    std::cout << "HOLD F2 if you are afking until the program stops doing things" << std::endl;

    std::cout << "Press Middle Mouse Wheel to do /heal" << std::endl;
    std::cout << "Press the Comma key to /tpa a" << std::endl;

    initBlacklist();

    Display *display;
    display = XOpenDisplay(NULL);
    /*
    while(true)
    {
        getColor(display);
    }
     */
    bool tpaFlag = false;
    bool healFlag = false;
    bool toggled = false;
    while (true)
    {
        if (isKeyPressed(display, 8))
        {
            afk(display);
        }

        if (isKeyPressed(display, 2))
        {
            getColor(display);
        }

        //if not pressing button allow it to be toggled again
        if (!isMouseButtonPressed(display, 512))
        {
            healFlag = false;
        }
        if (isMouseButtonPressed(display, 512) && !healFlag/*Check if high-order bit is set (1 << 15)*/)
        {
            healFlag = true;

            emulateKey(display, 'L');
            Sleep(60);

            std::vector<int> input = { '/', 'H', 'E', 'A', 'L' };
            emulateWord(display, input);

            Sleep(150);

            emulateKey(display, XK_KP_Enter);
        }

        //if not pressing button allow it to be toggled again
        if (!isKeyPressed(display, 7))
        {
            tpaFlag = false;
        }
        if (isKeyPressed(display, 7) && !tpaFlag/*Check if high-order bit is set (1 << 15)*/)
        {
            tpaFlag = true;

            emulateKey(display, 'L');
            Sleep(60);

            std::vector<int> input = {'/', 'T', 'P', 'A', XK_space, 'A'};
            emulateWord(display, input);

            Sleep(150);

            emulateKey(display, XK_KP_Enter);

        }
        Sleep(1);
    }
    // Exit normally
    return 0;
}
