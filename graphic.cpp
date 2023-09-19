#pragma comment(lib, "d3d9.lib")

#include <d3d9.h>
#include <map>
#include <unordered_map>
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

IDirect3D9* D3D{};
IDirect3DDevice9* Dev{};

D3DMATERIAL9 Material{};

//2D
D3DLIGHT9 Light2D{};
MAT World2D;
MAT View2D;
MAT Proj2D;

//3D
D3DLIGHT9 Light{};
static MAT World;
MAT View;
MAT Proj;

//Container
std::vector<VERTEX_BUFFER> VertexBuffers;
std::vector<INDEX_BUFFER> IndexBuffers;
std::vector<TEXTURE> Textures;
int VtxSquare{};
int VtxCircle{};
int TexNone{};

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

        //�A���`�G�C���A�V���O
        DWORD qualityBackBuffer = 0;
        DWORD qualityZBuffer = 0;
        for (DWORD m = (DWORD)D3DMULTISAMPLE_16_SAMPLES; m > 0; m--)
        {
            //�����_�[�^�[�Q�b�g�ŃA���`�G�C���A�V���O���T�|�[�g����Ă��邩���`�F�b�N
            hr = D3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                d3dpp.BackBufferFormat, d3dpp.Windowed, (D3DMULTISAMPLE_TYPE)m,
                &qualityBackBuffer
            );
            if (FAILED(hr))continue;

            //�[�x�X�e���V���ŃA���`�G�C���A�V���O���T�|�[�g����Ă��邩���`�F�b�N
            hr = D3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
                d3dpp.AutoDepthStencilFormat, d3dpp.Windowed, (D3DMULTISAMPLE_TYPE)m,
                &qualityZBuffer
            );
            if (FAILED(hr))continue;

            //�A���`�G�C���A�V���O���g�p�ł���̂�D3DPRESENT_PARAMETERS�Ƀ^�C�v���Z�b�g����B
            d3dpp.MultiSampleType = (D3DMULTISAMPLE_TYPE)m;

            //QualityBackBuffer��QualityZBuffer�ŏ��������̒l��L���ɂ���B�ǂ�ȃp�^�[���Œl���Ԃ�̂��킩���̂ŁA���������ق������S���ƁB
            if (qualityBackBuffer < qualityZBuffer)
                d3dpp.MultiSampleQuality = qualityBackBuffer - 1;
            else
                d3dpp.MultiSampleQuality = qualityZBuffer - 1;

            break;
        }

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

    //�e�N�X�`���X�e�[�W�̏����ݒ�
    {
        //�|���S���̐F�ƃe�N�X�`���̐F����Z����
        Dev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
        Dev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
        Dev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TEXTURE);
        //�|���S���̃A���t�@�ƃe�N�X�`���̃A���t�@����Z����
        Dev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
        Dev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
        Dev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);
    }

    //�����_�[�X�e�[�g�̏����ݒ�
    {
        //���v���|���S���𗠖ʂƂ��A�`�悵�Ȃ�
        Dev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
        //�g��k�����Ă��@���̒������P�ɂ���
        Dev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
        //�[�x�o�b�t�@�ɐ[�x����������
        Dev->SetRenderState(D3DRS_ZENABLE, TRUE);
        //�������̍������@
        Dev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
        Dev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
        Dev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
        //�A���`�G�C���A�V���O�L��
        Dev->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
    }

    //���C�g�X�C�b�`�I��
    Dev->LightEnable(0, TRUE);

    //3D�p���C�g
    {
        Light.Type = D3DLIGHT_DIRECTIONAL;
        Light.Direction = { 0,0,1 };
        Light.Diffuse = { 1,1,1,1 };
        Light.Ambient = { 0,0,0,0 };
        Light.Specular = { 0.5f,0.5f,0.5f,0.5f };
    }

    //2D�p���C�g(3D�p���C�g�Ɠ����B�ύX���Ȃ��B)
    {
        Light2D.Type = D3DLIGHT_DIRECTIONAL;
        Light2D.Direction = { 0,0,1 };
        Light2D.Diffuse = { 1,1,1,1 };
    }

    //�}�e���A��
    {
        Material.Diffuse = { 1,1,1,1 };
        Material.Ambient = { 1,1,1,1 };
        Material.Specular = { 1,1,1,1 };
        Material.Power = 40;
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

    //���_�o�b�t�@�O�F�����`
    {
        VtxSquare = createVtxSquare();
    }

    //���_�o�b�t�@�P�F�~
    {
        VtxCircle = createVtxCircle(0.5f, 60);
    }

    //�e�N�X�`���O�F�e�N�X�`������
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

void specularOn()
{
    Dev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
}
void specularOff()
{
    Dev->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
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
    void* buffer = 0;
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
    unsigned char r, g, b, a;
    unsigned int argb;
    for (int y = 0; y < texHeight; y++) {
        for (int x = 0; x < texWidth; x++) {
            //�R�s�[��pixel�f�[�^
            int i = (x + y * texWidth) * 4;
            r = pixels[i];
            g = pixels[i + 1];
            b = pixels[i + 2];
            a = pixels[i + 3];
            argb = a << 24 | r << 16 | g << 8 | b;
            //�R�s�[��A�h���X
            void* buffer = (unsigned char*)lockRect.pBits + i;
            memcpy(buffer, &argb, sizeof(unsigned int));
        }
    }
    obj->UnlockRect(0);

    //�e�N�X�`���z��֒ǉ�
    Textures.emplace_back(obj, texWidth, texHeight, filename);

    //�e�N�X�`���ԍ���Ԃ�
    return (int)Textures.size() - 1;
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
    Dev->SetStreamSource(0, VertexBuffers[VtxSquare].obj, 0, sizeof(VERTEX));
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
    Dev->SetStreamSource(0, VertexBuffers[VtxCircle].obj, 0, sizeof(VERTEX));
    //�`��i�Ō�̃p�����^�͕`�悷��O�p�`���j
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, VertexBuffers[VtxCircle].numVertices - 2);
}

