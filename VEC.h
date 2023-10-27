#pragma once
struct VEC {
    float x, y, z;
    VEC();
    VEC(float x, float y, float z=0);
    void set(float x, float y, float z = 0);
    VEC operator+(const VEC& v) const;
    VEC operator-(const VEC& v) const;
    VEC operator*(float f) const;
    VEC operator/(float f) const;
    VEC operator-() const;
    void operator+=(const VEC& v);
    void operator-=(const VEC& v);
    void operator*=(float f);
    void operator/=(float f);
    float sqMag() const;
    float magSq() const;//ã@î\ÇÕsqMagÇ∆ìØÇ∂
    float mag() const;
    VEC setMag(float mag);
    VEC limmit(float maxMag);
    VEC normalize();
    float dot(const VEC& v)const;
    VEC cross(const VEC& v)const;
    float crossZ(const VEC& v)const;//äOêœZê¨ï™ÇÃÇ›åvéZ(ÇQÇcóp)
    float crossY(const VEC& v)const;//äOêœYê¨ï™ÇÃÇ›åvéZ(ÇQÇcóp)
};

VEC operator* (float f, const VEC& v);// VEC = float * VEC
VEC normalize(const VEC& a);
float length(const VEC& a);
float dot(const VEC& a, const VEC& b);
VEC cross(const VEC& a, const VEC& b);
float crossZ(const VEC& a, const VEC& b);
float crossY(const VEC& a, const VEC& b);
float clamp(float t, float a, float b);

