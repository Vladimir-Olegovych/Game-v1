#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <gdiplus.h>



void draw(HDC hdc);

//uint_16 max 65535
const UINT16 height = 500;
const UINT16 width = 1000;
UINT8 speed = 25;

short x = 50, y = 50;




LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    //GDI+
    Gdiplus::GdiplusStartupInput gdiPlusInput;
    ULONG_PTR gdiPlusToken;
    Gdiplus::GdiplusStartup(&gdiPlusToken, &gdiPlusInput, nullptr);
    //Screen
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASSEX wc = { };
    MSG msg = { };
    HWND hwnd;


    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)S_WHITE2048;
    wc.hInstance = hInstance;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.lpfnWndProc = WindowProc;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassEx(&wc);

    hwnd = CreateWindowEx(
        0,                         // Optional window styles.
        CLASS_NAME,                // Window class
        L"(Math Test)",   // Window text
        WS_OVERLAPPEDWINDOW,       // Window style

        CW_USEDEFAULT,  //0
        CW_USEDEFAULT,  //0
        width,
        height,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );


    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Run the message loop.

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    Gdiplus::GdiplusShutdown(gdiPlusToken);

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    HDC hdc;
    PAINTSTRUCT ps;

    switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            UINT32 VKCode = wParam;
            //ESC
            if (VKCode == 0x1B) {
                exit(0);
            }
            //W
            if (VKCode == 0x57) {
                RedrawWindow(hwnd, NULL, NULL, uMsg);
                y -= speed;
            }
            //S
            if (VKCode == 0x53) {
                RedrawWindow(hwnd, NULL, NULL, uMsg);
                y += speed;
               
            }
            //A
            if (VKCode == 0x41) {
                RedrawWindow(hwnd, NULL, NULL, uMsg);
                x -= speed;
            }
            //D
            if (VKCode == 0x44) {
                RedrawWindow(hwnd, NULL, NULL, uMsg);
                x += speed;
            }
            break;
        }

        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            draw(hdc);
            EndPaint(hwnd, &ps);
        }

        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void draw(HDC hdc) {
    Gdiplus::Graphics gf(hdc);
    Gdiplus::SolidBrush brush(Gdiplus::Color(255, 255, 0, 0));
    //Gdiplus::SolidBrush delBrush(Gdiplus::Color(255, 255, 255, 255));
    gf.FillRectangle(&brush, x, y, 50, 50);
}