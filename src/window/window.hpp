#pragma once

#include <Windows.h>

#include <functional>
#include <vector>

/**
 * 封装窗口类和样式操作
 */

class Window {
public:
    WNDCLASSEX BB = {
        sizeof(WNDCLASSEX),      // cbSize
        CS_HREDRAW | CS_VREDRAW, // style
        nullptr,                 //lpfnWndProc
        0,                       // cbClsExtra
        0,                       // cbWndExtra
        nullptr,                 // hInstance
        NULL,                    // hIcon
        NULL,                    // hCursor
        (HBRUSH)COLOR_WINDOW,    // hbrBackground
        NULL,                    // lpszMenuName
        "Window",                // lpszClassName
        NULL                     // hIconSm
    };
    HINSTANCE hInst = nullptr;
    HWND hwnd = nullptr;
    HDC hDC = nullptr;

    int width = 800;
    int height = 600;

    Window();
    ~Window();

    bool init(HINSTANCE);
    std::function<void()> rende = []() {};

    void setStyle();

    static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
};