void line(float sx, float sy, float ex, float ey, float thickness, int order)
{
    //�n�_����I�_�܂ł̃x�N�g���Ƒ傫��
    float vx = ex - sx;
    float vy = ey - sy;
    float length = sqrt(vx * vx + vy * vy);
    //�s��i�l�p�`�̒��_���g���āA���ɂ���world�s��j
    World2D.identity();
    World2D.mulTranslate(0.5f, 0, 0);
    World2D.mulScaling(length,thickness,1);
    World2D.mulRotateZ(atan2(-vy,vx));
    World2D.mulTranslate(sx, -sy, order / 1000.0f);
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
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, VertexBuffers[0].numVertices - 2);
}

void arrow(float sx, float sy, float ex, float ey, float thickness, float size, int order) {
    if (sx == ex && sy == ey)return;
    float ang = atan2(ey - sy, ex - sx);
    float c = cos(ang);
    float s = sin(ang);
    float sizeY = size / 3;
    float ax, ay;
    //��
    line(sx, sy, ex, ey, thickness, order);
    //���P
    ax = -size * c + sizeY * -s;
    ay = -size * s + sizeY * c;
    line(ex, ey, ex + ax, ey + ay, thickness, order);
    //���Q
    ax = -size * c + -sizeY * -s;
    ay = -size * s + -sizeY * c;
    line(ex, ey, ex + ax, ey + ay, thickness, order);
    circle(ex, ey, thickness, order);
}

