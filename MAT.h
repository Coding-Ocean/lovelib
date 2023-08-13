#pragma once
#include <d3d9.h>

struct MAT : D3DMATRIX {
    MAT();
    void identity();
    void mulTranslate(float x, float y, float z);
    void mulScaling(float x, float y, float z);
    void mulRotateZ(float r);
    void mulRotateX(float r);
    void mulRotateY(float r);
    void camera(struct VEC&, struct VEC&, struct VEC&);
    void setOrtho(float l, float r, float b, float t, float n, float f);
    void setOrtho(float w, float h);
    void setPers(float angle, float aspect, float n, float f);
    void transpose();
};

/*éQçl
typedef struct _D3DMATRIX {
    union {
        struct {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float m[4][4];
    };
} D3DMATRIX;
*/

