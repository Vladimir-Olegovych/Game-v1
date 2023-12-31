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

    short x = 0;
    short y = 0;
};
struct Apple {
    boolean spawned = true;
    UINT16 size = 0;
    short x = 0;
    short y = 0;
};
//functions
void drawSnake(HDC hdc);
void drawApple(HDC hdc);
void update();
//const
const UINT16 height = GetSystemMetrics(SM_CYSCREEN)-30;
const UINT16 width = GetSystemMetrics(SM_CXSCREEN);
const UINT8 snakeSize = 70;
const UINT8 speed = snakeSize/2;
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
    wc.hbrBackground = (HBRUSH)COLOR_BTNHIGHLIGHT;
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

        0,  //CW_USEDEFAULT
        0,  //CW_USEDEFAULT
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
            //Space
            if (wParam == 0x20) { 
                apple.spawned = true;
                break;
            }
            //Tab
            if (wParam == 0x09) {
                snake.maxEnumSnake += 5;
                break;
            }
            //W
            if (wParam == 0x57) {
                y -= speed;
                if (y <= -50) {y = height-y;}
                RedrawWindow(hwnd, NULL, NULL, uMsg);
                break;
            }
            //S
            if (wParam == 0x53) {
                y += speed;
                if (y >= height) {y = height-y;}
                RedrawWindow(hwnd, NULL, NULL, uMsg);     
                break;
            }
            //A
            if (wParam == 0x41) {
                x -= speed;
                if (x <= -50) {x = width-x;}
                RedrawWindow(hwnd, NULL, NULL, uMsg);
                break;
            }
            //D
            if (wParam == 0x44) {
                x += speed;
                if (x >= width) {x = width-x;}
                RedrawWindow(hwnd, NULL, NULL, uMsg);
                break;
            }
        }
        case WM_PAINT: {
            //Update
            update();
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
void update() {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i].x != -1 && arr[i].x == x && arr[i].y == y)
        {
            exit(0);
        }
    }
    snake.x = x;
    snake.y = y;
    arr.push_back(snake);
}
void drawApple(HDC hdc) {
    Graphics gf(hdc);
    SolidBrush brush(Color::LightBlue);

    apple.spawned = false;
    random_device rd;
    uniform_int_distribution<short> distX(1, width-150);
    uniform_int_distribution<short> distY(1, height-150);
    uniform_int_distribution<short> distA(5, 110);

    apple.size = distA(rd);
    apple.x = distX(rd);
    apple.y = distY(rd);

    if (apple.size <= 15) brush.SetColor(Color::Red);
    if (apple.size > 50 && apple.size < 100) brush.SetColor(Color::White);
    if (apple.size >= 100) brush.SetColor(Color::Blue);


    gf.FillEllipse(&brush, apple.x, apple.y, apple.size, apple.size);
}

void drawSnake(HDC hdc) {
    Graphics gf(hdc);
    SolidBrush brush(Color::White);
    SolidBrush delBrush(Color::Black);

    random_device rd;
    uniform_int_distribution<short> colR(1, 7);
    //SnakeLogic
    switch (colR(rd)) {
        case 1: brush.SetColor(Color::Red); break;
        case 2: brush.SetColor(Color::Orange); break;
        case 3: brush.SetColor(Color::Yellow); break;
        case 4: brush.SetColor(Color::Green); break;
        case 5: brush.SetColor(Color::LightBlue); break;
        case 6: brush.SetColor(Color::Blue); break;
        case 7: brush.SetColor(Color::Purple); break;
    }
    gf.FillRectangle(&brush, x, y, snakeSize, snakeSize);
    if (snake.maxEnumSnake >= snake.i){ snake.i++; }
    else {
        gf.FillRectangle(&delBrush, arr[snake.enumSnake].x, arr[snake.enumSnake].y, snakeSize, snakeSize);
        snake.x = -1;
        snake.y = -1;
        arr[snake.enumSnake] = snake;
        snake.enumSnake++;
    }

    //EatAppleLogic - "math pizdec"
    if ((sqrt(pow(apple.x-x,2)+pow(apple.y-y,2))) <= 100){
        if (apple.size <= 12) exit(0);
        if (apple.size > 15 && apple.size < 100) snake.maxEnumSnake += 2;
        if (apple.size >= 100) snake.maxEnumSnake += 10;
        apple.spawned = true;
    }
}
