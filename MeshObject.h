////////////////////////////////////////
// MeshObject.h
////////////////////////////////////////

#pragma once

#include "Object.h"
#include "Triangle.h"

////////////////////////////////////////////////////////////////////////////////

class MeshObject: public Object {

public:
	
    MeshObject();
	~MeshObject();

	bool Intersect(const Ray &ray,Intersection &hit);

	void MakeBox(float x,float y,float z,Material *mtl=0);
    
    bool Load(const char *filename);

private:
	int NumVertexes, NumTriangles, NumMaterials;
	Vertex *Vertexes;
	Triangle *Triangles;
    Material *Materials;
};

////////////////////////////////////////////////////////////////////////////////
