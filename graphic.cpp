#pragma comment(lib, "d3d9.lib")

#include <unordered_map>
#include <vector>
#include <cmath>
#include <d3d9.h>
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
int VtxSquareCenter{};
int VtxCircle{};
int VtxSquareLeftTop{};
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

        Dev->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        Dev->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
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
        VtxSquareCenter = createVtxSquare();
    }

    //���_�o�b�t�@�P�F�~
    {
        VtxCircle = createVtxCircle(0.5f, 60);
    }

    //���_�o�b�t�@�Q�F�t�H���g�p�����`
    {
        VtxSquareLeftTop = createVtxSquare(0, 0, 1, -1);
    }
    //�e�N�X�`���O�F�e�N�X�`������
    {
        Textures.emplace_back(nullptr, 0, 0, "TexNone");
        TexNone = 0;
    }
}

void destroyGraphic()
{
    destroyFontTextures();

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
    printInit();
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
    D3DLOCKED_RECT lockedRect;
    hr = obj->LockRect(0, &lockedRect, NULL, D3DLOCK_DISCARD);
    WARNING(FAILED(hr), "Texture", "Lock error");
    unsigned char r, g, b, a;
    unsigned int argb;
    //�R�s�[��|�C���^
    unsigned int* texBuf = (unsigned int*)lockedRect.pBits;
    for (int y = 0; y < texHeight; y++) {
        for (int x = 0; x < texWidth; x++) {
            //�R�s�[��pixel�f�[�^
            int i = x + y * texWidth;
            r = pixels[i*4 + 0];
            g = pixels[i*4 + 1];
            b = pixels[i*4 + 2];
            a = pixels[i*4 + 3];
            argb = a << 24 | r << 16 | g << 8 | b;
            //�e�N�X�`���ɃR�s�[
            texBuf[i] = argb;
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
    Dev->SetStreamSource(0, VertexBuffers[VtxSquareCenter].obj, 0, sizeof(VERTEX));
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
    Dev->SetStreamSource(0, VertexBuffers[VtxSquareCenter].obj, 0, sizeof(VERTEX));
    //�`��i�Ō�̃p�����^�͕`�悷��O�p�`���j
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
    circle(sx, sy, thickness, order);
    circle(ex, ey, thickness, order);
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
    circle(sx, sy, thickness, order);
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
    World2D.mulTranslate(px+0.5f, -py-0.5f, order / 1000.0f);
    Dev->SetTransform(D3DTS_WORLD, &World2D);
    Dev->SetTransform(D3DTS_VIEW, &View2D);
    Dev->SetTransform(D3DTS_PROJECTION, &Proj2D);
    //�F
    Dev->SetLight(0, &Light2D);
    Dev->SetMaterial(&Material);
    Dev->SetTexture(0, Textures[idx].obj);
    //���_
    Dev->SetFVF(VERTEX_FORMAT);
    Dev->SetStreamSource(0, VertexBuffers[VtxSquareCenter].obj, 0, sizeof(VERTEX));
    //�`��
    Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
}

//3D-------------------------------------------------------------------

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

void model(VERTEX* vtx, unsigned* idx, int numTriangles, int texIdx)
{
  //�s��
  World.identity();
  Dev->SetTransform(D3DTS_WORLD, &World);
  Dev->SetTransform(D3DTS_VIEW, &View);
  Dev->SetTransform(D3DTS_PROJECTION, &Proj);
  //�F
  Dev->SetLight(0, &Light);
  Dev->SetMaterial(&Material);
  Dev->SetTexture(0, Textures[texIdx].obj);
  //���_
  Dev->SetFVF(VERTEX_FORMAT);
  //�`��
  Dev->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,0,0,
    numTriangles,idx,D3DFMT_INDEX32,vtx,sizeof(VERTEX)
  );
}


//FONT--------------------------------------------------------------------

//���ݕ`�撆�̃t�H���g�t�F�C�X�\����
struct CURRENT_FONT_FACE {
    std::string name;
    unsigned long charset;
    int id;
    int size;
};
static CURRENT_FONT_FACE CurFontFace{ "�l�r �S�V�b�N",SHIFTJIS_CHARSET,0,50 };

//FontFace�����Ƃɂ�����t���ĊǗ�����}�b�v
static std::unordered_map<std::string, int> FontIdMap{ {CurFontFace.name, 0} };
static int FontIdCnt = 0;

//�`�悷��t�H���g��ݒ肷��
void fontFace(const char* fontname, unsigned charset)
{
    //�t�H���g����charset
    CurFontFace.name = fontname;
    CurFontFace.charset = charset;

    //CurFontFace.id���Z�b�g����
    auto itr = FontIdMap.find(fontname);
    if (itr == FontIdMap.end()) {
        FontIdCnt++;
        WARNING(FontIdCnt >= 32, "FontFace", "����ȏ�ǉ��ł��܂���");
        FontIdMap[fontname] = FontIdCnt;
        CurFontFace.id = FontIdCnt;
    }
    else {
        CurFontFace.id = itr->second;
    }
}

//�t�H���g�T�C�Y��ݒ肷��
void fontSize(int size)
{
    WARNING(size > 2048, "FontSize", "2048���傫���T�C�Y�͎w��ł��܂���");
    CurFontFace.size = size;
}

//FONT_TEXTURE�\����(���̃}�b�v�ɕۑ����Ă����t�H���g�̕`��ɕK�v�ȃf�[�^�B)
struct FONT_TEXTURE {
    IDirect3DTexture9* obj;
    float texWidth,texHeight;//�e�N�X�`���̕��A����
    float width,height;//�`�敝�A����
    float ofstX, ofstY;//�`�悷��Ƃ��ɂ��炷�l
    //�f�t�H���g�R���X�g���N�^
    FONT_TEXTURE()
        : obj(nullptr)
        , texWidth(0), texHeight(0)
        , width(0), height(0)
        , ofstX(0), ofstY(0)
    {
    }
    //�R���X�g���N�^�B�`��p��int�^��float�^�ɕϊ����ĕێ�����
    FONT_TEXTURE(
        IDirect3DTexture9* obj, 
        int texWidth, int texHeight,
        int width, int height,
        int ofstX, int ofstY)
        : obj(obj)
        , width((float)width), height((float)height)
        , texWidth((float)texWidth), texHeight((float)texHeight)
        , ofstX((float)ofstX), ofstY((float)ofstY)
    {
    }
};

//�t�H���g�e�N�X�`���f�[�^���Ǘ�����}�b�v
static std::unordered_map<DWORD, FONT_TEXTURE> FontTextureMap;

//�P�������̃t�H���g�e�N�X�`���������ď�̃}�b�v�ɒǉ�����
FONT_TEXTURE* createFontTexture(DWORD key)
{
    //�t�H���g�i�T�C�Y��t�H���g�̎�ށj�����߂�I
    HFONT hFont = CreateFont(
        CurFontFace.size, 0, 0, 0, 0, 0, 0, 0, 
        CurFontFace.charset,
        OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS, 
        PROOF_QUALITY, FIXED_PITCH | FF_MODERN,
        CurFontFace.name.c_str()
    );
    WARNING(!hFont, "Font", "Create error");

    //�f�o�C�X�R���e�L�X�g�擾
    HDC hdc = GetDC(NULL);
    //�f�o�C�X�R���e�L�X�g�Ƀt�H���g��ݒ�
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    //�t�H���g�̊e�퐡�@�ƃr�b�g�}�b�v���擾
    TEXTMETRIC tm;
    GetTextMetrics(hdc, &tm);
    GLYPHMETRICS gm;
    CONST MAT2 mat = {{0,1},{0,0},{0,0},{0,1}};
    UINT code = key & 0xffff;//key���當���R�[�h�����o��
    DWORD bmpSize = GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &gm, 0, NULL, &mat);
    BYTE* bmpBuf = new BYTE[bmpSize];
    GetGlyphOutline(hdc, code, GGO_GRAY4_BITMAP, &gm, bmpSize, bmpBuf, &mat);
    //���l�̊K�� (GGO_GRAY4_BITMAP��17�K���BbmpBuf[i]�͂O�`�P�U�̒l�ƂȂ�)
    DWORD tone = 16;//�ő�l

    //�f�o�C�X�R���e�L�X�g�ƃt�H���g�n���h���̊J��
    DeleteObject(hFont);
    SelectObject(hdc, oldFont);
    ReleaseDC(NULL, hdc);

    //��̃e�N�X�`���I�u�W�F�N�g������
    HRESULT hr;
    IDirect3DTexture9* obj = 0;
    DWORD texWidth = (gm.gmBlackBoxX + 3) & 0xfffc;//4�̔{���ɂ���
    DWORD texHeight = gm.gmBlackBoxY;
    hr = Dev->CreateTexture(texWidth, texHeight, 1,
        0/*USAGE*/, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED,
        &obj, NULL
    );
    WARNING(FAILED(hr), "FontTexture", "Create error");

    //�e�N�X�`���Ƀt�H���g�r�b�g�}�b�v���R�s�[
    D3DLOCKED_RECT lockedRect;
    obj->LockRect(0, &lockedRect, NULL, 0);
    WARNING(FAILED(hr), "FontTexture", "Lock error");
    DWORD* texBuf = (DWORD*)lockedRect.pBits;// �e�N�X�`���������ւ̃|�C���^
    DWORD x, y, i, alpha;
    for (y = 0; y < texHeight; y++) {
        for (x = 0; x < texWidth; x++) {
            i = y * texWidth + x;
            if (i < bmpSize) {
                alpha = bmpBuf[i] * 255 / tone;//0�`16��0�`255�ɕϊ�
                texBuf[i] = (alpha << 24) | 0x00ffffff;
            }
        }
    }
    obj->UnlockRect(0);
    
    //�R�s�[�����̂ł����r�b�g�}�b�v�͗v��Ȃ�
    delete[] bmpBuf;

    //FONT_TEXTURE(�`��ɕK�v�ȃf�[�^�B)���}�b�v�ɓo�^
    FontTextureMap[key] = FONT_TEXTURE(
        obj, //�e�N�X�`���I�u�W�F�N�g
        texWidth, texHeight,//�e�N�X�`���̕��ƍ���
        gm.gmCellIncX, tm.tmHeight,//�`�悷�镝�ƍ���
        gm.gmptGlyphOrigin.x, tm.tmAscent - gm.gmptGlyphOrigin.y//�`�悷�鎞�ɂ��炷�l
    );

    return &FontTextureMap[key];
}

