#pragma once
void createGraphic();
void destroyGraphic();
void clear(float r, float g, float b);
void present();

void wireframe();
void solid();

void fill(float r, float g, float b, float a = 1);
void setAmbient(float r, float g, float b);
void setLightDirection(float dx, float dy, float dz);
void setView(struct VEC& campos, struct VEC& lookat, struct VEC& up);

int createVertexBuffer(struct VERTEX* vertices, int numVertices);
int createIndexBuffer(unsigned short* indices, int numIndices);
int createTexture(unsigned char* pixels, int texWidth, int texHeight, const char* filename);

void rect(float px, float py, float w, float h, float rad = 0, int order = 0);
void circle(float px, float py, float diameter, int order = 0);

int createShape(int numAngles, float ratio = 1);
void shape(int vertexId, float px, float py, float w, float h, float rad = 0, int order = 0);

int loadImage(const char* filename);
void image(int idx, float px, float py, float rad=0, float scale=1, int order=0);

void model(int vertexId, int indexId, int textureId, struct MAT& world);
