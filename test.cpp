#include "src/defs.hpp"

int main() {
    auto window = WindowManager.create();
    auto render = &MainRender;
    auto hinst = GetModuleHandle(NULL);
    if (window->first->init(hinst) && render->init()) {
        window->second->load("./res/test.xml");
        window->second->root.compute();

        ShowWindow(window->first->hwnd, SW_SHOWDEFAULT);
        UpdateWindow(window->first->hwnd);

        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return msg.wParam;
    };
    return 0;
}