#include "window.hpp"

#include "../defs.hpp"

// #define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>

/*
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Window window;
    if (window.init(hInstance)) {
        /*
    // Set WS_EX_LAYERED on this window
    SetWindowLong(hwnd,
                  GWL_EXSTYLE,
                  GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    // Make this window 70% alpha
    SetLayeredWindowAttributes(hwnd, 0, (255 * 10) / 100, LWA_ALPHA);

        Window::setDefaultStyle(window.hwnd);

        // Show the window using the flag specified by the program
        // that started the application, and send the application
        // a WM_PAINT message.
    } else
        return 1;
}*/

struct _GL_ST {
    int pf;
    PIXELFORMATDESCRIPTOR pfd;
    HGLRC hRC;

    _GL_ST() {
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
    }
    bool glInit(Window *window) {
        // hRC 初始化
        pf = ChoosePixelFormat(window->hDC, &pfd);
        if (pf == 0) {
            std::wcout << "ChoosePixelFormat() failed:  "
                       << "Cannot find a suitable pixel format."
                       << "Error" << std::endl;
            return false;
        }
        if (SetPixelFormat(window->hDC, pf, &pfd) == FALSE) {
            std::wcout << "SetPixelFormat() failed:  "
                       << "Cannot set format specified."
                       << "Error" << std::endl;
            return false;
        }
        DescribePixelFormat(window->hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

        hRC = wglCreateContext(window->hDC);
        wglMakeCurrent(window->hDC, hRC);

        if (glewInit() != GLEW_OK) {
            std::cout << "GLEW init failed." << std::endl;
            return false;
        }
        return true;
    }
    void release() {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(hRC);
    }
} GL_ST;

/*************************************************************************/

Window::Window() {
    //
}

Window::~Window() {
    GL_ST.release();
    ReleaseDC(this->hwnd, this->hDC);
    DestroyWindow(hwnd);
}

bool Window::init(HINSTANCE hInstance) {
    this->hInst = hInstance;
    this->BB.hInstance = this->hInst;
    this->BB.lpfnWndProc = WndProc;
    if (!RegisterClassEx(&BB)) {
        std::cout << "Register Window Class \'" << this->BB.lpszClassName << "\' failed!" << std::endl;
        return false;
    }
    this->hwnd = CreateWindowEx(
        0,                // no extended styles
        BB.lpszClassName, // class name
        BB.lpszClassName, // window name
        0,                // no styles
        100,              // default horizontal position
        100,              // default vertical position
        this->width,      // default width
        this->height,     // default height
        nullptr,          // no parent or owner window
        nullptr,          // class menu used
        this->hInst,      // instance handle
        NULL);

    this->setStyle();

    this->hDC = GetDC(hwnd);

    return GL_ST.glInit(this);
}

void Window::setStyle() {
    // todo
}

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    Window *_this = WindowManager.currentWindow->first;
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_SIZE:
        _this->width = LOWORD(lParam);
        _this->height = HIWORD(lParam);
        glViewport(0, 0, _this->width, _this->height);
        return 0;
    case WM_PAINT:
        static PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        _this->rende();
        EndPaint(hwnd, &ps);
        return 0;
    default:
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
