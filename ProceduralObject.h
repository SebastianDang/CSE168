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

class ProceduralObject : public Object {
public:
    ProceduralObject();
    ~ProceduralObject();
    
    bool Intersect(const Ray &ray,Intersection &hit);
    
    void Generate(float size, Material *mtl=0);

    Triangle **getTriangles();
    
    int getNumTriangles(){ return NumTriangles; }

private:
    int NumVertexes,NumTriangles,NumMaterials;
    Vertex *Vertexes;
    Triangle *Triangles;
    Material *Materials;
    
    void diamond_square(int x1, int x2, int y1, int y2, int level, float range);
    
    void updateNormals();
    
    float getHeightFromVertex(int x, int y);
    
};


#endif /* ProceduralObject_h */
