#include<cmath>
#include<vector>
#include"VERTEX.h"
#include"graphic.h"

int createVtxCube(float l)
{
	VERTEX vertices[] = {
		//正面
		-l, l,-l, 0,0,-1, 0,0,
		-l,-l,-l, 0,0,-1, 0,1,
		 l,-l,-l, 0,0,-1, 1,1,
		 l, l,-l, 0,0,-1, 1,0,
		 //裏面
		  l, l, l, 0,0,1, 0,0,
		  l,-l, l, 0,0,1, 0,1,
		 -l,-l, l, 0,0,1, 1,1,
		 -l, l, l, 0,0,1, 1,0,
		 //右面
		  l, l,-l, 1,0,0, 0,0,
		  l,-l,-l, 1,0,0, 0,1,
		  l,-l, l, 1,0,0, 1,1,
		  l, l, l, 1,0,0, 1,0,
		  //左面
		  -l, l, l, -1,0,0, 0,0,
		  -l,-l, l, -1,0,0, 0,1,
		  -l,-l,-l, -1,0,0, 1,1,
		  -l, l,-l, -1,0,0, 1,0,
		  //上面
		  -l, l, l, 0,1,0, 0,0,
		  -l, l,-l, 0,1,0, 0,1,
		   l, l,-l, 0,1,0, 1,1,
		   l, l, l, 0,1,0, 1,0,
		   //下面
		   -l,-l,-l, 0,-1,0, 0,0,
		   -l,-l, l, 0,-1,0, 0,1,
			l,-l, l, 0,-1,0, 1,1,
			l,-l,-l, 0,-1,0, 1,0,
	};
	return createVertexBuffer(vertices, _countof(vertices));
}

int createIdxCube()
{
	unsigned short indices[] = {
		0,1,2,
		0,2,3,
		4,5,6,
		4,6,7,
		8,9,10,
		8,10,11,
		12,13,14,
		12,14,15,
		16,17,18,
		16,18,19,
		20,21,22,
		20,22,23,
	};
	return createIndexBuffer(indices, _countof(indices));
}

int createVtxSphere(float radius, int numAngles)
{
	std::vector<VERTEX> vertices;
	//北極南極点以外の頂点
	float divAngle = 3.141592f * 2 / numAngles;
	VERTEX v;
	float r;
	for (int j = 1; j < numAngles / 2; j++) {
		v.ny = cos(divAngle * j);
		v.y = v.ny * radius;
		r = sin(divAngle * j);
		v.v = (1.0f - asin(v.ny) / (3.141592f / 2.0f)) / 2.0f;
		for (int i = 0; i <= numAngles; i++) {
			v.nx = cos(divAngle * i) * r;
			v.x = v.nx * radius;
			v.nz = sin(divAngle * i) * r;
			v.z = v.nz * radius;
			v.u = atan2(v.z, v.x) / (3.141592f * 2);
			if (i > numAngles / 2)	v.u += 1.0f;
			vertices.push_back(v);
		}
	}
	//北極点
	v.x = 0; v.y = radius; v.z = 0; v.nx = 0, v.ny = 1, v.nz = 0;
	v.u = 0.5f; v.v = 0;
	vertices.push_back(v);
	size_t n = vertices.size();
	//南極点
	v.y = -radius; v.ny = -1;
	v.u = 0.5f; v.v = 1;
	vertices.push_back(v);

	return createVertexBuffer(vertices.data(), (int)vertices.size());
}
int createIdxSphere(int numAngles)
{
	std::vector<unsigned short> indices;
	int stride = numAngles + 1;
	for (int j = 0; j < numAngles / 2 - 2; j++) {
		for (int i = 0; i < numAngles; i++) {
			int k = i + stride * j;
			indices.push_back(k);
			indices.push_back(k + stride);
			indices.push_back(k + stride + 1);
			indices.push_back(k);
			indices.push_back(k + stride + 1);
			indices.push_back(k + 1);
		}
	}
	{
		//北極点のインデックスn
		int n = (numAngles + 1) * (numAngles / 2 - 1);
		for (int i = 0; i < numAngles; i++) {
			indices.push_back(n);
			indices.push_back(i);
			indices.push_back(i + 1);
		}
	}
	{
		//南極点のインデックスs
		int s = (numAngles + 1) * (numAngles / 2 - 1) + 1;
		int j = numAngles / 2 - 2;
		for (int i = 0; i < numAngles; i++) {
			int k = i + (numAngles + 1) * j;
			indices.push_back(k);
			indices.push_back(s);
			indices.push_back(k + 1);
		}
	}
	return createIndexBuffer(indices.data(), (int)indices.size());
}
