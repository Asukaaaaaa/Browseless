#include "src/render/render.hpp"
#include "src/window/window.hpp"

int main() {
    auto window = WindowManager.create();
    auto render = new Render();
    window->rende = [render]() {
        render->drawImage("./res/test.jpg", 0.5);
        render->drawText("Hello World!", 48, 0, 0);
    };
    auto hinst = GetModuleHandle(NULL);
    if (window->init(hinst) && render->init()) {
        ShowWindow(window->hwnd, SW_SHOWDEFAULT);
        UpdateWindow(window->hwnd);

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return msg.wParam;
    };
    return 0;
}