// keyboardEMU.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define monter
#include <iostream>
//#define WINVER 0x0500
#include <windows.h>
#include<vector>
/*
#include "Gdiplus.h"
#include<vector>
#include <fstream>
 
#include <atlstr.h>
#include <atlimage.h>


using namespace Gdiplus;


BITMAPINFOHEADER createBitmapHeader(int width, int height)
{
    BITMAPINFOHEADER  bi;

    // create a bitmap
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    return bi;
}
bool saveToMemory(HBITMAP* hbitmap, std::vector<BYTE>& data, std::string dataFormat = "png")
{
    Gdiplus::Bitmap bmp(*hbitmap, nullptr);
    // write to IStream
    IStream* istream = nullptr;
    CreateStreamOnHGlobal(NULL, TRUE, &istream);

    // define encoding
    CLSID clsid;
    if (dataFormat.compare("bmp") == 0) { CLSIDFromString(L"{557cf400-1a04-11d3-9a73-0000f81ef32e}", &clsid); }
    else if (dataFormat.compare("jpg") == 0) { CLSIDFromString(L"{557cf401-1a04-11d3-9a73-0000f81ef32e}", &clsid); }
    else if (dataFormat.compare("gif") == 0) { CLSIDFromString(L"{557cf402-1a04-11d3-9a73-0000f81ef32e}", &clsid); }
    else if (dataFormat.compare("tif") == 0) { CLSIDFromString(L"{557cf405-1a04-11d3-9a73-0000f81ef32e}", &clsid); }
    else if (dataFormat.compare("png") == 0) { CLSIDFromString(L"{557cf406-1a04-11d3-9a73-0000f81ef32e}", &clsid); }

    Gdiplus::Status status = bmp.Save(istream, &clsid, NULL);
    if (status != Gdiplus::Status::Ok)
        return false;

    // get memory handle associated with istream
    HGLOBAL hg = NULL;
    GetHGlobalFromStream(istream, &hg);

    // copy IStream to buffer
    int bufsize = GlobalSize(hg);
    data.resize(bufsize);

    // lock & unlock memory
    LPVOID pimage = GlobalLock(hg);
    memcpy(&data[0], pimage, bufsize);
    GlobalUnlock(hg);
    istream->Release();
    return true;
}
HBITMAP GdiPlusScreenCapture(HWND hWnd)
{
    // get handles to a device context (DC)
    HDC hwindowDC = GetDC(hWnd);
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);
    //100, 165
    // define scale, height and width
    int scale = 1;
    //where to start
    int screenx = GetSystemMetrics(SM_XVIRTUALSCREEN);
    int screeny = GetSystemMetrics(SM_YVIRTUALSCREEN);
    //how big
    int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    std::cout << screeny << " " << height << std::endl;
    // create a bitmap
    HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    BITMAPINFOHEADER bi = createBitmapHeader(width, height);

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);

    // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented as wrapper functions that call HeapAlloc using a handle to the process's default heap.
    // Therefore, GlobalAlloc and LocalAlloc have greater overhead than HeapAlloc.
    DWORD dwBmpSize = ((width * bi.biBitCount + 31) / 32) * 4 * height;
    HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
    char* lpbitmap = (char*)GlobalLock(hDIB);

    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);   //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, lpbitmap, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

    // avoid memory leak
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hWnd, hwindowDC);

    return hbwindow;
}

void sc()
{
    // Initialize GDI+.
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // get the bitmap handle to the bitmap screenshot
    HWND hWnd = GetDesktopWindow();
    HBITMAP hBmp = GdiPlusScreenCapture(hWnd);

    // save as png to memory
    std::vector<BYTE> data;
    std::string dataFormat = "bmp";
  

    // save as png (method 2)
    CImage image;
    image.Attach(hBmp);
    image.Save(L"capture.png");

    GdiplusShutdown(gdiplusToken);
}
*/
void holdKey(INPUT& ip, int keyCode)
{
    // Press the keyCode key
    ip.ki.wVk = keyCode; // virtual-key code for the keyCode key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));
}
void releaseKey(INPUT& ip)
{
    // Release the keyCode key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
}
void emulateKey(INPUT& ip, int keyCode)
{
    // Press the keyCode key
    ip.ki.wVk = keyCode; // virtual-key code for the keyCode key
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the keyCode key
    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &ip, sizeof(INPUT));
}
void emulateWord(INPUT& ip, std::vector<int> input)
{
    for (int i = 0; i != input.size(); i++)
    {
        emulateKey(ip, input[i]);
    }
    //Sleep(150);
}
void emulateCapKey(INPUT& ip, const int keyCode)
{
    Sleep(150);
    holdKey(ip, VK_SHIFT);
    holdKey(ip, keyCode);
    Sleep(100);
    releaseKey(ip);
    releaseKey(ip);
    emulateKey(ip, VK_SHIFT);
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




    blacklist.push_back(col{ 121,121, 121, "Military Drum" });
    blacklist.push_back(col{ 145,145, 145, "Locker" });
    /*
    blacklist.push_back(col{ 161,37, 22, "Shotgun Ammo" });
    blacklist.push_back(col{ 149,43, 26, "Shotgun Ammo2" });
    blacklist.push_back(col{ 169,41, 18, "Shotgun Ammo2" });
    blacklist.push_back(col{ 160,44, 24, "Shotgun Ammo" });
    */
    blacklist.push_back(col{ 70,72, 70, "Surefire Mag" });
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
void dropItems(INPUT& ip, HDC& dc)
{
    //Check pixels for the correct color
    COLORREF color;

    int screenx = GetSystemMetrics(SM_CXSCREEN);
    int screeny = GetSystemMetrics(SM_CYSCREEN);

    //start at 23% screen width and end at 54 percent, size of inventory on screen
    //move by 1 percent of screen
#ifdef calvin
    float Start = 0.165;
    float End = 0.41;
    float Down = 0.001;
#endif
   
#ifdef monter
    float Start = 0.23;
    float End = 0.54;
    float Down = 0.0015;
#endif

    for (int i = screenx * Start; i < screenx * End; i += screenx * 0.01)
    {
        if ((GetKeyState(VK_F2) & 0x8000))
            return;
        for (int j = 0; j < screeny; j += screenx * Down)
        {
            if ((GetKeyState(VK_F2) & 0x8000))
                return;
            color = GetPixel(dc, i, j);

            SetCursorPos(i+5, j+5);
            col structure{ GetRValue(color), GetGValue(color), GetBValue(color), "empty" };
           // std::cout << structure.red << " " << structure.green << " " << structure.blue << std::endl;
            if (inBlackList(structure))
            {
                Sleep(250);
                holdKey(ip, VK_CONTROL);
                mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                Sleep(50);
                releaseKey(ip);
                Sleep(250);
            }
        }
    }
}
void dropUneeded(INPUT& ip, HDC& dc)
{
    //go into inventory and then scroll all the way up
    emulateKey(ip, 'G');
    //so we can scroll
    SetCursorPos(800, 800);

    Sleep(1000);
    const int minWheelMovement = 120;
    for (int i = 0; i != 50; i++)
    {
        mouse_event(MOUSEEVENTF_WHEEL, 0, 0, minWheelMovement, 0);
        Sleep(25);
    }

    dropItems(ip, dc);
    //so we can scroll

    SetCursorPos(800, 800);
    for (int i = 0; i != 20; i++)
    {
        mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -minWheelMovement, 0);
        Sleep(25);
    }
    dropItems(ip, dc);

    SetCursorPos(800, 800);
    for (int i = 0; i != 20; i++)
    {
        mouse_event(MOUSEEVENTF_WHEEL, 0, 0, -minWheelMovement, 0);
        Sleep(25);
    }
    dropItems(ip, dc);

    //leave inventory
    emulateKey(ip, 'G');
    Sleep(1000);
}
void sellAllKits(INPUT& ip, HDC& dc)
{
    //Kit Starter
    emulateKey(ip, 'L');
    Sleep(100);

    std::vector<int> input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'S', 'T', 'A', 'R', 'T', 'E', 'R' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit Starter
    //Heartbreaker
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = {VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '1', '0', '3', '7'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //Dressings
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '4', ' ', '6'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '1' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);


    //Kit NEWBIE
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'N', 'E', 'W', 'B', 'I', 'E'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit NEWBIE
    //MAPLE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '6', '3'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //SCALAR
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '1', '4', '4', '7' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //ALICE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //Kit MVP
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'M', 'V', 'P' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit MVP
    //M4A1
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'L', 'V', 'O', 'A' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MDR
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'D', 'R'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //ALICE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '6' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '4' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    //dropUneeded(ip, dc);

    //Kit PRO
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'P', 'R', 'O' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit Pro
   //MCX
    emulateKey(ip, 'L');
    Sleep(100);
    
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '3', '4' ,'2', '3'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MP5K
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '3', '6', '7', '6'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '4' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    //dropUneeded(ip, dc);


    //Kit SNIPER
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'S', 'N', 'I', 'P', 'E', 'R'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit SNIPER
   //SHOTGUN
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '3', '3' ,'3', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //SNIPER
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '3', '0', '8', '7' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '5' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //ALICE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //Kit SKILLED
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'S', 'K', 'I', 'L', 'L', 'E', 'D'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit SKILLED
   //NIGHRAIDER
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '1', '3', '7' ,'7'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);


    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '6' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '2' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    dropUneeded(ip, dc);


    //Kit MILITARY
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'M', 'I', 'L', 'I', 'T', 'A', 'R', 'Y'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit MILITARY
   //MPX
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '3', '4' ,'4', '1'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //ALICE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '5' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '5' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //Kit BEGINNER
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'B', 'E', 'G', 'I', 'N', 'N', 'E', 'R'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit BEGINNER
   //GRIZZLY
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '2', '9', '7'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //PDW
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '1', '1', '6'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    //dropUneeded(ip, dc);

    //Kit RUSSIAN
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'R', 'U', 'S', 'S', 'I', 'A', 'N'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit RUSSIAN
   //ZUBEK
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '1', '2', '2' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //SCALAR
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '1', '4', '4', '7'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //ALICE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //Kit ROOKIE
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'R', 'O', 'O', 'K', 'I', 'E'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit ROOKIE
   //AKM
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '3', '2', '4', '2'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);


    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '6' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '5' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //ALICE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    //dropUneeded(ip, dc);

    //Kit EXPERT
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'E', 'X', 'P', 'E', 'R', 'T' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit EXPERT
   //MIDWEST
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '3', '5', '7', '9' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //PDW
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '1', '1', '6' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '5' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //ALICE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //Kit GODLIKE
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'G', 'O', 'D', 'L', 'I', 'K', 'E'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit GODLIKE
   //BARRET
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '5', '8', '2', '2', '2' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MP5K
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '3', '6', '7', '6' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '5' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //ALICE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    //dropUneeded(ip, dc);

    //Kit MVP+
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'M', 'V', 'P'};
    emulateWord(ip, input);
    emulateCapKey(ip, VK_OEM_PLUS);
    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit MVP+
   //AK74N
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '3', '3', '6', '6' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BIZON
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '5', '8', '2', '0', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '5' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //ALICE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //Kit MASTER
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'M', 'A', 'S', 'T', 'E', 'R'};
    emulateWord(ip, input);
    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit MASTER
   //RPK
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '3', '0', '5', '6' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //SHOTGUN
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '3', '3' ,'3', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '5' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //ALICE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '2', '5', '3', ' ', '1' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //dropUneeded(ip, dc);

    //Kit LEGEND
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'L', 'E', 'G', 'E', 'N', 'D' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    if ((GetKeyState(VK_F2) & 0x8000))
        return;
    //Selling Kit LEGEND
   //MIDWEST
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '3', '5', '7', '9' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BIZON
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '5', '8', '2', '0', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //BLOODBAG
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '9', '5', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //MRE
    emulateKey(ip, 'L');
    Sleep(100);
    input.clear();
    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', 'M', 'R', 'E', ' ', '3' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);
    dropUneeded(ip, dc);

    //wait
    Sleep(150);
}
void afk(INPUT& ip, HDC& dc)
{
    while (!(GetKeyState(VK_F2) & 0x8000))
    {
        sellAllKits(ip, dc);
        //45 minute timer
        int timer = 2700;

        for (int i = 0; i != 2700; i++)
        {
            if ((GetKeyState(VK_F2) & 0x8000))
                break;
            Sleep(1000);
            timer--;
            system("cls");
            std::cout << "Minutes until another kit sell : " << timer / 60 << std::endl;
            std::cout << "To cancel afk and kit sell, hold F2 until the terminal is cleared" << std::endl;
        }
    }
    system("cls");
    /*
    while (!(GetKeyState(VK_F2) & 0x8000))
    {
        holdKey(ip, 'A');
        Sleep(1000);
        releaseKey(ip);
        holdKey(ip, 'W');
        Sleep(1000);
        releaseKey(ip);
        holdKey(ip, 'D');
        Sleep(1000);
        releaseKey(ip);
        holdKey(ip, 'S');
        Sleep(1000);
        releaseKey(ip);
    }
    */
}
void test()
{
   
  
   // Sleep(1000);
}
void getColor(HDC& dc)
{
    POINT point;
    GetCursorPos(&point);

    COLORREF color = GetPixel(dc, point.x, point.y);
    int _red = GetRValue(color);
    int _green = GetGValue(color);
    int _blue = GetBValue(color);
    std::cout << _red << " " << _green << " " << _blue << " " << std::endl;

    return;
}

