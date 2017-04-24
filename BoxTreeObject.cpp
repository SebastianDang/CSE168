//
//  BoxTreeObject.cpp
//  CSE168
//
//  Created by Sebastian Dang on 4/16/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#include "BoxTreeObject.h"


BoxTreeObject::BoxTreeObject(){
    RootNode = new BoxTreeNode();
}

BoxTreeObject::~BoxTreeObject(){
    
}

void BoxTreeObject::Construct(MeshObject &mesh){
    
    //Get triangle properties from the mesh.
    int numTriangles = mesh.getNumTriangles();
    Triangle **triangles = mesh.getTriangles();
    
    //Construct from the triangles.
    RootNode->Construct(numTriangles, triangles);
    
    //Debug
//    Triangle *triangle = triangles[0];
//    Vertex vtx = triangle->GetVtx(3);
//    printf("Done Constructing\n");
}

bool BoxTreeObject::Intersect(const Ray &ray, Intersection &hit){
    //Intersect starting at the root.
    return RootNode->Intersect(ray, hit);
}
