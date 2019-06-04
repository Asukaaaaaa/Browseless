#pragma once

#include "dom/dom.hpp"
#include "render/render.hpp"
#include "window/window.hpp"

struct _MANAGER {
    std::pair<Window *, Dom *> *currentWindow;
    std::vector<std::pair<Window *, Dom *> *> windows;
    std::pair<Window *, Dom *> *create() {
        auto window = new std::pair<Window *, Dom *>(new Window(), new Dom());
        window->first->rende = [window]() {
            window->second->draw();
        };
        if (!windows.size())
            currentWindow = window;
        windows.push_back(window);
        return windows.back();
    }
    // todo
};

extern struct _MANAGER WindowManager;

extern Render MainRender;

// todo window render dom control