//�w�肵����������w�肵���X�N���[�����W�ŕ`�悷��
float text(const char* str, float x, float y)
{
    int len = (int)strlen(str);

    //���[�v���Ȃ���P�������`�悵�Ă���
    for (int i = 0; i < len; i++) {

        //�����R�[�h�̌���(�}���`�o�C�g�R�[�h��������Ȃ��O��)
        WORD code;
        if (IsDBCSLeadByte(str[i])) {
            //2�o�C�g�����̃R�[�h��[�擱�R�[�h] + [�����R�[�h]�ł�
            code = (BYTE)str[i] << 8 | (BYTE)str[i + 1];
            i++;
        }
        else {
            //1�o�C�g�����̃R�[�h
            code = str[i];
        }

        //�}�b�v�����pkey(�t�H���gID�{�t�H���g�T�C�Y�{�����R�[�h)������
        DWORD key = CurFontFace.id << 27 | CurFontFace.size << 16 | code;

        //key�Ń}�b�v���Ƀe�N�X�`�������邩�T��
        FONT_TEXTURE* tex = 0;
        auto itr = FontTextureMap.find(key);
        if (itr == FontTextureMap.end()) {
            //�Ȃ������̂Ńt�H���g�̃e�N�X�`�������̏�ł����ăA�h���X�����炤
            tex = createFontTexture(key);
        }
        else {
            //�������̂ŃA�h���X���擾����
            tex = &itr->second;
        }

        //�s��
        World2D.identity();
        World2D.mulScaling(tex->texWidth, tex->texHeight, 1.0f);
        World2D.mulTranslate(tex->ofstX, -tex->ofstY, 0);
        World2D.mulTranslate(x - 0.5f, -(y - 0.5f), 0);
        //       ��DirectX�̎d�l��0.5���炳�Ȃ��ƃe�N�X�`���������
        Dev->SetTransform(D3DTS_WORLD, &World2D);
        Dev->SetTransform(D3DTS_VIEW, &View2D);
        Dev->SetTransform(D3DTS_PROJECTION, &Proj2D);
        //�F
        Dev->SetLight(0, &Light2D);
        Dev->SetMaterial(&Material);
        Dev->SetTexture(0, tex->obj);
        //���_
        Dev->SetFVF(VERTEX_FORMAT);
        Dev->SetStreamSource(0, VertexBuffers[VtxSquareLeftTop].obj, 0, sizeof(VERTEX));
        //Dev->SetStreamSource(0, VertexBuffers[VtxSquareCenter].obj, 0, sizeof(VERTEX));
        //�`��
        Dev->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
        
        //���̕����̕`��ʒu�������߂Ă���
        x += tex->width;
    }
    //���ɑ����ĕʂ̕������\�����邽�߂̍��W��Ԃ�
    return x;
}

static float PrintInitX = 10;
static float PrintInitY = 10;
static float PrintY = PrintInitY;
void printInit()
{
    PrintY = PrintInitY;
}
void print(const char* format, ...)
{
    char str[256];
    va_list args;
    va_start(args, format);
    vsprintf_s(str, format, args);
    va_end(args);

    float printX = PrintInitX;
    text(str, printX, PrintY);
    PrintY += CurFontFace.size;
}
void setPrintInitX(float initX)
{
    PrintInitX = initX;
}
void setPrintInitY(float initY)
{
    PrintInitY = initY;
}

void destroyFontTextures()
{
    for (auto& pair : FontTextureMap) {
        SAFE_RELEASE(pair.second.obj);
    }
}



