#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <gdiplus.h>

void draw(HDC hdc);

//uint_16 max 65535
const UINT16 height = 500;
const UINT16 width = 1000;
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

        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            //Render
            draw(hdc);

            return 0;
        }
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
                     return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void draw(HDC hdc) {
    boolean ads = true;
    Gdiplus::Graphics gf(hdc);
    Gdiplus::Pen pen(Gdiplus::Color(255, 255, 0, 0));
    for (int i = 0; i < 1000; i++) {
        gf.DrawLine(&pen, i, 500, i, rand() % height);
    }
}