int main()
{
    std::cout << "Press F1 to afk while selling kits" << std::endl;
    std::cout << "HOLD F2 if you are afking until the program stops doing things" << std::endl;

    std::cout << "Press Middle Mouse Wheel to do /heal" << std::endl;
    std::cout << "Press the Comma key to /tpa a" << std::endl;


    initBlacklist();
    HDC dc = GetDC(NULL);
    
   
    // This structure will be used to create the keyboard
   // input event.
    INPUT ip;
    INPUT KB;

    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    bool tpaFlag = false;
    bool healFlag = false;
    bool toggled = false;
    while (true)
    {   
        if (GetKeyState(VK_F1) & 0x8000)
        {
            afk(ip, dc);
        }

        if (GetKeyState(VK_BACK) & 0x8000)
        {
            getColor(dc);
        }
        
        //if not pressing button allow it to be toggled again
        if (!(GetKeyState(VK_MBUTTON) & 0x8000))
        {
            healFlag = false;
        }
        if (GetKeyState(VK_MBUTTON) & 0x8000 && !healFlag/*Check if high-order bit is set (1 << 15)*/)
        {
            healFlag = true;

            emulateKey(ip, 'L');
            Sleep(60);

            std::vector<int> input = { VK_OEM_2, 'H', 'E', 'A', 'L' };
            emulateWord(ip, input);

            Sleep(150);

            emulateKey(ip, VK_RETURN);
        }

        //if not pressing button allow it to be toggled again
        if (!(GetKeyState(VK_OEM_COMMA) & 0x8000))
        {
            tpaFlag = false;
        }
        if (GetKeyState(VK_OEM_COMMA) & 0x8000 && !tpaFlag/*Check if high-order bit is set (1 << 15)*/)
        {
            tpaFlag = true;

            emulateKey(ip, 'L');
            Sleep(60);

            std::vector<int> input = {VK_OEM_2, 'T', 'P', 'A', VK_SPACE, 'A'};
            emulateWord(ip, input);

            Sleep(150);
          
            emulateKey(ip, VK_RETURN);

        }
        Sleep(1);
    }
    ReleaseDC(NULL, dc);
    // Exit normally
    return 0;
}

