//
//  BoxTreeObject.cpp
//  CSE168
//
//  Created by Sebastian Dang on 4/16/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#include "BoxTreeObject.h"
#include <chrono>

BoxTreeObject::BoxTreeObject(){
    RootNode = new BoxTreeNode();
}

BoxTreeObject::~BoxTreeObject(){
    
}

void BoxTreeObject::Construct(MeshObject &mesh){
    
    //Start time.
    clock_t start = clock();
    long double start_time = start / (double) CLOCKS_PER_SEC;
    printf("Construct Start Time: %Lf\n", start_time);
    
    // ----- Construct Operation. ----- //
    
    //Get triangle properties from the mesh.
    int numTriangles = mesh.getNumTriangles();
    Triangle **triangles = mesh.getTriangles();
    
    //Construct from the triangles.
    RootNode->Construct(numTriangles, triangles);
    
    // ----- Construct Operation. ----- //
    
    //End time.
    clock_t end = clock();
    long double end_time = end / (double) CLOCKS_PER_SEC;
    printf("Construct End Time: %Lf\n", end_time);
    
    //Calculate duration.
    long double duration = end_time - start_time;
    printf("Time Elapsed: %Lf seconds.\n\n", duration);
}

bool BoxTreeObject::Intersect(const Ray &ray, Intersection &hit){
    return RootNode->Intersect(ray, hit);
}
