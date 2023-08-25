#pragma once
//�J���[�o�b�t�@�E�[�x�o�b�t�@
void createGraphic();
void destroyGraphic();
void clear(float r, float g, float b);
void present();
//�����_�[�X�e�[�g
void ambient(float r, float g, float b);
void normalizeNormals();
void notNormalizeNormals();
void cullnone();
void cullcw();
void cullccw();
void wireframe();
void solid();
//�}�e���A��
void fill(float r, float g, float b, float a = 1);
//���C�g
void setLightDirection(float dx, float dy, float dz);
void lightOn();
void lightOff();
//�r���[�s��
void setView(const struct VEC& campos, const struct VEC& lookat, const struct VEC& up);
//DirectX�̃I�u�W�F�N�g�o�b�t�@�[
int createVertexBuffer(struct VERTEX* vertices, int numVertices);
int createIndexBuffer(unsigned short* indices, int numIndices);
int createTexture(unsigned char* pixels, int texWidth, int texHeight, const char* filename);
//�QD�i�e�N�X�`�������j
void rect(float px, float py, float w, float h, float rad = 0, int order = 0);
void circle(float px, float py, float diameter, int order = 0);
void line(float sx, float sy, float ex, float ey, float thickness = 1, int order = 0);
int createShape(int numCorners, float ratio = 1);
void shape(int vertexId, float px, float py, float w, float h, float rad = 0, int order = 0);
//�QD�i�e�N�X�`���L��j
int loadImage(const char* filename);
void image(int idx, float px, float py, float rad=0, float scale=1, int order=0);
//�RD���f��
extern int TexNone;
void model(int vertexId, int indexId, int textureId, struct MAT& world);
void createLine3D(float radius=0.01f, int numCorners=4);
void line3D(const VEC& p1, const VEC& p2);
