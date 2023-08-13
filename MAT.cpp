#include <cmath>
#include "VEC.h"
#include "MAT.h"
MAT::MAT() {
    identity();
}
void MAT::identity() {
    _11 = 1; _12 = 0; _13 = 0; _14 = 0;
    _21 = 0; _22 = 1; _23 = 0; _24 = 0;
    _31 = 0; _32 = 0; _33 = 1; _34 = 0;
    _41 = 0; _42 = 0; _43 = 0; _44 = 1;
}
//行列効果は掛けた順に表れるバージョン
void MAT::mulTranslate(float x, float y, float z) {
    _41 += x;    _42 += y;    _43 += z;
}
void MAT::mulScaling(float x, float y, float z) {
    _11 *= x;    _12 *= y;    _13 *= z;
    _21 *= x;    _22 *= y;    _23 *= z;
    _31 *= x;    _32 *= y;    _33 *= z;
    _41 *= x;    _42 *= y;    _43 *= z;
}
void MAT::mulRotateZ(float r) {
    float c = cos(r);
    float s = sin(r);
    float tmp;
    //1行目
    tmp = _11 * c + _12 * -s;
    _12 = _11 * s + _12 * c;
    _11 = tmp;
    //2行目
    tmp = _21 * c + _22 * -s;
    _22 = _21 * s + _22 * c;
    _21 = tmp;
    //3行目
    tmp = _31 * c + _32 * -s;
    _32 = _31 * s + _32 * c;
    _31 = tmp;
    //4行目
    tmp = _41 * c + _42 * -s;
    _42 = _41 * s + _42 * c;
    _41 = tmp;
}
void MAT::mulRotateX(float r)
{
    float c = cos(r);
    float s = sin(r);
    float tmp;
    //1行目
    tmp = _12 * c + _13 * -s;
    _13 = _12 * s + _13 * c;
    _12 = tmp;
    //2行目
    tmp = _22 * c + _23 * -s;
    _23 = _22 * s + _23 * c;
    _22 = tmp;
    //3行目
    tmp = _32 * c + _33 * -s;
    _33 = _32 * s + _33 * c;
    _32 = tmp;
    //4行目
    tmp = _42 * c + _43 * -s;
    _43 = _42 * s + _43 * c;
    _42 = tmp;
}
void MAT::mulRotateY(float r) {
    float c = cosf(r);
    float s = sinf(r);
    float tmp;
    //1行目
    tmp = _11 * c + _13 * s;
    _13 = _11 * -s + _13 * c;
    _11 = tmp;
    //2行目
    tmp = _21 * c + _23 * s;
    _23 = _21 * -s + _23 * c;
    _21 = tmp;
    //3行目
    tmp = _31 * c + _33 * s;
    _33 = _31 * -s + _33 * c;
    _31 = tmp;
    //4行目
    tmp = _41 * c + _43 * s;
    _43 = _41 * -s + _43 * c;
    _41 = tmp;
}

void MAT::camera(VEC& camPos, VEC& lookat, VEC& up) {
    //カメラのローカル軸座標を求める(正規直交ベクトル）
    VEC z = normalize(lookat-camPos);
    VEC x = normalize(cross(up, z));
    VEC y = cross(z, x);

    _11 = x.x;    _12 = y.x;    _13 = z.x;  _14 = 0;
    _21 = x.y;    _22 = y.y;    _23 = z.y;  _24 = 0;
    _31 = x.z;    _32 = y.z;    _33 = z.z;  _34 = 0;

    _41 = x.x * -camPos.x + x.y * -camPos.y + x.z * -camPos.z;
    _42 = y.x * -camPos.x + y.y * -camPos.y + y.z * -camPos.z;
    _43 = z.x * -camPos.x + z.y * -camPos.y + z.z * -camPos.z;
    _44 = 1;
}

