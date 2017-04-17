////////////////////////////////////////
// Triangle.h
////////////////////////////////////////

#pragma once

#include "Vertex.h"
#include "Material.h"
#include "Ray.h"

////////////////////////////////////////////////////////////////////////////////

class Triangle {
public:
	Triangle();
	void Init(Vertex *v0,Vertex *v1,Vertex *v2,Material *m) {Vtx[0]=v0; Vtx[1]=v1; Vtx[2]=v2; Mtl=m;}

	bool Intersect(const Ray &ray, Intersection &hit) const;
    
    Vertex GetVtx(int index) {return *Vtx[index];}
    
    glm::vec3 GetCenter();

private:
	Vertex *Vtx[3];
	Material *Mtl;
};

////////////////////////////////////////////////////////////////////////////////
