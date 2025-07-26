#include "engine.h"
#include "renderer.h"

Engine::Engine() {}
Engine::~Engine() {}

bool Engine::Initialize(HINSTANCE hInstance, int nCmdShow) {
    return Renderer::InitializeWindow(hInstance, nCmdShow);
}

void Engine::Run() {
    Renderer::MainLoop();
}

void Engine::Shutdown() {
    Renderer::Cleanup();
}
