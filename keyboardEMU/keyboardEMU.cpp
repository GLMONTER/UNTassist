// keyboardEMU.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
//#define WINVER 0x0500
#include <windows.h>

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
void emulateWord(INPUT& ip, int str[])
{
    for (int i = 0; i != sizeof(str) - 1; i++)
    {
        emulateKey(ip, str[i]);
    }
}
void afk(INPUT& ip)
{
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
}
int main()
{
    // This structure will be used to create the keyboard
   // input event.
    INPUT ip;

    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    bool tpaFlag = false;
    bool healFlag = false;

    while (true)
    {
        
        if (GetKeyState(VK_F1) & 0x8000)
        {
            afk(ip);
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

            static int input[] = { VK_OEM_2, 'H', 'E', 'A', 'L' };
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

            int input[] = {VK_OEM_2, 'T', 'P', 'A', VK_SPACE, 'A'};
            emulateWord(ip, input);

            Sleep(150);
          
            emulateKey(ip, VK_RETURN);

        }
        Sleep(1);
    }

    // Exit normally
    return 0;
}

