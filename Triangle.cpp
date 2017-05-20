//
//  Triangle.cpp
//  PA1
//
//  Created by Sebastian Dang on 4/5/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#include <stdio.h>
#include "Triangle.h"

#define PRECISION_OFFSET 0.001f

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
    if (t < PRECISION_OFFSET || t>= hit.HitDistance) return false;//Verify t>0 and t<HitDistance (already hit).
    
    //alpha = -d dot [(p-a) x (c-a)] / det(M)
    float alpha = glm::dot(-d, glm::cross(p-a, c-a)) / detM;
    if (alpha < 0) return false;//Verify alpha>0
    
    //beta = -d dot [(b-a) x (p-a)] / det(M)
    float beta = glm::dot(-d, glm::cross(b-a, p-a)) / detM;
    if (beta < 0) return false;//Verify beta>0
    if ((alpha + beta) > 1) return false;//Verify a+b<1
    
    //Compute hit intersection values.
    hit.Position = p + (t * d);//Ray equation.
    hit.HitDistance = glm::distance(ray.Origin, hit.Position);//Correct for any scaling.
    hit.HitDistance -= PRECISION_OFFSET;
    
    //Normals
    glm::vec3 na = Vtx[0]->Normal;
    glm::vec3 nb = Vtx[1]->Normal;
    glm::vec3 nc = Vtx[2]->Normal;
    
    //Smooth shade the triangle (normal).
    hit.Normal = ((1-alpha-beta) * na) + (alpha * nb) + (beta * nc);
    
    //Check if the back or front is hit. Normal needs to point towards ray origin.
    if (glm::dot(hit.Normal, ray.Direction) > 0.0f){
        hit.Normal = -1.0f * hit.Normal;//flip.
    }
    
    //Triangle Texture Coordinates. TODO: Slide 2, p62.
    //u = ((1-alpha-beta) * u_a) + (alpha * u_b) + (beta * u_c)
    //v = ((1-alpha-beta) * v_a) + (alpha * v_b) + (beta * v_c)
    hit.TangentU = glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), hit.Normal);
    if(glm::length(hit.TangentU) < 0.0001f)
        hit.TangentU = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), hit.Normal);
    hit.TangentU = glm::normalize(hit.TangentU);
    hit.TangentV = glm::cross(hit.Normal, hit.TangentU);
    
    //Set the hit material.
    if (this->Mtl != NULL){
        hit.Mtl = Mtl;
    }

    return true;
}

glm::vec3 Triangle::GetCenter(){
    
    //Positions
    glm::vec3 a = Vtx[0]->Position;
    glm::vec3 b = Vtx[1]->Position;
    glm::vec3 c = Vtx[2]->Position;
    
    //Barycentric coordinates
    float alpha = 0.5, beta = 0.5;
    
    //Calculate center
    glm::vec3 center = a + (alpha * (b-a)) + (beta * (c-a));
    
    return center;
}
