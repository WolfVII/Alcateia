#pragma once

class Engine {
public:
    Engine();
    ~Engine();

    bool Initialize(HINSTANCE hInstance, int nCmdShow);
    void Run();
    void Shutdown();
};
