#pragma once
struct INDEX_BUFFER {
    struct IDirect3DIndexBuffer9* obj;
    int numIndices;

    INDEX_BUFFER(IDirect3DIndexBuffer9* obj, int numIndices)
        : obj(obj)
        , numIndices(numIndices)
    {
    }
};