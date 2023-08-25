#pragma once
int createVtxSquare(float l=0.5f);
int createIdxSquare();
int createVtxCircle(float radius = 0.5f, int numCorners = 30);
int createVtxCube(float length=0.5f);
int createIdxCube();
int createVtxSphere(float radius=0.5f, int numCorners=30 );
int createIdxSphere(int numCorners=30);
int createVtxCylinder(float radius = 0.5f, int numCorners = 30, float low = -0.5f, float high = 0.5f);
int createVtxCylinderAxisX(float radius = 0.5f, int numCorners = 30, float low = -0.5f, float high = 0.5f);
int createIdxCylinder(int numCorners = 30);

