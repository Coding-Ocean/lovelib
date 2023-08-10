#pragma comment(lib, "d3d9.lib")

#include <d3d9.h>

IDirect3D9* D3D = NULL;
IDirect3DDevice9* Dev = NULL;

void createGraphic()
{
    HRESULT hr;

    //Direct3Dの親分ポインタ
    D3D = Direct3DCreate9(D3D_SDK_VERSION);

    //バックバッファとデプスバッファの情報を用意する
    D3DPRESENT_PARAMETERS d3dpp{};
    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.EnableAutoDepthStencil = true;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

    //バックバッファとデプスバッファをつくる
    HWND hWnd = FindWindow("GameWindow", NULL);
    hr = D3D->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &Dev
    );
}

void destroyGraphic()
{
    Dev->Release();
    D3D->Release();
}

void clear(float r, float g, float b)
{
    Dev->Clear(
        0,
        NULL,
        D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        unsigned(r * 255) << 16 | unsigned(g * 255) << 8 | unsigned(b * 255),
        1.0f,
        0
    );
    Dev->BeginScene();
}

void present()
{
    Dev->EndScene();
    Dev->Present(NULL, NULL, NULL, NULL);
}