int createShape(int numCorners, float ratio)
{
    //TRIANGLEFAN�ݒ�ŕ`�悷�邽�߂̒��_�o�b�t�@������
    std::vector<VERTEX> vertices;
    float divAngle = 3.1415926f * 2 / numCorners;
    //���S�_
    VERTEX temp;
    vertices.emplace_back(temp);
    //���͂̒��_
    for (int i = 0; i < numCorners; i++) {
        float radius = i % 2 == 0 ? 0.5f : 0.5f * ratio;
        temp.x = -sin(divAngle * i) * radius;
        temp.y = cos(divAngle * i) * radius;
        vertices.emplace_back(temp);
    }
    //�Ō�̒��_�͂P�Ԗڂ̒��_�Ɠ���
    vertices.emplace_back(vertices[1]);
    //�o�b�t�@������B+2�͒��S�_�ƍŌ�̒��_�̕�
    return createVertexBuffer(vertices.data(), numCorners + 2);
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
    World2D.mulTranslate(px, -py, order / 1000.0f);
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


//------------------------------------------------------------------------
struct CURRENT_FONT {
    std::string name;
    int id;
};
CURRENT_FONT CurrentFont;
std::unordered_map<std::string, int> FontIdMap;
int FontIdCnt = -1;
void fontFace(const char* fontname) 
{
    auto itr = FontIdMap.find(fontname);
    if (itr != FontIdMap.end()) {
        CurrentFont.name = fontname;
        CurrentFont.id = itr->second;
    }
    else {
        FontIdCnt++;
        FontIdMap[fontname] = FontIdCnt;
        CurrentFont.name = fontname;
        CurrentFont.id = FontIdCnt;
    }
}

static int FontSize=100;
void fontSize(int size)
{
    FontSize = size;
}


class KEY {
public:
    int fontId;
    int fontSize;
    unsigned code;

    bool operator<(const KEY& key) const {
        bool b;
        if (this->fontId < key.fontId)
            b = true;
        else if (this->fontId > key.fontId)
            b = false;
        else if (this->fontSize < key.fontSize)
            b = true;
        else if (this->fontSize > key.fontSize)
            b = false;
        else if (this->code < key.code)
            b = true;
        else
            b = false;

        return b;
    }
};
std::map<KEY, TEXTURE> FontMap;

TEXTURE* createFont(KEY& key)
{
    // �t�H���g�̐���
    LOGFONT lf = { FontSize, 0, 0, 0, 0, 0, 0, 0, SHIFTJIS_CHARSET, OUT_TT_ONLY_PRECIS,
    CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FIXED_PITCH | FF_MODERN };
    strcpy_s(lf.lfFaceName, CurrentFont.name.c_str());
    HFONT hFont = CreateFontIndirect(&lf);
    WARNING(!hFont, "Font", "Create error");

    // �f�o�C�X�R���e�L�X�g�擾
    // �f�o�C�X�Ƀt�H���g���������Ȃ���GetGlyphOutline�֐��̓G���[�ƂȂ�
    HDC hdc = GetDC(NULL);
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    // �t�H���g�r�b�g�}�b�v�擾
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    GLYPHMETRICS gm;
    CONST MAT2 mat = { {0,1},{0,0},{0,0},{0,1} };
    DWORD size = GetGlyphOutline(hdc, key.code, GGO_GRAY4_BITMAP, &gm, 0, NULL, &mat);
    BYTE* fontPixels = new BYTE[size];
    GetGlyphOutline(hdc, key.code, GGO_GRAY4_BITMAP, &gm, size, fontPixels, &mat);

    // �f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
    ReleaseDC(NULL, hdc);

    //�e�N�X�`���I�u�W�F�N�g������
    IDirect3DTexture9* obj = 0;
    HRESULT hr;
    hr = Dev->CreateTexture(gm.gmCellIncX, tm.tmHeight, 1,
        0/*USAGE*/, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
        &obj, NULL
    );
    WARNING(FAILED(hr), "Texture", "Create error");

    //���p�S�p�X�y�[�X�͍����ɓ_������̂Ńe�N�X�`���ɃR�s�[���Ȃ�
    if (key.code == 0x20 || key.code == 0x8140) {
        delete[] fontPixels;
        FontMap[key] = TEXTURE(obj, gm.gmCellIncX, tm.tmHeight, "");
        return &FontMap[key];
    }

    //�R�s�[�̑O�Ƀe�N�X�`���I�u�W�F�N�g�����b�N����
    D3DLOCKED_RECT lockedRect;
    hr = obj->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD);
    WARNING(FAILED(hr), "Texture", "Lock error");
    //�t�H���g�摜�̃s�b�`(�����͂S�̔{���ɂȂ��Ă���̂ł�������)
    unsigned pitch = (gm.gmBlackBoxX + 3) & 0xfffc;
    //�R�s�[��̏����o���ʒu(����)//ofsX, ofsY
    unsigned ofsX = gm.gmptGlyphOrigin.x;
    unsigned ofsY = tm.tmAscent - gm.gmptGlyphOrigin.y;
    //���l�̒i�K (GGO_GRAY4_BITMAP�Ȃ̂�17�i�K)
    unsigned level = 17 - 1;
    //�e�N�X�`���I�u�W�F�N�g�Ƀt�H���g�摜���R�s�[
    unsigned alpha, color;
    unsigned srcY = 0, distY = 0;
    unsigned int x, y;
    for (y = 0; y < gm.gmBlackBoxY; y++) {
        srcY = y * pitch;
        distY = (ofsY + y) * lockedRect.Pitch / 4 + ofsX;
        for (x = 0; x < gm.gmBlackBoxX; x++) {
            alpha = *(fontPixels + srcY + x) * 255 / level;
            color = 0xffffff | (alpha << 24);
            memcpy((DWORD*)lockedRect.pBits + distY + x, &color, sizeof(DWORD) );
        }
    }
    obj->UnlockRect(0);
    delete[] fontPixels;
    FontMap[key] = TEXTURE(obj, gm.gmCellIncX, tm.tmHeight, "");
    return &FontMap[key];
}

