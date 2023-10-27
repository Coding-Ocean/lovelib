#include <cmath>
#include "VEC.h"
VEC::VEC() :x(0), y(0), z(0) {

}
VEC::VEC(float x, float y, float z) {
    this->x = x;    this->y = y;    this->z = z;
}
void VEC::set(float x, float y, float z) {
    this->x = x;    this->y = y;    this->z = z;
}
VEC VEC::operator-(const VEC& v) const {
    return VEC(x - v.x, y - v.y, z - v.z);
}
VEC VEC::operator+(const VEC& v) const {
    return VEC(x + v.x, y + v.y, z + v.z);
}
VEC VEC::operator*(float f) const {
    return VEC(x * f, y * f, z * f);
}
VEC VEC::operator/(float f) const {
    return VEC(x / f, y / f, z / f);
}
VEC VEC::operator-() const {
    return VEC(-x, -y, -z);
}
void VEC::operator+=(const VEC& v) {
    x += v.x;   y += v.y;   z += v.z;
}
void VEC::operator-=(const VEC& v) {
    x -= v.x;    y -= v.y;    z -= v.z;
}
void VEC::operator*=(float f) {
    x *= f;    y *= f;    z *= f;
}
void VEC::operator/=(float f) {
    x /= f;    y /= f;    z /= f;
}
float VEC::sqMag() const {
    return x * x + y * y + z * z;
}
float VEC::magSq() const {
    return x * x + y * y + z * z;
}
float VEC::mag() const {
    return sqrtf(x * x + y * y + z * z);
}
VEC VEC::setMag(float mag) {
    float len = sqrtf(x * x + y * y + z * z);
    if (len > 0) {
        mag /= len;
        x *= mag;
        y *= mag;
        z *= mag;
    }
    return VEC(x, y, z);
}
VEC VEC::limmit(float maxMag) {
    float len = sqrtf(x * x + y * y + z * z);
    if (len > maxMag) {
        maxMag /= len;
        x *= maxMag;
        y *= maxMag;
        z *= maxMag;
    }
    return VEC(x, y, z);
}
VEC VEC::normalize() {
    float len = sqrtf(x * x + y * y + z * z);
    if (len > 0) {
        x /= len;
        y /= len;
        z /= len;
    }
    return VEC(x, y, z);
}
float VEC::dot(const VEC& v) const {
    return x * v.x + y * v.y + z * v.z;
}
VEC VEC::cross(const VEC& v) const {
    return VEC(
        y * v.z - z * v.y,
        z * v.x - x * v.z,
        x * v.y - y * v.x
    );
}
float VEC::crossZ(const VEC& v) const {
    return x * v.y - y * v.x;
}
float VEC::crossY(const VEC& v) const {
    return x * v.z - z * v.x;
}

//‚RŽŸŒ³ƒxƒNƒgƒ‹‚Ìˆê”ÊŠÖ”-------------------------------------------------------
VEC operator*(float f, const VEC& v) {
    return VEC(f * v.x, f * v.y, f * v.z);
}
//”ñ”j‰ónoramalize
VEC normalize(const VEC& a) {
    float l = sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
    if (l > 0) {
        return VEC(a.x / l, a.y / l, a.z / l);
    }
    return a;
}
float length(const VEC& a)
{
  return sqrtf(a.x * a.x + a.y * a.y);
}
float dot(const VEC& a, const VEC& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
VEC cross(const VEC& a, const VEC& b) {
    return VEC(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}
float crossZ(const VEC& a, const VEC& b) {
    return a.x * b.y - a.y * b.x;
}
float crossY(const VEC& a, const VEC& b) {
    return a.x * b.z - a.z * b.x;
}

float clamp(float t, float a, float b)
{
  if (t < a)
    t = a;
  else if (t > b)
    t = b;
  return t;
}



