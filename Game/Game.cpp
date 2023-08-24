#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASSEX wc = { };
    MSG msg = { };
    HWND hwnd;


    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
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
        L"(Кто прочитал тот гей)",   // Window text
        WS_OVERLAPPEDWINDOW,       // Window style

        CW_USEDEFAULT,  //0
        CW_USEDEFAULT,  //0
        CW_USEDEFAULT,
        CW_USEDEFAULT,

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

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);


        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
                 return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}