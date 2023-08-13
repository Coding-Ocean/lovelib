#pragma comment(lib, "d3d9.lib")

#include <d3d9.h>
#include <vector>
#include <cmath>
#include "common.h"
#include "VERTEX.h"
#include "VERTEX_BUFFER.h"
#include "MAT.h"
#include "graphic.h"

IDirect3D9* D3D = NULL;
IDirect3DDevice9* Dev = NULL;

D3DMATERIAL9 Material;
D3DLIGHT9 Light2D;
MAT World;
MAT View2D;
MAT Proj2D;

std::vector<VERTEX_BUFFER> VertexBuffers;

void createGraphic()
{
    HRESULT hr;

    //Direct3Dの親分ポインタ
    D3D = Direct3DCreate9(D3D_SDK_VERSION);
    WARNING(D3D==0, "Direct3Dがつくれませんでした", "");

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
    WARNING(FAILED(hr), "Graphic Deviceがつくれませんでした", "");

    //レンダーステート
    {
        Dev->SetRenderState(D3DRS_ZENABLE, TRUE);
        Dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        Dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        Dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        Dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
        Dev->SetRenderState(D3DRS_LIGHTING, TRUE);
        Dev->SetRenderState(D3DRS_AMBIENT, 0x0);
        Dev->LightEnable(0, TRUE);
    }

    //マテリアル初期値：白
    {
        Material.Diffuse = { 1,1,1,1 };
        Material.Ambient = { 1,1,1,1 };
    }

    //2D用ライト
    {
        Light2D.Type = D3DLIGHT_DIRECTIONAL;
        Light2D.Direction = { 0,0,1 };
        Light2D.Diffuse = { 1,1,1,1 };
    }

    //プロジェクション行列
    {
        //クライアント領域の幅と高さを得る
        RECT rect;
        GetClientRect(hWnd, &rect);
        //２D用正投影行列
        Proj2D.setOrtho((float)rect.right, (float)rect.bottom);
    }

    //頂点データ０：正方形
    {
        const int numVertices = 4;
        VERTEX vertices[numVertices] = {
            { -0.5,  0.5, 0,  0,0,-1,  0,0 },//left top
            { -0.5, -0.5, 0,  0,0,-1,  0,1 },//left bottom
            {  0.5, -0.5, 0,  0,0,-1,  1,1 },//right bottom
            {  0.5,  0.5, 0,  0,0,-1,  1,0 },//right top
        };
        createVertexBuffer(vertices, numVertices);
    }

    //頂点データ１：円
    {
        std::vector<VERTEX> vertices;
        int numVertices = 60;
        float divAngle = 3.1415926f * 2 / (numVertices);
        float radius = 0.5f;
        VERTEX temp;
        for (int i = 0; i < numVertices; i++) {
            temp.x = cos(divAngle * i) * radius;
            temp.y = sin(divAngle * i) * radius;
            vertices.emplace_back(temp);
        }
        createVertexBuffer(vertices.data(), numVertices);
    }
}

void destroyGraphic()
{
    for (size_t i = 0; i < VertexBuffers.size(); i++) {
        VertexBuffers[i].obj->Release();
    }

    SAFE_RELEASE(Dev);
    SAFE_RELEASE(D3D);
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

void fill(float r, float g, float b, float a)
{
    Material.Diffuse = { r,g,b,a };
    Material.Ambient = { r,g,b,a };
}

void wireframe()
{
    Dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

//頂点フォーマット
#define VERTEX_FORMAT ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )

int createVertexBuffer(VERTEX* vertices, int numVertices)
{
    IDirect3DVertexBuffer9* obj;

    //頂点バッファの生成
    HRESULT hr = Dev->CreateVertexBuffer(sizeof(VERTEX) * numVertices,
        D3DUSAGE_WRITEONLY, VERTEX_FORMAT, D3DPOOL_MANAGED, &obj, NULL
    );

    //バッファにデータをコピー
    void* data = 0;
    hr = obj->Lock(0, sizeof(VERTEX) * numVertices, (void**)&data, 0);
    memcpy(data, vertices, sizeof(VERTEX) * numVertices);
    obj->Unlock();

    VertexBuffers.emplace_back(obj, numVertices);

    return int(VertexBuffers.size()) - 1;
}

int createShape(int numAngles, float ratio)
{
    //TRIANGLEFAN設定で描画するための頂点バッファをつくる
    std::vector<VERTEX> vertices;
    float divAngle = 3.1415926f * 2 / numAngles;
    //中心点
    VERTEX temp;
    vertices.emplace_back(temp);
    //周囲の頂点
    for (int i = 0; i < numAngles; i++) {
        float radius = i % 2 == 0 ? 0.5f : 0.5f * ratio;
        temp.x = -sin(divAngle * i) * radius;
        temp.y = cos(divAngle * i) * radius;
        vertices.emplace_back(temp);
    }
    //最後の頂点は１番目の頂点と同じ
    vertices.emplace_back(vertices[1]);
    //バッファをつくる。+2は中心点と最後の頂点の分
    return createVertexBuffer(vertices.data(), numAngles + 2);
}

void rect(float px, float py, float w, float h, float rad, int z)
{
    //行列
    World.identity();
    World.mulScaling(w, h, 1.0f);
    World.mulRotateZ(-rad);
    World.mulTranslate(px, -py, z / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World);
    Dev->SetTransform(D3DTS_VIEW, &View2D);
    Dev->SetTransform(D3DTS_PROJECTION, &Proj2D);
    //色
    Dev->SetLight(0, &Light2D);
    Dev->SetMaterial(&Material);
    Dev->SetTexture(0, 0);
    //頂点
    Dev->SetFVF(VERTEX_FORMAT);
    Dev->SetStreamSource(0, VertexBuffers[0].obj, 0, sizeof(VERTEX));
    //描画（最後のパラメタは描画する三角形数）
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
}

void circle(float px, float py, float diameter, int z)
{
    //行列
    World.identity();
    World.mulScaling(diameter, diameter, 1.0f);
    World.mulTranslate(px, -py, z / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World);
    Dev->SetTransform(D3DTS_VIEW, &View2D);
    Dev->SetTransform(D3DTS_PROJECTION, &Proj2D);
    //色
    Dev->SetLight(0, &Light2D);
    Dev->SetMaterial(&Material);
    Dev->SetTexture(0, 0);
    //頂点
    Dev->SetFVF(VERTEX_FORMAT);
    Dev->SetStreamSource(0, VertexBuffers[1].obj, 0, sizeof(VERTEX));
    //描画（最後のパラメタは描画する三角形数）
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, VertexBuffers[1].numVertices - 2);
}

void shape(int vertexId, float px, float py, float w, float h, float rad, int z)
{
    //行列
    World.identity();
    World.mulScaling(w, h, 1.0f);
    World.mulRotateZ(-rad);
    World.mulTranslate(px, -py, z / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World);
    Dev->SetTransform(D3DTS_VIEW, &View2D);
    Dev->SetTransform(D3DTS_PROJECTION, &Proj2D);
    //色
    Dev->SetLight(0, &Light2D);
    Dev->SetMaterial(&Material);
    Dev->SetTexture(0, 0);
    //頂点
    Dev->SetFVF(VERTEX_FORMAT);
    Dev->SetStreamSource(0, VertexBuffers[vertexId].obj, 0, sizeof(VERTEX));
    //描画（最後のパラメタは描画する三角形数）
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, VertexBuffers[vertexId].numVertices - 2);
}

