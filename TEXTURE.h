#pragma once
#include <string>
struct TEXTURE {
    struct IDirect3DTexture9* obj;
    int width;
    int height;
    std::string filename;
    TEXTURE()
        :obj(nullptr)
        , width(0)
        , height(0)
    {
    }
    TEXTURE(struct IDirect3DTexture9* obj, int width, int height, const char* filename)
        : obj(obj)
        , width(width)
        , height(height)
        , filename(filename)
    {
    }
};
