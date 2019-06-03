#include "render/render.hpp"
#include "window/window.hpp"

/*
#define DEFAULT_WIDTH 800
#define DEFAULT_HEIGHT 600

#define FT_DEFAULT_HEIGHT 48
*/

struct _MANAGER {
    Window *currentWindow;
    std::vector<Window *> windows;
    Window *create() {
        auto window = new Window();
        if (!windows.size())
            currentWindow = window;
        windows.push_back(window);
        return windows.back();
    }
    // todo
} WindowManager;

Render MainRender;
