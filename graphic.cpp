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

    //Direct3D�̐e���|�C���^
    D3D = Direct3DCreate9(D3D_SDK_VERSION);
    WARNING(D3D==0, "Direct3D������܂���ł���", "");

    //�o�b�N�o�b�t�@�ƃf�v�X�o�b�t�@�̏���p�ӂ���
    D3DPRESENT_PARAMETERS d3dpp{};
    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.EnableAutoDepthStencil = true;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

    //�o�b�N�o�b�t�@�ƃf�v�X�o�b�t�@������
    HWND hWnd = FindWindow("GameWindow", NULL);
    hr = D3D->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        hWnd,
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp,
        &Dev
    );
    WARNING(FAILED(hr), "Graphic Device������܂���ł���", "");

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

    //2D�p���C�g
    {
        Light2D.Type = D3DLIGHT_DIRECTIONAL;
        Light2D.Direction = { 0,0,1 };
        Light2D.Diffuse = { 1,1,1,1 };
    }

    //�v���W�F�N�V�����s��
    {
        //�N���C�A���g�̈�̕��ƍ����𓾂�
        RECT rect;
        GetClientRect(hWnd, &rect);
        //�QD�p�����e�s��
        Proj2D.setOrtho((float)rect.right, (float)rect.bottom);
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

//���_�t�H�[�}�b�g
#define VERTEX_FORMAT ( D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 )

int createVertexBuffer(VERTEX* vertices, int numVertices)
{
    IDirect3DVertexBuffer9* obj;

    //���_�o�b�t�@�̐���
    HRESULT hr = Dev->CreateVertexBuffer(sizeof(VERTEX) * numVertices,
        D3DUSAGE_WRITEONLY, VERTEX_FORMAT, D3DPOOL_MANAGED, &obj, NULL
    );

    //�o�b�t�@�Ƀf�[�^���R�s�[
    void* data = 0;
    hr = obj->Lock(0, sizeof(VERTEX) * numVertices, (void**)&data, 0);
    memcpy(data, vertices, sizeof(VERTEX) * numVertices);
    obj->Unlock();

    VertexBuffers.emplace_back(obj, numVertices);

    return int(VertexBuffers.size()) - 1;
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

void rect(float px, float py, float w, float h, float rad, int z)
{
    //�s��
    World.identity();
    World.mulScaling(w, h, 1.0f);
    World.mulRotateZ(-rad);
    World.mulTranslate(px, -py, z / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World);
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

void circle(float px, float py, float diameter, int z)
{
    //�s��
    World.identity();
    World.mulScaling(diameter, diameter, 1.0f);
    World.mulTranslate(px, -py, z / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World);
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

void shape(int vertexId, float px, float py, float w, float h, float rad, int z)
{
    //�s��
    World.identity();
    World.mulScaling(w, h, 1.0f);
    World.mulRotateZ(-rad);
    World.mulTranslate(px, -py, z / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World);
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

