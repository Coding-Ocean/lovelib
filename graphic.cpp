#pragma comment(lib, "d3d9.lib")

#include <d3d9.h>
#include <vector>
#include <cmath>
#include "common.h"
#include "VERTEX.h"
#include "VERTEX_BUFFER.h"
#include "INDEX_BUFFER.h"
#include "TEXTURE.h"
#include "MAT.h"
#include "VEC.h"
#include "primitive.h"
#include "graphic.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

IDirect3D9* D3D;
IDirect3DDevice9* Dev;

D3DMATERIAL9 Material;

//2D
D3DLIGHT9 Light2D;
MAT World2D;
MAT View2D;
MAT Proj2D;

//3D
D3DLIGHT9 Light;
MAT World;
MAT View;
MAT Proj;

//Container
std::vector<VERTEX_BUFFER> VertexBuffers;
std::vector<INDEX_BUFFER> IndexBuffers;
std::vector<TEXTURE> Textures;
int VtxSquare=0;
int VtxCircle=1;
int TexNone=0;

void createGraphic()
{
    HRESULT hr;
    HWND hWnd = FindWindow("GameWindow", NULL);
    
    //Direct3Dの親分ポインタ
    {
        D3D = Direct3DCreate9(D3D_SDK_VERSION);
        WARNING(D3D == 0, "Direct3Dがつくれませんでした", "");
    }

    //バックバッファとデプスバッファをつくる
    {
        //バックバッファとデプスバッファの情報を用意する
        D3DPRESENT_PARAMETERS d3dpp{};
        d3dpp.Windowed = true;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
        d3dpp.EnableAutoDepthStencil = true;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
        //バックバッファとデプスバッファをつくる
        hr = D3D->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hWnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &d3dpp,
            &Dev
        );
        WARNING(FAILED(hr), "Graphic Deviceがつくれませんでした", "");
    }

    //テクスチャステージの初期設定
    {
        //ポリゴンの色とテクスチャの色を乗算合成
        Dev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        Dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        Dev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
        //ポリゴンのアルファとテクスチャのアルファを乗算合成
        Dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        Dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        Dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
    }

    //レンダーステートの初期設定
    {
        //時計回りポリゴンを裏面とし、描画しない
        Dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
        //拡大縮小しても法線の長さを１にする
        Dev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
        //深度バッファに深度を書き込む
        Dev->SetRenderState(D3DRS_ZENABLE, TRUE);
        //半透明の合成方法
        Dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        Dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        Dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    }

    //ライトスイッチオン
    Dev->LightEnable(0, TRUE);

    //3D用ライト
    {
        Light.Type = D3DLIGHT_DIRECTIONAL;
        Light.Direction = { 0,0,1 };
        Light.Diffuse = { 1,1,1,1 };
        Light.Ambient = { 0,0,0,0 };
    }

    //2D用ライト(3D用ライトと同じ。変更しない。)
    {
        Light2D = Light;
    }

    //マテリアル
    {
        Material.Diffuse = { 1,1,1,1 };
        Material.Ambient = { 1,1,1,1 };
    }

    //行列
    {
        //View行列
        VEC pos(0, 0, -5); VEC lookat(0, 0, 0); VEC up(0, 1, 0);
        View.camera(pos, lookat, up);
        //クライアント領域の幅と高さを得る
        RECT rect;
        GetClientRect(hWnd, &rect);
        //２D用正投影行列
        Proj2D.setOrtho((float)rect.right, (float)rect.bottom);
        //３D用射影行列
        Proj.setPers(0.7f, (float)rect.right / rect.bottom, 0.1f, 30.0f);
    }

    //頂点バッファ０：正方形
    {
        VtxSquare = createVtxSquare();
    }

    //頂点バッファ１：円
    {
        VtxCircle = createVtxCircle(0.5f, 48);
    }

    //テクスチャ０：テクスチャ無し
    {
        Textures.emplace_back(nullptr, 0, 0, "TexNone");
        TexNone = 0;
    }
}

