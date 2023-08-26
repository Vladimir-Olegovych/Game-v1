#include <windows.h>
#include <vector>
#include <random>
#include <gdiplus.h>

using namespace Gdiplus;
using namespace std;

//uint_16 max 65535
//uint_8 max 255

//struct
struct Snake {
    UINT16 enumSnake = 0;
    UINT16 maxEnumSnake = 2;

    UINT16 i = 0;

    short x = 50;
    short y = 50;
};
struct Apple {
    boolean spawned = true;
    short x = 0;
    short y = 0;
};
//functions
void drawSnake(HDC hdc);
void drawApple(HDC hdc);
//const
const UINT16 height = 950;
const UINT16 width = 1900;
const UINT8 speed = 25;
//snake logic
Snake snake;
Apple apple;
std::vector<Snake> arr;
//x-y
short x = width/2, y = height/2;


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
    //GDI+
    GdiplusStartupInput gdiPlusInput;
    ULONG_PTR gdiPlusToken;
    GdiplusStartup(&gdiPlusToken, &gdiPlusInput, nullptr);
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
        L"(Snake's game)",   // Window text
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
    GdiplusShutdown(gdiPlusToken);

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
            //ESC
            if (wParam == 0x1B) {
                exit(0);
                break;
            }
            //W
            if (wParam == 0x57) {
                y -= speed;
                if (y <= -50) {y = height;}
                RedrawWindow(hwnd, NULL, NULL, uMsg);
                break;
            }
            //S
            if (wParam == 0x53) {
                y += speed;
                if (y >= height) {y = -50;}
                RedrawWindow(hwnd, NULL, NULL, uMsg);     
                break;
            }
            //A
            if (wParam == 0x41) {
                x -= speed;
                if (x <= -50) {x = width;}
                RedrawWindow(hwnd, NULL, NULL, uMsg);
                break;
            }
            //D
            if (wParam == 0x44) {
                x += speed;
                if (x >= width) {x = -50;}
                RedrawWindow(hwnd, NULL, NULL, uMsg);
                break;
            }
        }
        case WM_PAINT: {
            //Update
            snake.x = x;
            snake.y = y;
            arr.push_back(snake);
            //Render
            hdc = BeginPaint(hwnd, &ps);

            if(apple.spawned) drawApple(hdc);
            drawSnake(hdc);

            EndPaint(hwnd, &ps);
            //End Render
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
void drawApple(HDC hdc) {
    Graphics gf(hdc);
    SolidBrush brush(Color(255, 0, 255, 0));

    apple.spawned = false;

    random_device rd;
    uniform_int_distribution<short> distX(1, width-70);
    uniform_int_distribution<short> distY(1, height-70);

    uniform_int_distribution<short> distA(10, 100);

    apple.x = distX(rd);
    apple.y = distY(rd);

    gf.FillEllipse(&brush, apple.x, apple.y, distA(rd), distA(rd));
}

void drawSnake(HDC hdc) {
    Graphics gf(hdc);
    SolidBrush brush(Color(100, 255, 0, 0));
    SolidBrush delBrush(Color(255, 255, 255, 255));

    //SnakeLogic
    gf.FillRectangle(&brush, x, y, 50, 50);
    if (snake.maxEnumSnake >= snake.i){ snake.i++; }
    else {
        gf.FillRectangle(&delBrush, arr[snake.enumSnake].x, arr[snake.enumSnake].y, 50, 50);
        snake.enumSnake++;
    }

    //EatAppleLogic - "math pizdec"
    if ((sqrt(pow(apple.x-x,2)+pow(apple.y-y,2))) <= 50){
        snake.maxEnumSnake += 3;
        apple.spawned = true;
    }
}