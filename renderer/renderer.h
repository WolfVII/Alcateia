#pragma once
#include <windows.h>

namespace Renderer {
    bool InitializeWindow(HINSTANCE hInstance, int nCmdShow);
    void MainLoop();
    void Cleanup();
}