void destroyGraphic()
{
    for (size_t i = 0; i < VertexBuffers.size(); i++) {
        SAFE_RELEASE(VertexBuffers[i].obj);
    }
    for (size_t i = 0; i < IndexBuffers.size(); i++) {
        SAFE_RELEASE(IndexBuffers[i].obj);
    }
    for (size_t i = 0; i < Textures.size(); i++) {
        SAFE_RELEASE(Textures[i].obj);
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

void cullnone()
{
    Dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}
void cullcw()
{
    Dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
}
void cullccw()
{
    Dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void normalizeNormals()
{
    Dev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
}
void notNormalizeNormals()
{
    Dev->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
}

void writeDepth()
{
    Dev->SetRenderState(D3DRS_ZENABLE, TRUE);
}
void notWriteDepth()
{
    Dev->SetRenderState(D3DRS_ZENABLE, FALSE);
}

void solid()
{
    Dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
void wireframe()
{
    Dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

void lightOn()
{
    Dev->LightEnable(0, TRUE);
}

void lightOff()
{
    Dev->LightEnable(0, FALSE);
}

void lightDirection(float dx, float dy, float dz)
{
    float len = sqrt(dx * dx + dy * dy + dz * dz);
    Light.Direction.x = dx / len;
    Light.Direction.y = dy / len;
    Light.Direction.z = dz / len;
}

void lightAmbient(float r, float g, float b)
{
    Light.Ambient.r = r;
    Light.Ambient.g = g;
    Light.Ambient.b = b;
}

void fill(float r, float g, float b, float a)
{
    Material.Diffuse = { r,g,b,a };
    Material.Ambient = { r,g,b,a };
}

void setView(const VEC& campos, const VEC& lookat, const VEC& up)
{
    View.camera(campos, lookat, up);
}

//頂点フォーマット
#define VERTEX_FORMAT ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )

int createVertexBuffer(VERTEX* vertices, int numVertices)
{
    IDirect3DVertexBuffer9* obj = 0;
    unsigned bufferSize = sizeof(VERTEX) * numVertices;

    HRESULT hr;
    //頂点バッファオブジェクトをつくる
    hr = Dev->CreateVertexBuffer(bufferSize,
        0/*USAGE*/, VERTEX_FORMAT, D3DPOOL_MANAGED,
        &obj, NULL
    );
    WARNING(FAILED(hr), "VertexBuffer", "Create error");

    //頂点バッファオブジェクトに頂点データをコピー
    void* buffer = 0;
    hr = obj->Lock(0, bufferSize, (void**)&buffer, 0);
    WARNING(FAILED(hr), "VertexBuffer", "Lock error");
    memcpy(buffer, vertices, bufferSize);
    obj->Unlock();

    //頂点バッファ配列に追加
    VertexBuffers.emplace_back(obj, numVertices);

    //頂点バッファ番号を返す
    return int(VertexBuffers.size()) - 1;
}

int createIndexBuffer(unsigned short* indices, int numIndices)
{
    IDirect3DIndexBuffer9* obj = 0;
    unsigned bufferSize = sizeof(unsigned short) * numIndices;

    HRESULT hr;
    //インデックスバッファオブジェクトをつくる
    hr = Dev->CreateIndexBuffer(bufferSize,
        0/*USAGE*/, D3DFMT_INDEX16, D3DPOOL_MANAGED,
        &obj, NULL
    );
    WARNING(FAILED(hr), "IndexBuffer", "Create error");

    //インデックスバッファにデータをコピー
    void* buffer = 0;
    hr = obj->Lock(0, bufferSize, (void**)&buffer, 0);
    WARNING(FAILED(hr), "IndexBuffer", "Lock error");
    memcpy(buffer, indices, bufferSize);
    obj->Unlock();

    //インデックスバッファ配列に追加
    IndexBuffers.emplace_back(obj, numIndices);

    //インデックスバッファ番号を返す
    return int(IndexBuffers.size()) - 1;
}

int createTexture(unsigned char* pixels, int texWidth, int texHeight, const char* filename)
{
    //テクスチャオブジェクトをつくる
    IDirect3DTexture9* obj = 0;
    HRESULT hr;
    hr = Dev->CreateTexture(texWidth, texHeight, 1,
        0/*USAGE*/, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
        &obj, NULL
    );
    WARNING(FAILED(hr), "Texture", "Create error");

    //テクスチャオブジェクトに画像をコピー
    D3DLOCKED_RECT lockRect;
    hr = obj->LockRect(0, &lockRect, NULL, D3DLOCK_DISCARD);
    WARNING(FAILED(hr), "Texture", "Lock error");
    unsigned char r, g, b, a;
    unsigned int argb;
    for (int y = 0; y < texHeight; y++) {
        for (int x = 0; x < texWidth; x++) {
            //コピー元pixelデータ
            int i = (x + y * texWidth) * 4;
            r = pixels[i];
            g = pixels[i + 1];
            b = pixels[i + 2];
            a = pixels[i + 3];
            argb = a << 24 | r << 16 | g << 8 | b;
            //コピー先アドレス
            void* buffer = (unsigned char*)lockRect.pBits + i;
            memcpy(buffer, &argb, sizeof(unsigned int));
        }
    }
    obj->UnlockRect(0);

    //テクスチャ配列へ追加
    Textures.emplace_back(obj, texWidth, texHeight, filename);

    //テクスチャ番号を返す
    return (int)Textures.size() - 1;
}

void rect(float px, float py, float w, float h, float rad, int order)
{
    //行列
    World2D.identity();
    World2D.mulScaling(w, h, 1.0f);
    World2D.mulRotateZ(-rad);
    World2D.mulTranslate(px, -py, order / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World2D);
    Dev->SetTransform(D3DTS_VIEW, &View2D);
    Dev->SetTransform(D3DTS_PROJECTION, &Proj2D);
    //色
    Dev->SetLight(0, &Light2D);
    Dev->SetMaterial(&Material);
    Dev->SetTexture(0, 0);
    //頂点
    Dev->SetFVF(VERTEX_FORMAT);
    Dev->SetStreamSource(0, VertexBuffers[VtxSquare].obj, 0, sizeof(VERTEX));
    //描画（最後のパラメタは描画する三角形数）
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
}

void circle(float px, float py, float diameter, int order)
{
    //行列
    World2D.identity();
    World2D.mulScaling(diameter, diameter, 1.0f);
    World2D.mulTranslate(px, -py, order / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World2D);
    Dev->SetTransform(D3DTS_VIEW, &View2D);
    Dev->SetTransform(D3DTS_PROJECTION, &Proj2D);
    //色
    Dev->SetLight(0, &Light2D);
    Dev->SetMaterial(&Material);
    Dev->SetTexture(0, 0);
    //頂点
    Dev->SetFVF(VERTEX_FORMAT);
    Dev->SetStreamSource(0, VertexBuffers[VtxCircle].obj, 0, sizeof(VERTEX));
    //描画（最後のパラメタは描画する三角形数）
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, VertexBuffers[VtxCircle].numVertices - 2);
}

void line(float sx, float sy, float ex, float ey, float thickness, int order)
{
    //始点から終点までのベクトルと大きさ
    float vx = ex - sx;
    float vy = ey - sy;
    float length = sqrt(vx * vx + vy * vy);
    //行列（四角形の頂点を使って、線にするworld行列）
    World2D.identity();
    World2D.mulTranslate(0.5f, 0, 0);
    World2D.mulScaling(length,thickness,1);
    World2D.mulRotateZ(atan2(-vy,vx));
    World2D.mulTranslate(sx, -sy, order / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World2D);
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
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, VertexBuffers[0].numVertices - 2);
}

int createShape(int numCorners, float ratio)
{
    //TRIANGLEFAN設定で描画するための頂点バッファをつくる
    std::vector<VERTEX> vertices;
    float divAngle = 3.1415926f * 2 / numCorners;
    //中心点
    VERTEX temp;
    vertices.emplace_back(temp);
    //周囲の頂点
    for (int i = 0; i < numCorners; i++) {
        float radius = i % 2 == 0 ? 0.5f : 0.5f * ratio;
        temp.x = -sin(divAngle * i) * radius;
        temp.y = cos(divAngle * i) * radius;
        vertices.emplace_back(temp);
    }
    //最後の頂点は１番目の頂点と同じ
    vertices.emplace_back(vertices[1]);
    //バッファをつくる。+2は中心点と最後の頂点の分
    return createVertexBuffer(vertices.data(), numCorners + 2);
}

void shape(int vertexId, float px, float py, float w, float h, float rad, int order)
{
    //行列
    World2D.identity();
    World2D.mulScaling(w, h, 1.0f);
    World2D.mulRotateZ(-rad);
    World2D.mulTranslate(px, -py, order / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World2D);
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

int loadImage(const char* filename)
{
    //すでにloadしていたら、テクスチャ番号を返す
    int n = (int)Textures.size();
    for (int i = 0; i < n; i++) {
        if (Textures[i].filename == filename) {
            return i;
        }
    }

    //画像ロード
    unsigned char* pixels = 0;
    int texWidth = 0;
    int texHeight = 0;
    int numBytePerPixel = 0;
    pixels = stbi_load(filename, &texWidth, &texHeight, &numBytePerPixel, 4);
    WARNING(pixels == 0, filename, "Load error");

    //テクスチャをつくる
    int texIdx =  createTexture(pixels, texWidth, texHeight, filename);

    //ロードした画像を解放
    stbi_image_free(pixels);

    return texIdx;
}

void image(int idx, float px, float py, float rad, float scale, int order)
{
    //行列
    World2D.identity();
    World2D.mulScaling((float)Textures[idx].width * scale, (float)Textures[idx].height * scale, 1.0f);
    World2D.mulRotateZ(-rad);
    World2D.mulTranslate(px, -py, order/1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World2D);
    Dev->SetTransform(D3DTS_VIEW, &View2D);
    Dev->SetTransform(D3DTS_PROJECTION, &Proj2D);
    //色
    Dev->SetLight(0, &Light2D);
    Dev->SetMaterial(&Material);
    Dev->SetTexture(0, Textures[idx].obj);
    //頂点
    Dev->SetFVF(VERTEX_FORMAT);
    Dev->SetStreamSource(0, VertexBuffers[0].obj, 0, sizeof(VERTEX));
    //描画
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
}

void model(int vertexId, int indexId, int textureId, MAT& world)
{
    //行列
    Dev->SetTransform(D3DTS_WORLD, &world);
    Dev->SetTransform(D3DTS_VIEW, &View);
    Dev->SetTransform(D3DTS_PROJECTION, &Proj);
    //色
    Dev->SetLight(0, &Light);
    Dev->SetMaterial(&Material);
    Dev->SetTexture(0, Textures[textureId].obj);
    //頂点
    Dev->SetFVF(VERTEX_FORMAT);
    Dev->SetStreamSource(0, VertexBuffers[vertexId].obj, 0, sizeof(VERTEX));
    Dev->SetIndices(IndexBuffers[indexId].obj);
    //描画
    Dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 
        0, 0, VertexBuffers[vertexId].numVertices,
        0,IndexBuffers[indexId].numIndices/3
    );
}

int VtxAxisX, IdxCylinder;
void createLine3D(float radius, int numCorners)
{
    VtxAxisX = createVtxCylinderAxisX(radius, numCorners, 0,1);
    IdxCylinder = createIdxCylinder(numCorners);
}
void line3D(const VEC& p1, const VEC& p2)
{
    VEC v = p2 - p1;
    World.identity();
    World.mulScaling(v.mag(), 1, 1);
    v.normalize();
    World.mulRotateZ(acos(-v.y) - 3.141592f / 2);
    World.mulRotateY(-atan2(v.z, v.x));
    World.mulTranslate(p1.x, p1.y, p1.z);
    model(VtxAxisX, IdxCylinder, TexNone, World);
}


