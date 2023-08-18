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
MAT View;
MAT Proj;

//Container
std::vector<VERTEX_BUFFER> VertexBuffers;
std::vector<INDEX_BUFFER> IndexBuffers;
std::vector<TEXTURE> Textures;

void createGraphic()
{
    HRESULT hr;
    HWND hWnd = FindWindow("GameWindow", NULL);
    
    //Direct3D�̐e���|�C���^
    {
        D3D = Direct3DCreate9(D3D_SDK_VERSION);
        WARNING(D3D == 0, "Direct3D������܂���ł���", "");
    }

    //�o�b�N�o�b�t�@�ƃf�v�X�o�b�t�@������
    {
        //�o�b�N�o�b�t�@�ƃf�v�X�o�b�t�@�̏���p�ӂ���
        D3DPRESENT_PARAMETERS d3dpp{};
        d3dpp.Windowed = true;
        d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
        d3dpp.EnableAutoDepthStencil = true;
        d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
        //�o�b�N�o�b�t�@�ƃf�v�X�o�b�t�@������
        hr = D3D->CreateDevice(
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hWnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &d3dpp,
            &Dev
        );
        WARNING(FAILED(hr), "Graphic Device������܂���ł���", "");
    }

    //�e�N�X�`���X�e�[�W
    {
        Dev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        Dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
        Dev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
        Dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        Dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
        Dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    }

    //�����_�[�X�e�[�g
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

    //�}�e���A�������l�F��
    {
        Material.Diffuse = { 1,1,1,1 };
        Material.Ambient = { 1,1,1,1 };
    }

    //3D�p���C�g�����l
    {
        Light.Type = D3DLIGHT_DIRECTIONAL;
        Light.Direction = { 0,0,1 };
        Light.Diffuse = { 1,1,1,1 };
    }

    //2D�p���C�g
    {
        Light2D.Type = D3DLIGHT_DIRECTIONAL;
        Light2D.Direction = { 0,0,1 };
        Light2D.Diffuse = { 1,1,1,1 };
    }

    //�s��
    {
        //View�s��
        VEC pos(0, 0, -5); VEC lookat(0, 0, 0); VEC up(0, 1, 0);
        View.camera(pos, lookat, up);
        //�N���C�A���g�̈�̕��ƍ����𓾂�
        RECT rect;
        GetClientRect(hWnd, &rect);
        //�QD�p�����e�s��
        Proj2D.setOrtho((float)rect.right, (float)rect.bottom);
        //�RD�p�ˉe�s��
        Proj.setPers(0.7f, (float)rect.right / rect.bottom, 0.1f, 30.0f);
    }

    //���_�f�[�^�O�F�����`
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

    //���_�f�[�^�P�F�~
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

    //�e�N�X�`���O�F�����P�~�P�̃e�N�X�`��
    {
        unsigned char rgba[] = { 
            0xff,0xff,0xff,0xff,
        };
        createTexture(rgba, 1, 1, "whiteDot");
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

void wireframe()
{
    Dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

void solid()
{
    Dev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void ambient(float r, float g, float b)
{
    unsigned ambient = 
        unsigned(r * 255) << 16 | 
        unsigned(g * 255) << 8 | 
        unsigned(b * 255);
    Dev->SetRenderState(D3DRS_AMBIENT, ambient);
}

void fill(float r, float g, float b, float a)
{
    Material.Diffuse = { r,g,b,a };
    Material.Ambient = { r,g,b,a };
}

void setLightDirection(float dx, float dy, float dz)
{
    float len = sqrt(dx * dx + dy * dy + dz * dz);
    Light.Direction.x = dx / len;
    Light.Direction.y = dy / len;
    Light.Direction.z = dz / len;
}

void setView(struct VEC& campos, struct VEC& lookat, struct VEC& up)
{
    View.camera(campos, lookat, up);
}

//���_�t�H�[�}�b�g
#define VERTEX_FORMAT ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )

int createVertexBuffer(VERTEX* vertices, int numVertices)
{
    IDirect3DVertexBuffer9* obj = 0;
    unsigned bufferSize = sizeof(VERTEX) * numVertices;
    
    HRESULT hr;
    //���_�o�b�t�@�I�u�W�F�N�g������
    hr = Dev->CreateVertexBuffer(bufferSize,
        0/*USAGE*/, VERTEX_FORMAT, D3DPOOL_MANAGED,
        &obj, NULL
    );
    WARNING(FAILED(hr), "VertexBuffer", "Create error");

    //���_�o�b�t�@�I�u�W�F�N�g�ɒ��_�f�[�^���R�s�[
    void* buffer = 0;
    hr = obj->Lock(0, bufferSize, (void**)&buffer, 0);
    WARNING(FAILED(hr), "VertexBuffer", "Lock error");
    memcpy(buffer, vertices, bufferSize);
    obj->Unlock();

    //���_�o�b�t�@�z��ɒǉ�
    VertexBuffers.emplace_back(obj, numVertices);

    //���_�o�b�t�@�ԍ���Ԃ�
    return int(VertexBuffers.size()) - 1;
}

int createIndexBuffer(unsigned short* indices, int numIndices)
{
    IDirect3DIndexBuffer9* obj = 0;
    unsigned bufferSize = sizeof(unsigned short) * numIndices;

    HRESULT hr;
    //�C���f�b�N�X�o�b�t�@�I�u�W�F�N�g������
    hr = Dev->CreateIndexBuffer(bufferSize,
        0/*USAGE*/, D3DFMT_INDEX16, D3DPOOL_MANAGED,
        &obj, NULL
    );
    WARNING(FAILED(hr), "IndexBuffer", "Create error");

    //�C���f�b�N�X�o�b�t�@�Ƀf�[�^���R�s�[
    WORD* buffer;
    hr = obj->Lock(0, bufferSize, (void**)&buffer, 0);
    WARNING(FAILED(hr), "IndexBuffer", "Lock error");
    memcpy(buffer, indices, bufferSize);
    obj->Unlock();

    //�C���f�b�N�X�o�b�t�@�z��ɒǉ�
    IndexBuffers.emplace_back(obj, numIndices);

    //�C���f�b�N�X�o�b�t�@�ԍ���Ԃ�
    return int(IndexBuffers.size()) - 1;
}

int createTexture(unsigned char* pixels, int texWidth, int texHeight, const char* filename)
{
    //�e�N�X�`���I�u�W�F�N�g������
    IDirect3DTexture9* obj = 0;
    HRESULT hr;
    hr = Dev->CreateTexture(texWidth, texHeight, 1,
        0/*USAGE*/, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
        &obj, NULL
    );
    WARNING(FAILED(hr), "Texture", "Create error");

    //�e�N�X�`���I�u�W�F�N�g�ɉ摜���R�s�[
    D3DLOCKED_RECT lockRect;
    hr = obj->LockRect(0, &lockRect, NULL, D3DLOCK_DISCARD);
    WARNING(FAILED(hr), "Texture", "Lock error");
    for (int y = 0; y < texHeight; y++) {
        for (int x = 0; x < texWidth; x++) {
            int i = (x + y * texWidth) * 4;
            DWORD r = pixels[i];
            DWORD g = pixels[i + 1];
            DWORD b = pixels[i + 2];
            DWORD a = pixels[i + 3];
            DWORD color = a << 24 | r << 16 | g << 8 | b;
            memcpy((BYTE*)lockRect.pBits + i, &color, sizeof(DWORD));
        }
    }
    obj->UnlockRect(0);

    //�e�N�X�`���z��֒ǉ�
    Textures.emplace_back(obj, texWidth, texHeight, filename);

    //�e�N�X�`���ԍ���Ԃ�
    return (int)Textures.size() - 1;
}

int createShape(int numAngles, float ratio)
{
    //TRIANGLEFAN�ݒ�ŕ`�悷�邽�߂̒��_�o�b�t�@������
    std::vector<VERTEX> vertices;
    float divAngle = 3.1415926f * 2 / numAngles;
    //���S�_
    VERTEX temp;
    vertices.emplace_back(temp);
    //���͂̒��_
    for (int i = 0; i < numAngles; i++) {
        float radius = i % 2 == 0 ? 0.5f : 0.5f * ratio;
        temp.x = -sin(divAngle * i) * radius;
        temp.y = cos(divAngle * i) * radius;
        vertices.emplace_back(temp);
    }
    //�Ō�̒��_�͂P�Ԗڂ̒��_�Ɠ���
    vertices.emplace_back(vertices[1]);
    //�o�b�t�@������B+2�͒��S�_�ƍŌ�̒��_�̕�
    return createVertexBuffer(vertices.data(), numAngles + 2);
}

void rect(float px, float py, float w, float h, float rad, int order)
{
    //�s��
    World2D.identity();
    World2D.mulScaling(w, h, 1.0f);
    World2D.mulRotateZ(-rad);
    World2D.mulTranslate(px, -py, order / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World2D);
    Dev->SetTransform(D3DTS_VIEW, &View2D);
    Dev->SetTransform(D3DTS_PROJECTION, &Proj2D);
    //�F
    Dev->SetLight(0, &Light2D);
    Dev->SetMaterial(&Material);
    Dev->SetTexture(0, 0);
    //���_
    Dev->SetFVF(VERTEX_FORMAT);
    Dev->SetStreamSource(0, VertexBuffers[0].obj, 0, sizeof(VERTEX));
    //�`��i�Ō�̃p�����^�͕`�悷��O�p�`���j
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
}

void circle(float px, float py, float diameter, int order)
{
    //�s��
    World2D.identity();
    World2D.mulScaling(diameter, diameter, 1.0f);
    World2D.mulTranslate(px, -py, order / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World2D);
    Dev->SetTransform(D3DTS_VIEW, &View2D);
    Dev->SetTransform(D3DTS_PROJECTION, &Proj2D);
    //�F
    Dev->SetLight(0, &Light2D);
    Dev->SetMaterial(&Material);
    Dev->SetTexture(0, 0);
    //���_
    Dev->SetFVF(VERTEX_FORMAT);
    Dev->SetStreamSource(0, VertexBuffers[1].obj, 0, sizeof(VERTEX));
    //�`��i�Ō�̃p�����^�͕`�悷��O�p�`���j
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, VertexBuffers[1].numVertices - 2);
}

void shape(int vertexId, float px, float py, float w, float h, float rad, int order)
{
    //�s��
    World2D.identity();
    World2D.mulScaling(w, h, 1.0f);
    World2D.mulRotateZ(-rad);
    World2D.mulTranslate(px, -py, order / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World2D);
    Dev->SetTransform(D3DTS_VIEW, &View2D);
    Dev->SetTransform(D3DTS_PROJECTION, &Proj2D);
    //�F
    Dev->SetLight(0, &Light2D);
    Dev->SetMaterial(&Material);
    Dev->SetTexture(0, 0);
    //���_
    Dev->SetFVF(VERTEX_FORMAT);
    Dev->SetStreamSource(0, VertexBuffers[vertexId].obj, 0, sizeof(VERTEX));
    //�`��i�Ō�̃p�����^�͕`�悷��O�p�`���j
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, VertexBuffers[vertexId].numVertices - 2);
}

int loadImage(const char* filename)
{
    //���ł�load���Ă�����A�e�N�X�`���ԍ���Ԃ�
    int n = (int)Textures.size();
    for (int i = 0; i < n; i++) {
        if (Textures[i].filename == filename) {
            return i;
        }
    }

    //�摜���[�h
    unsigned char* pixels = 0;
    int texWidth = 0;
    int texHeight = 0;
    int numBytePerPixel = 0;
    pixels = stbi_load(filename, &texWidth, &texHeight, &numBytePerPixel, 4);
    WARNING(pixels == 0, filename, "Load error");

    //�e�N�X�`��������
    int texIdx =  createTexture(pixels, texWidth, texHeight, filename);

    //���[�h�����摜�����
    stbi_image_free(pixels);

    return texIdx;
}

void image(int idx, float px, float py, float rad, float scale, int order)
{
    //�s��
    World2D.identity();
    World2D.mulScaling((float)Textures[idx].width * scale, (float)Textures[idx].height * scale, 1.0f);
    World2D.mulRotateZ(-rad);
    World2D.mulTranslate(px, -py, order/1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World2D);
    Dev->SetTransform(D3DTS_VIEW, &View2D);
    Dev->SetTransform(D3DTS_PROJECTION, &Proj2D);
    //�F
    Dev->SetLight(0, &Light2D);
    Dev->SetMaterial(&Material);
    Dev->SetTexture(0, Textures[idx].obj);
    //���_
    Dev->SetFVF(VERTEX_FORMAT);
    Dev->SetStreamSource(0, VertexBuffers[0].obj, 0, sizeof(VERTEX));
    //�`��
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
}

void model(int vertexId, int indexId, int textureId, MAT& world)
{
    //�s��
    Dev->SetTransform(D3DTS_WORLD, &world);
    Dev->SetTransform(D3DTS_VIEW, &View);
    Dev->SetTransform(D3DTS_PROJECTION, &Proj);
    //�F
    Dev->SetLight(0, &Light);
    Dev->SetMaterial(&Material);
    Dev->SetTexture(0, Textures[textureId].obj);
    //���_
    Dev->SetFVF(VERTEX_FORMAT);
    Dev->SetStreamSource(0, VertexBuffers[vertexId].obj, 0, sizeof(VERTEX));
    Dev->SetIndices(IndexBuffers[indexId].obj);
    //�`��
    Dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 
        0, 0, VertexBuffers[vertexId].numVertices,
        0,IndexBuffers[indexId].numIndices/3
    );
}