float PrintX, PrintY,PrintInitX=10;
void printInit()
{
    PrintY = 5;
}
void print(const char* format, ...)
{
    float PrintX = PrintInitX;

    KEY key;
    key.fontId = CurrentFont.id;
    key.fontSize = FontSize;

    va_list args;
    va_start(args, format);
    char str[256];
    vsprintf_s(str, format, args);
    va_end(args);
    int len = (int)strlen(str);

    for (int i = 0; i < len; i++) {
        //�����R�[�h
        if (IsDBCSLeadByte(str[i])) {
            //2�o�C�g�����̃R�[�h��[�擱�R�[�h]*256 + [�����R�[�h]�ł�
            key.code = (BYTE)str[i] << 8 | (BYTE)str[i + 1];
            i++;
        }
        else {
            //1�o�C�g�����̃R�[�h��1�o�C�g�ڂ�UINT�ϊ��A
            key.code = str[i];
        }

        //key�Ńe�N�X�`����T���܂�
        TEXTURE* tex=0;
        auto itr = FontMap.find(key);
        if (itr != FontMap.end()) {
            tex = &itr->second;
        }
        else {
            tex = createFont(key);
        }

        //�s��
        World2D.identity();
        World2D.mulTranslate(0.5f, -0.5f, 0);
        World2D.mulScaling((float)tex->width, (float)tex->height, 1.0f);
        World2D.mulTranslate(PrintX, -PrintY, 0);
        Dev->SetTransform(D3DTS_WORLD, &World2D);
        Dev->SetTransform(D3DTS_VIEW, &View2D);
        Dev->SetTransform(D3DTS_PROJECTION, &Proj2D);
        //�F
        Dev->SetLight(0, &Light2D);
        Dev->SetMaterial(&Material);
        Dev->SetTexture(0, tex->obj);
        //���_
        Dev->SetFVF(VERTEX_FORMAT);
        Dev->SetStreamSource(0, VertexBuffers[0].obj, 0, sizeof(VERTEX));
        //�`��
        Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
        
        PrintX += tex->width;
    }

    PrintY += FontSize;
}

//void print(const char* format, ...)
//{
//    va_list args;
//    va_start(args, format);
//    char buf[256];
//    vsprintf_s(buf, format, args);
//    va_end(args);
//    print_(buf);
//}

void destroyFontMap()
{
    for (auto& pair : FontMap) {
        SAFE_RELEASE(pair.second.obj);
    }
}
/*
DWORD iBmp_w = gm.gmBlackBoxX;
DWORD iBmp_h = gm.gmBlackBoxY;
DWORD x, y;
DWORD Alpha, Color;
for (y = 0; y < iBmp_h; y++) {
    for (x = 0; x < iBmp_w; x++) {
        Alpha = pMono[x + iBmp_w * y] * 255 / grad;
        if (code == 0x20 || code == 0x3000)//���p�S�p�X�y�[�X�͍����ɐ�������̂ŁAColor��0�ɂ���
            Color = 0x0;
        else
            Color = 0x00ffffff | (Alpha << 24);
        memcpy((BYTE*)pData + mapped.RowPitch * y + 4 * x, &Color, sizeof(DWORD));
    }
}
*/



