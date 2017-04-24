//
//  BoxTreeNode.h
//  CSE168
//
//  Created by Sebastian Dang on 4/16/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#ifndef BoxTreeNode_h
#define BoxTreeNode_h

#include "glm/glm.hpp"
#include <stdio.h>
#include "Object.h"
#include "Triangle.h"

#define MaxTrianglesPerBox 10

class BoxTreeNode {
    
public:
    BoxTreeNode();
    ~BoxTreeNode();
    
    bool Intersect(const Ray &ray, Intersection &hit);
    
    bool IntersectTriangles(const Ray &ray, Intersection &hit);
    
    bool IntersectVolume(const Ray &ray, Intersection &hit);
    
    void Construct(int count, Triangle **tri);

private:
    
    //At each node, axis aligned box based on x,y,z of min and max.
    glm::vec3 BoxMin, BoxMax;
    //Typically represented as binary trees with zero or two children.
    BoxTreeNode *Child1, *Child2;
    //Usually 10 or less triangles at the leaf.
    Triangle *Tri[MaxTrianglesPerBox];
    //Count the number of triangles.
    int numTriangles;
};



#endif /* BoxTreeNode_h */
