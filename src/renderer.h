// ======================= renderer.h =======================
#ifndef RENDERER_H
#define RENDERER_H

#include <windows.h>

namespace Renderer {
    void Init(HWND hwnd);
    void Shutdown();
    void Begin();
    void End();
    void RenderObjectMode();
    void RenderGameMode();
    LRESULT CALLBACK HandleInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}

#endif
