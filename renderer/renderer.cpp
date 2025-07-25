#include "renderer.h"

namespace {
    HWND hWnd = nullptr;
    const char* CLASS_NAME = "AlcateiaWindowClass";

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
        if (message == WM_DESTROY) PostQuitMessage(0);
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

namespace Renderer {
    bool InitializeWindow(HINSTANCE hInstance, int nCmdShow) {
        WNDCLASS wc = {};
        wc.lpfnWndProc = WndProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = CLASS_NAME;

        RegisterClass(&wc);

        hWnd = CreateWindowEx(0, CLASS_NAME, "Alcateia Engine",
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
            nullptr, nullptr, hInstance, nullptr);

        if (!hWnd) return false;

        ShowWindow(hWnd, nCmdShow);
        return true;
    }

    void MainLoop() {
        MSG msg = {};
        while (msg.message != WM_QUIT) {
            if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            } else {
                // Futuro: renderizar aqui
            }
        }
    }

    void Cleanup() {
        // Futuro: liberar recursos DirectX
    }
}
