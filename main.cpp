#include "core/engine.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Engine engine;
    if (engine.Initialize(hInstance, nCmdShow)) {
        engine.Run();
    }
    engine.Shutdown();
    return 0;
}
