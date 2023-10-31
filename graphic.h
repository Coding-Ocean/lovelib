#pragma once
//カラーバッファ・深度バッファ
void createGraphic();
void destroyGraphic();
void clear(float r, float g, float b);
void present();
//レンダーステート
void cullnone();
void cullcw();
void cullccw();
void normalizeNormals();
void notNormalizeNormals();
void writeDepth();
void notWriteDepth();
void specularOn();
void specularOff();
void solid();
void wireframe();
//ライト
void lightOn();
void lightOff();
void lightDirection(float dx, float dy, float dz);
void lightAmbient(float r, float g, float b);
//マテリアル
void fill(float r, float g, float b, float a = 1);
//ビュー行列
void setView(const struct VEC& campos, const struct VEC& lookat, const struct VEC& up);
//DirectXのオブジェクトバッファー
int createVertexBuffer(struct VERTEX* vertices, int numVertices);
int createIndexBuffer(unsigned short* indices, int numIndices);
int createTexture(unsigned char* pixels, int texWidth, int texHeight, const char* filename);
//２D（テクスチャ無し）
void rect(float px, float py, float w, float h, float rad = 0, int order = 0);
void circle(float px, float py, float diameter, int order = 0);
void line(float sx, float sy, float ex, float ey, float thickness = 1, int order = 0);
void arrow(float sx, float sy, float ex, float ey, float thickness = 1, float size = 20, int order = 0);;
int createShape(int numCorners, float ratio = 1);
void shape(int vertexId, float px, float py, float w, float h, float rad = 0, int order = 0);
//２D（テクスチャ有り）
int loadImage(const char* filename);
void image(int idx, float px, float py, float rad=0, float scale=1, int order=0);
//３Dモデル
extern int TexNone;
void model(int vertexId, int indexId, int textureId, struct MAT& world);
void model(struct VERTEX* vtx, unsigned* idx, int numTriangles);
void createLine3D(float radius=0.01f, int numCorners=4);
void line3D(const VEC& p1, const VEC& p2);
//フォント
void fontFace(const char* fontname, unsigned charset = 128);//128=>SHIFTJIS_CHARSET
void fontSize(int size);
float text(const char* str, float x, float y);
void printInit();
void print(const char* format, ...);
void setPrintInitX(float initX);
void setPrintInitY(float initY);
void destroyFontTextures();


