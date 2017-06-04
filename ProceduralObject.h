//
//  ProceduralObject.hpp
//  CSE168
//
//  Created by Sebastian Dang on 6/3/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#ifndef ProceduralObject_h
#define ProceduralObject_h

#include <stdio.h>
#include <vector>
#include "Object.h"
#include "Triangle.h"

class ProceduralObject:public Object {
public:
    ProceduralObject();
    ~ProceduralObject();
    
    bool Intersect(const Ray &ray,Intersection &hit);
    
    void Generate(float x,float y,float z,Material *mtl=0);

    

private:
    int NumVertexes,NumTriangles,NumMaterials;
    Vertex *Vertexes;
    Triangle *Triangles;
    Material *Materials;
    
};


#endif /* ProceduralObject_h */
