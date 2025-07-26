// ======================= main.cpp =======================
#include <windows.h>
#include "engine.h"

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

HWND hWnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, WindowProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "AlcateiaWnd", NULL };
    RegisterClassEx(&wc);

    hWnd = CreateWindow("AlcateiaWnd", "", WS_OVERLAPPEDWINDOW,
                        100, 100, 1280, 720, NULL, NULL, wc.hInstance, NULL);

    Engine::Init(hWnd);
    ShowWindow(hWnd, nCmdShow);

    MSG msg;
    while (true) {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            Engine::Update();
            Engine::Render();
        }
    }

    Engine::Shutdown();
    UnregisterClass("AlcateiaWnd", wc.hInstance);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return Engine::HandleMessage(hWnd, msg, wParam, lParam);
}
