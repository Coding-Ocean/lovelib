#pragma once
struct VERTEX_BUFFER {
    struct IDirect3DVertexBuffer9* obj;
    unsigned int numVertices;

    VERTEX_BUFFER(IDirect3DVertexBuffer9* obj, unsigned int numVertices)
        : obj(obj)
        , numVertices(numVertices)
    {
    }
};