void MAT::setOrtho(float l, float r, float b, float t, float n, float f) {
    _11 = 2.0f / (r - l);     _12 = 0.0f;               _13 = 0.0f;      _14 = 0;
    _21 = 0.0f;               _22 = 2.0f / (t - b);     _23 = 0.0f;      _24 = 0;
    _31 = 0.0f;               _32 = 0.0f;               _33 = 1.0f / f;  _34 = 0;
    _41 = -(r + l) / (r - l); _42 = -(t + b) / (t - b); _43 = 0.0f;      _44 = 1.0f;
}
void MAT::setOrtho(float w, float h) {
    _11 = 2.0f / w; _12 = 0.0f;     _13 = 0.0f; _14 = 0;
    _21 = 0.0f;     _22 = 2.0f / h; _23 = 0.0f; _24 = 0;
    _31 = 0.0f;     _32 = 0.0f;     _33 = 1.0f; _34 = 0;
    _41 = -1;       _42 = 1;        _43 = 0.0f; _44 = 1.0f;
}
void MAT::setPers(float angle, float aspect, float n, float f) {
    float s = 1.0f / tanf(angle * 0.5f);
    float a = f / (f - n);
    _11 = s / aspect;   _12 = 0;        _13 = 0;            _14 = 0;
    _21 = 0;            _22 = s;        _23 = 0;            _24 = 0;
    _31 = 0;            _32 = 0;        _33 = a;            _34 = 1;
    _41 = 0;            _42 = 0;        _43 = a * -n;       _44 = 0;
}

void MAT::transpose()
{
    float w;
    w = _12; _12 = _21; _21 = w;
    w = _13; _13 = _31; _31 = w;
    w = _14; _14 = _41; _41 = w;
    w = _23; _23 = _32; _32 = w;
    w = _24; _24 = _42; _42 = w;
    w = _34; _34 = _43; _43 = w;
}


/*
//行列計算効果が、掛けた順と逆になるバージョン
void MAT::mulTranslate(float x, float y, float z) {
    _41 += _11 * x + _21 * y + _31 * z;
    _42 += _12 * x + _22 * y + _32 * z;
    _43 += _13 * x + _23 * y + _33 * z;
}
void MAT::mulScaling(float x, float y, float z) {
    _11 *= x;    _12 *= x;    _13 *= x;
    _21 *= y;    _22 *= y;    _23 *= y;
    _31 *= z;    _32 *= z;    _33 *= z;
}
void MAT::mulRotateZ(float r) {
    float c = cos(r);
    float s = sin(r);
    float tmp;
    //1行目
    tmp = _11 * c + _12 * -s;
    _12 = _11 * s + _12 * c;
    _11 = tmp;
    //2行目
    tmp = _21 * c + _22 * -s;
    _22 = _21 * s + _22 * c;
    _21 = tmp;
    //3行目
    tmp = _31 * c + _32 * -s;
    _32 = _31 * s + _32 * c;
    _31 = tmp;
}
void MAT::mulRotateX(float r)
{
    float c = cos(r);
    float s = sin(r);
    float tmp;
    //1行目
    tmp = _12 * c + _13 * -s;
    _13 = _12 * s + _13 * c;
    _12 = tmp;
    //2行目
    tmp = _22 * c + _23 * -s;
    _23 = _22 * s + _23 * c;
    _22 = tmp;
    //3行目
    tmp = _32 * c + _33 * -s;
    _33 = _32 * s + _33 * c;
    _32 = tmp;
}
void MAT::mulRotateY(float r) {
    float c = cosf(r);
    float s = sinf(r);
    float tmp;
    //1行目
    tmp = _11 * c + _13 * s;
    _13 = _11 * -s + _13 * c;
    _11 = tmp;
    //2行目
    tmp = _21 * c + _23 * s;
    _23 = _21 * -s + _23 * c;
    _21 = tmp;
    //3行目
    tmp = _31 * c + _33 * s;
    _33 = _31 * -s + _33 * c;
    _31 = tmp;
}
*/
