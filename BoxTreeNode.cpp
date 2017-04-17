//
//  BoxTreeNode.cpp
//  CSE168
//
//  Created by Sebastian Dang on 4/17/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#include "BoxTreeNode.h"


BoxTreeNode::BoxTreeNode(){
    
}
BoxTreeNode::~BoxTreeNode(){
    
}

bool BoxTreeNode::Intersect(const Ray &ray, Intersection &hit){
    
    return false;
}

void BoxTreeNode::Construct(int count, Triangle **tri){
    
    //Initialize min, max, scale values for each coordinate.
    float min_X = INFINITY, min_Y = INFINITY, min_Z = INFINITY;//Minimum set to infinity
    float max_X = -INFINITY, max_Y = -INFINITY, max_Z = -INFINITY;//Maximum set to -infinity
    
    //Compute the BoxMin and BoxMax to fit around all the triangles.
    for (int i = 0; i < count; i++){
        Triangle *triangle = tri[i];
        for (int j = 0; j < 3; j++){
            Vertex vertex = triangle->GetVtx(j);
            //Calculate min, max for x, y, z.
            if (vertex.Position.x < min_X) { min_X = vertex.Position.x; }
            if (vertex.Position.y < min_Y) { min_Y = vertex.Position.y; }
            if (vertex.Position.z < min_Z) { min_Z = vertex.Position.z; }
            if (vertex.Position.x > max_X) { max_X = vertex.Position.x; }
            if (vertex.Position.y > max_Y) { max_Y = vertex.Position.y; }
            if (vertex.Position.z > max_Z) { max_Z = vertex.Position.z; }
        }
    }
    BoxMin = glm::vec3(min_X, min_Y, min_Z);
    BoxMax = glm::vec3(max_X, max_Y, max_Z);
    
    //Check if this is a leaf node.
    if(count <= MaxTrianglesPerBox) {
        //Copy triangles to BoxTreeNode’s Tri array
        for (int i = 0; i < count; i++){
            Tri[i] = tri[i];
        }
        return;
    }
    
    //Determine the largest box dimension: x, y, or z.
    float longest_dim = -INFINITY;
    int dim = 0;
    float dim_x = (max_X - min_X);
    float dim_y = (max_Y - min_Y);
    float dim_z = (max_Z - min_Z);
    if (dim_x > longest_dim) { longest_dim = dim_x; dim = 0; }
    if (dim_y > longest_dim) { longest_dim = dim_y; dim = 1; }
    if (dim_z > longest_dim) { longest_dim = dim_z; dim = 2; }

    //Compute splitting the plane halfway along the largest dimension.
    float split = 0;
    if (dim == 0){//X
        split = min_X + (longest_dim/2.0f);
    }
    else if (dim == 1){//Y
        split = min_Y + (longest_dim/2.0f);
    }
    else if (dim == 2){//Z
        split = min_Z + (longest_dim/2.0f);
    }
    
    //Allocate two new temporary arrays.
    Triangle **tri1 = new Triangle*[count];
    Triangle **tri2 = new Triangle*[count];
    int count1 = 0, count2 = 0;

    //Place triangles into group 1 or group 2
    for (int i = 0; i < count; i++){
        Triangle *triangle = tri[i];
        //Compute center of triangle & determine which side of splitting plane
        glm::vec3 center = triangle->GetCenter();
        //Add to appropriate group
        if (dim == 0){
            if (center.x < split){ tri1[count1++] = triangle; }
            else { tri2[count2++] = triangle; }
        }
        else if (dim == 1){
            if (center.y < split){ tri1[count1++] = triangle; }
            else { tri2[count2++] = triangle; }
        }
        else if (dim == 2){
            if (center.z < split){ tri1[count1++] = triangle; }
            else { tri2[count2++] = triangle; }
        }
    }
    
    //Check if either group is empty. If so, move (at least) 1 triangle into that group TODO:
    if(count1 == 0){
        tri1[count1++] = tri2[--count2];
    }
    if(count2 == 0){
        tri2[count2++] = tri1[--count1];
    }
    
    //Recursively build subtrees.
    Child1 = new BoxTreeNode();
    Child1->Construct(count1, tri1);
    
    Child2 = new BoxTreeNode();
    Child2->Construct(count2, tri2);
    
    //Free up arrays.
    delete []tri1;
    delete []tri2;
}

