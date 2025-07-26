// ======================= engine.h =======================
#ifndef ENGINE_H
#define ENGINE_H

#include <windows.h>

namespace Engine {
    enum Mode { OBJECT_MODE, GAME_MODE };

    void Init(HWND hwnd);
    void Update();
    void Render();
    void Shutdown();
    LRESULT CALLBACK HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}

#endif
