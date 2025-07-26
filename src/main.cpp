// main.cpp

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void renderFrame();
void cleanD3D();
void drawGrid();

// Estrutura de vértice com posição 3D e cor
struct CUSTOMVERTEX {
    FLOAT x, y, z;
    DWORD color;
};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

// Variáveis da câmera
float cam_angle_h = 45.0f;
float cam_angle_v = 45.0f;
float cam_distance = 29.0f;
POINT lastMouse = { 0, 0 };
bool rotating = false;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "AlcateiaWnd";
    RegisterClassEx(&wc);

    HWND hWnd = CreateWindow("AlcateiaWnd", "", WS_OVERLAPPEDWINDOW,
        100, 100, 1280, 720, NULL, NULL, hInstance, NULL);

    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = 1280;
    d3dpp.BackBufferHeight = 720;

    d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);

    ShowWindow(hWnd, nCmdShow);

    MSG msg;
    while (TRUE) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        } else {
            renderFrame();
        }
    }

    cleanD3D();
    UnregisterClass("AlcateiaWnd", hInstance);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_MBUTTONDOWN:
        rotating = true;
        GetCursorPos(&lastMouse);
        return 0;

    case WM_MBUTTONUP:
        rotating = false;
        return 0;

    case WM_MOUSEMOVE:
        if (rotating) {
            POINT current;
            GetCursorPos(&current);
            cam_angle_h += (current.x - lastMouse.x) * 0.5f;
            cam_angle_v += (current.y - lastMouse.y) * 0.5f;
            if (cam_angle_v < -89.0f) cam_angle_v = -89.0f;
            if (cam_angle_v > 89.0f) cam_angle_v = 89.0f;
            lastMouse = current;
        }
        return 0;

    case WM_MOUSEWHEEL:
        cam_distance -= (GET_WHEEL_DELTA_WPARAM(wParam) / 120.0f) * 5.0f;
        if (cam_distance < 2.0f) cam_distance = 2.0f;
        if (cam_distance > 50.0f) cam_distance = 50.0f;
        return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void renderFrame() {
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(32, 32, 32), 1.0f, 0);
    d3ddev->BeginScene();

    d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
    d3ddev->SetRenderState(D3DRS_COLORVERTEX, TRUE);
    d3ddev->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);

    float hRad = cam_angle_h * 3.1415926f / 180.0f;
    float vRad = cam_angle_v * 3.1415926f / 180.0f;
    float camX = cosf(vRad) * sinf(hRad) * cam_distance;
    float camY = sinf(vRad) * cam_distance;
    float camZ = cosf(vRad) * cosf(hRad) * cam_distance;

    D3DXVECTOR3 camPos = D3DXVECTOR3(camX, camY, camZ);
    D3DXVECTOR3 lookAt = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 upDir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

    D3DXMATRIX matView;
    D3DXMatrixLookAtLH(&matView, &camPos, &lookAt, &upDir);
    d3ddev->SetTransform(D3DTS_VIEW, &matView);

    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45), 1280.0f / 720.0f, 1.0f, 100.0f);
    d3ddev->SetTransform(D3DTS_PROJECTION, &matProj);

    drawGrid();

    d3ddev->EndScene();
    d3ddev->Present(NULL, NULL, NULL, NULL);
}

void drawGrid() {
    CUSTOMVERTEX verts[132];
    int index = 0;
    float size = 8.0f;

    for (int i = -8; i <= 8; i++) {
        DWORD color = (i == 0) ? D3DCOLOR_XRGB(0, 0, 255) : D3DCOLOR_XRGB(64, 64, 64); // linha Z=0 = eixo X azul
        verts[index].x = -size; verts[index].y = 0; verts[index].z = (float)i; verts[index++].color = color;
        verts[index].x =  size; verts[index].y = 0; verts[index].z = (float)i; verts[index++].color = color;
    }
    for (int i = -8; i <= 8; i++) {
        DWORD color = (i == 0) ? D3DCOLOR_XRGB(255, 0, 0) : D3DCOLOR_XRGB(64, 64, 64); // linha X=0 = eixo Y vermelho
        verts[index].x = (float)i; verts[index].y = 0; verts[index].z = -size; verts[index++].color = color;
        verts[index].x = (float)i; verts[index].y = 0; verts[index].z =  size; verts[index++].color = color;
    }

    d3ddev->SetFVF(CUSTOMFVF);
    d3ddev->DrawPrimitiveUP(D3DPT_LINELIST, index / 2, verts, sizeof(CUSTOMVERTEX));
}

void cleanD3D() {
    if (d3ddev) d3ddev->Release();
    if (d3d) d3d->Release();
}
