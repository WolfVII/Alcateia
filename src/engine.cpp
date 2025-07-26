// ======================= engine.cpp =======================
#include "engine.h"
#include "renderer.h"

static Engine::Mode currentMode = Engine::OBJECT_MODE;

void Engine::Init(HWND hwnd) {
    Renderer::Init(hwnd);
}

void Engine::Update() {
    // Lógica futura
}

void Engine::Render() {
    Renderer::Begin();
    if (currentMode == OBJECT_MODE)
        Renderer::RenderObjectMode();
    else
        Renderer::RenderGameMode();
    Renderer::End();
}

void Engine::Shutdown() {
    Renderer::Shutdown();
}

LRESULT CALLBACK Engine::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        if (wParam == VK_RETURN) currentMode = GAME_MODE;
        if (wParam == VK_ESCAPE) currentMode = OBJECT_MODE;
        return 0;
    default:
        return Renderer::HandleInput(hWnd, msg, wParam, lParam);
    }
}
