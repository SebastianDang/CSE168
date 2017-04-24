//
//  BoxTreeNode.cpp
//  CSE168
//
//  Created by Sebastian Dang on 4/17/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#include "BoxTreeNode.h"


BoxTreeNode::BoxTreeNode(){
    numTriangles = 0;
    leaf = false;
}

BoxTreeNode::~BoxTreeNode(){
    
}

/* Intersect this node if it is the leaf, and continue through it's children. */
bool BoxTreeNode::Intersect(const Ray &ray, Intersection &hit){
    
    //If this is a leaf node, test against all triangles.
    if (leaf){
        return IntersectTriangles(ray, hit);
    }
    
    //Test both children. Gets an initial t value.
    float t1, t2;
    bool c1Hit = false, c2Hit = false;
    
    c1Hit = Child1->IntersectVolume(ray, t1);
    c2Hit = Child2->IntersectVolume(ray, t2);
    
    //Optimization. If child(ren) isn't hit, end early.
    if (!c1Hit && !c2Hit) {
        return false;
    }
    else if (c1Hit && !c2Hit) {
        return Child1->Intersect(ray, hit);
    }
    else if (!c1Hit && c2Hit) {
        return Child2->Intersect(ray, hit);
    }
    
    //Full Recursive Test on children, sorted in order of distance.
    bool success = false;
    if (t1 < t2){//t1 is closer.
        if (t1 < hit.HitDistance){
            if (Child1->Intersect(ray, hit)) success = true;
        }
    
        if (t2 < hit.HitDistance){
            if (Child2->Intersect(ray, hit)) success = true;
        }
    }
    
    else{//t2 is closer.
        if (t2 < hit.HitDistance){
            if (Child2->Intersect(ray, hit)) success = true;
        }
        
        if (t1 < hit.HitDistance){
            if (Child1->Intersect(ray, hit)) success = true;
        }
    }
    
    return success;
}

/* Loops through each triangle to determine if an intersection is found. */
bool BoxTreeNode::IntersectTriangles(const Ray &ray, Intersection &hit){
    
    //Test for the closest hit
    bool triangleHit = false;
    
    //Test against each triangle.
    for (int i = 0; i < numTriangles; i++){
        if (Tri[i]->Intersect(ray, hit)) {//Set the closest triangle in hit.
            triangleHit = true;
        }
    }

    //Return if a triangle is hit.
    return triangleHit;
}

/* Ray AABB Test. Intersect Volume. */
bool BoxTreeNode::IntersectVolume(const Ray &ray, float &t){
    
    //Min and Max corners of the box.
    glm::vec3 a = BoxMin;
    glm::vec3 b = BoxMax;
    
    //Ray origin and direction.
    glm::vec3 p = ray.Origin;
    glm::vec3 d = ray.Direction;
    
    //Calculate t values.
    glm::vec3 t1, t2;
    
    t1.x = (a.x-p.x) / d.x;
    t2.x = (b.x-p.x) / d.x;
    
    t1.y = (a.y-p.y) / d.y;
    t2.y = (b.y-p.y) / d.y;
    
    t1.z = (a.z-p.z) / d.z;
    t2.z = (b.z-p.z) / d.z;
    
    //Calculate min and max.
    float min_x = fmin(t1.x, t2.x);
    float min_y = fmin(t1.y, t2.y);
    float min_z = fmin(t1.z, t2.z);
    
    float max_x = fmax(t1.x, t2.x);
    float max_y = fmax(t1.y, t2.y);
    float max_z = fmax(t1.z, t2.z);
    
    //Calculate t_min and t_max.
    float t_min = -INFINITY;
    float t_max = INFINITY;
    
    //Get the maximum of the minimum values.
    if (min_x > t_min) { t_min = min_x; }
    if (min_y > t_min) { t_min = min_y; }
    if (min_z > t_min) { t_min = min_z; }
    
    //Get the minimum of the maximum values.
    if (max_x < t_max) { t_max = max_x; }
    if (max_y < t_max) { t_max = max_y; }
    if (max_z < t_max) { t_max = max_z; }
    
    //If t_min < t_max, the ray intersects at t=t_min or t=t_max.
    if (t_min <= t_max){
        //If t_min is >= 0, set t = t_min.
        if (t_min >= 0){
            t = t_min;
        }
        //Else t_max.
        else {
            t = t_max;
        }
        return true;
    }
    
    //If t_max < 0, miss. TODO: Proper check here?
    return false;
}

/* Top-Down Construction Algorithm. */
void BoxTreeNode::Construct(int count, Triangle **tri){
    
    //Initialize min, max, scale values for each coordinate.
    float min_X = INFINITY, min_Y = INFINITY, min_Z = INFINITY;//Minimum set to infinity
    float max_X = -INFINITY, max_Y = -INFINITY, max_Z = -INFINITY;//Maximum set to -infinity
    
    numTriangles = count;//Set number of triangles.
    
    //Compute the BoxMin and BoxMax to fit around all the triangles.
    for (int i = 0; i < count; i++){
        Triangle *triangle = tri[i];//Get the current triangle.
        for (int j = 0; j < 3; j++){
            Vertex vertex = triangle->GetVtx(j);//Get each vertex from the triangle.
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
    if(count <= MaxTrianglesPerBox){
        //Copy triangles to BoxTreeNode’s Tri array
        for (int i = 0; i < count; i++){
            Tri[i] = tri[i];
        }
        leaf = true;
        return;
    }
    
    //Determine the largest box dimension: x, y, or z.
    float longest_dim = -INFINITY;
    int dim = 0;
    float dim_x = (max_X - min_X);//0
    float dim_y = (max_Y - min_Y);//1
    float dim_z = (max_Z - min_Z);//2
    if (dim_x > longest_dim) { longest_dim = dim_x; dim = 0; }
    if (dim_y > longest_dim) { longest_dim = dim_y; dim = 1; }
    if (dim_z > longest_dim) { longest_dim = dim_z; dim = 2; }

    //Compute splitting the plane halfway along the largest dimension.
    float split = 0;
    if (dim == 0){//Split across X.
        split = min_X + (longest_dim/2.0f);
    }
    else if (dim == 1){//Split across Y.
        split = min_Y + (longest_dim/2.0f);
    }
    else if (dim == 2){//Split across Z.
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
        if (dim == 0){//Calculate split among X, add into < or >= groups.
            if (center.x < split){ tri1[count1++] = triangle; }
            else { tri2[count2++] = triangle; }
        }
        else if (dim == 1){//Calculate split among Y, add into < or >= groups.
            if (center.y < split){ tri1[count1++] = triangle; }
            else { tri2[count2++] = triangle; }
        }
        else if (dim == 2){//Calculate split among Z, add into < or >= groups.
            if (center.z < split){ tri1[count1++] = triangle; }
            else { tri2[count2++] = triangle; }
        }
    }
    
    //Check if either group is empty. If so, move (at least) 1 triangle into that group
    //TODO: Calculate a more efficient algorithm for splitting.
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

