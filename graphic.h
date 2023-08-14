#pragma once
void createGraphic();
void destroyGraphic();
void clear(float r, float g, float b);
void present();
void fill(float r, float g, float b, float a = 1);
void wireframe();

int createVertexBuffer(struct VERTEX* vertices, int numVertices);
int createShape(int numAngles, float ratio = 1);
void rect(float px, float py, float w, float h, float rad = 0, int order = 0);
void circle(float px, float py, float diameter, int order = 0);
void shape(int vertexId, float px, float py, float w, float h, float rad = 0, int order = 0);

int createTexture(unsigned char* pixels, int texWidth, int texHeight, const char* filename);
int loadImage(const char* filename);
void image(int idx, float px, float py, float rad=0, float scale=1, int order=0);
