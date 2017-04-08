//
//  Triangle.cpp
//  PA1
//
//  Created by Sebastian Dang on 4/5/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#include <stdio.h>
#include "Triangle.h"

//Empty Constructor.
Triangle::Triangle(){
}

//Calculate the intersection of the ray to this triangle.
bool Triangle::Intersect(const Ray &ray, Intersection &hit) const{
    
    //Ray - r(t) = p + td
    //q = a + [ alpha * (b-a) ] + [ beta * (c-a) ]

    //Get vertices.
    Vertex *vertex_a = Vtx[0];
    Vertex *vertex_b = Vtx[1];
    Vertex *vertex_c = Vtx[2];
    
    //Position
    glm::vec3 a = vertex_a->Position;
    glm::vec3 b = vertex_b->Position;
    glm::vec3 c = vertex_c->Position;
    
    //Ray
    glm::vec3 p = ray.Origin;
    glm::vec3 d = ray.Direction;
    
    //det(M) = -d dot [(b-a) x (c-a)]
    glm::vec3 negD = -d;
    glm::vec3 BminusA = b - a;
    glm::vec3 CminusA = c - a;
    glm::vec3 BAcrossCA = glm::cross(BminusA, CminusA);
    float detM = glm::dot(negD, BAcrossCA);
    
    //t = (p-a) dot [(b-a) x (c-a)] / det(M)
    glm::vec3 PminusA = p - a;
    float t = glm::dot(PminusA, BAcrossCA);
    
    //alpha = -d dot [(p-a) x (c-a)] / det(M)
    glm::vec3 PAcrossCA = glm::cross(PminusA, CminusA);
    float alpha = (glm::dot(negD, PAcrossCA)) / detM;
    
    //beta = -d dot [(b-a) x (p-a)] / det(M)
    glm::vec3 BAcrossPA = glm::cross(BminusA, PminusA);
    float beta = (glm::dot(negD, BAcrossPA)) / detM;
    
    //Check
    if (detM == 0) return false;
    if (alpha < 0) return false;
    if (beta < 0) return false;
    if (alpha + beta > 1) return false;
    if (t < 0) return false;
    
    return true;
}
