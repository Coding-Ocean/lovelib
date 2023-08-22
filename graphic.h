#pragma once
//カラーバッファ・深度バッファ
void createGraphic();
void destroyGraphic();
void clear(float r, float g, float b);
void present();
//レンダーステート
void wireframe();
void solid();
void cullnone();
void cullcw();
void ambient(float r, float g, float b);
//マテリアル
void fill(float r, float g, float b, float a = 1);
//ライト方向
void setLightDirection(float dx, float dy, float dz);
//ビュー行列
void setView(struct VEC& campos, struct VEC& lookat, struct VEC& up);
//DirectXのオブジェクトバッファー
int createVertexBuffer(struct VERTEX* vertices, int numVertices);
int createIndexBuffer(unsigned short* indices, int numIndices);
int createTexture(unsigned char* pixels, int texWidth, int texHeight, const char* filename);
//２D（テクスチャ無し）
void rect(float px, float py, float w, float h, float rad = 0, int order = 0);
void circle(float px, float py, float diameter, int order = 0);
int createShape(int numCorners, float ratio = 1);
void shape(int vertexId, float px, float py, float w, float h, float rad = 0, int order = 0);
//２D（テクスチャ有り）
int loadImage(const char* filename);
void image(int idx, float px, float py, float rad=0, float scale=1, int order=0);
//３Dモデル
void model(int vertexId, int indexId, int textureId, struct MAT& world);
