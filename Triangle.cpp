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
    
    //Positions
    glm::vec3 a = Vtx[0]->Position;
    glm::vec3 b = Vtx[1]->Position;
    glm::vec3 c = Vtx[2]->Position;
    
    //Rays
    glm::vec3 p = ray.Origin;
    glm::vec3 d = ray.Direction;
    
    //det(M) = -d dot [(b-a) x (c-a)]
    float detM = glm::dot(-d, glm::cross(b-a, c-a));
    if (detM == 0) return false;//Ray is parallel to the plane.
    
    //t = (p-a) dot [(b-a) x (c-a)] / det(M)
    float t = glm::dot(p-a, glm::cross(b-a, c-a)) / detM;
    if (t <= 0) return false;//Verify t>0
    
    //alpha = -d dot [(p-a) x (c-a)] / det(M)
    float alpha = glm::dot(-d, glm::cross(p-a, c-a)) / detM;
    if (alpha <= 0) return false;//Verify alpha>0
    
    //beta = -d dot [(b-a) x (p-a)] / det(M)
    float beta = glm::dot(-d, glm::cross(b-a, p-a)) / detM;
    if (beta <= 0) return false;//Verify beta>0
    if ((alpha + beta) >= 1) return false;//Verify a+b<1
    
    //Compute hit intersection values.
    hit.Position = p + (t * d);
    hit.HitDistance = glm::distance(ray.Origin, hit.Position); //Correct for any scaling.
    
    //Smooth shade the triangle (normal).
    //glm::vec3 triangle_normal = glm::normalize(glm::cross(b-a, c-a));
    
    glm::vec3 na = Vtx[0]->Normal;
    glm::vec3 nb = Vtx[1]->Normal;
    glm::vec3 nc = Vtx[2]->Normal;
    
    hit.Normal = ((1-alpha-beta) * na) + (alpha * nb) + (beta * nc);
    
    //TODO: Check if the back or front is hit.
    
    return true;
}
