// keyboardEMU.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
void sellAllKits(INPUT& ip)
{
    std::cout << "selling" << std::endl;

    //Kit Starter
    emulateKey(ip, 'L');
    Sleep(100);

    std::vector<int> input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'S', 'T', 'A', 'R', 'T', 'E', 'R' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

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

    //Kit MVP
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'M', 'V', 'P' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

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


    //Kit PRO
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'P', 'R', 'O' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

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


    //Kit SNIPER
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'S', 'N', 'I', 'P', 'E', 'R'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

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


    //Kit MILITARY
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'M', 'I', 'L', 'I', 'T', 'A', 'R', 'Y'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

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

    //Kit RUSSIAN
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'R', 'U', 'S', 'S', 'I', 'A', 'N'};
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

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

    //Kit EXPERT
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'E', 'X', 'P', 'E', 'R', 'T' };
    emulateWord(ip, input);

    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

    //Selling Kit EXPERT
   //MIDWEST
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'S', 'E', 'L', 'L', ' ', '3', '3', '1', '3', '1' };
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
    
    //Kit MVP+
    emulateKey(ip, 'L');
    Sleep(100);

    input = { VK_OEM_2, 'K', 'I', 'T', ' ', 'M', 'V', 'P'};
    emulateWord(ip, input);
    emulateCapKey(ip, VK_OEM_PLUS);
    Sleep(150);

    emulateKey(ip, VK_RETURN);
    Sleep(150);

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
}

void test(INPUT& ip)
{
    Sleep(150);
    holdKey(ip, VK_SHIFT);
    holdKey(ip, VK_OEM_PLUS);
    Sleep(100);
    releaseKey(ip);
    releaseKey(ip);
    emulateKey(ip, VK_SHIFT);
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
    bool toggled = false;
    while (true)
    {   
        if (GetKeyState(VK_F1) & 0x8000)
        {
            afk(ip);
        }
        if(GetKeyState(VK_END) & 0x8000)
        {
            sellAllKits(ip);
        }
        if (GetKeyState(VK_INSERT) & 0x8000)
        {
          //  test(ip);
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

    // Exit normally
    return 0;
}

