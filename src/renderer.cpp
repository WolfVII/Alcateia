// ======================= renderer.cpp =======================
#include "renderer.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <cmath>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

static LPDIRECT3D9 d3d = NULL;
static LPDIRECT3DDEVICE9 d3ddev = NULL;

struct CUSTOMVERTEX {
    FLOAT x, y, z;
    DWORD color;
};
#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

static float cam_angle_h = 45.0f;
static float cam_angle_v = 45.0f;
static float cam_distance = 29.0f;
static POINT lastMouse = { 0, 0 };
static bool rotating = false;

void Renderer::Init(HWND hwnd) {
    d3d = Direct3DCreate9(D3D_SDK_VERSION);

    D3DPRESENT_PARAMETERS d3dpp = {};
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferWidth = 1280;
    d3dpp.BackBufferHeight = 720;

    d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &d3ddev);
}

void Renderer::Shutdown() {
    if (d3ddev) d3ddev->Release();
    if (d3d) d3d->Release();
}

void Renderer::Begin() {
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

    D3DXMATRIX view, proj, world;
    D3DXMatrixLookAtLH(&view, &camPos, &lookAt, &upDir);
    D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(45), 1280.0f / 720.0f, 1.0f, 100.0f);
    D3DXMatrixRotationX(&world, D3DXToRadian(45));

    d3ddev->SetTransform(D3DTS_VIEW, &view);
    d3ddev->SetTransform(D3DTS_PROJECTION, &proj);
    d3ddev->SetTransform(D3DTS_WORLD, &world);
}

void Renderer::End() {
    d3ddev->EndScene();
    d3ddev->Present(NULL, NULL, NULL, NULL);
}

void Renderer::RenderObjectMode() {
    CUSTOMVERTEX verts[132];
    int index = 0;
    float size = 8.0f;

    for (int i = -8; i <= 8; i++) {
        DWORD color = (i == 0) ? D3DCOLOR_XRGB(255, 0, 0) : D3DCOLOR_XRGB(96, 96, 96);
        verts[index].x = -size;
        verts[index].y = 0;
        verts[index].z = (float)i;
        verts[index].color = color;
        index++;
        verts[index].x = size;
        verts[index].y = 0;
        verts[index].z = (float)i;
        verts[index].color = color;
        index++;
    }
    for (int i = -8; i <= 8; i++) {
        DWORD color = (i == 0) ? D3DCOLOR_XRGB(0, 0, 255) : D3DCOLOR_XRGB(96, 96, 96);
        verts[index].x = (float)i;
        verts[index].y = 0;
        verts[index].z = -size;
        verts[index].color = color;
        index++;
        verts[index].x = (float)i;
        verts[index].y = 0;
        verts[index].z = size;
        verts[index].color = color;
        index++;
    }
    verts[index].x = 0;
    verts[index].y = 0;
    verts[index].z = 0;
    verts[index].color = D3DCOLOR_XRGB(255, 255, 0);
    index++;
    verts[index].x = 0;
    verts[index].y = 8;
    verts[index].z = 0;
    verts[index].color = D3DCOLOR_XRGB(255, 255, 0);
    index++;

    d3ddev->SetFVF(CUSTOMFVF);
    d3ddev->DrawPrimitiveUP(D3DPT_LINELIST, index / 2, verts, sizeof(CUSTOMVERTEX));
}

void Renderer::RenderGameMode() {
    // Placeholder para lógica futura
}

LRESULT CALLBACK Renderer::HandleInput(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
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
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
