//
//  InstanceObject.cpp
//  PA1
//
//  Created by Sebastian Dang on 4/5/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#include "InstanceObject.h"

//Empty Constructor.
InstanceObject::InstanceObject(){
    //Set defaults just in case.
    Matrix = glm::mat4x4(1.0f);
    Inverse = glm::inverse(Matrix);
    
    Child = NULL;
}

//Constructor that takes in a pointer to an object.
InstanceObject::InstanceObject(Object &obj){
    //Set defaults just in case.
    Matrix = glm::mat4x4(1.0f);
    Inverse = glm::inverse(Matrix);
    
    Child = &obj;
}

InstanceObject::~InstanceObject(){
    
}

//Instance object intersection routine.
bool InstanceObject::Intersect(const Ray &ray,Intersection &hit){
    
    //Create ray2 from ray.
    Ray ray2;
    ray2.Origin = glm::vec3(Inverse * glm::vec4(ray.Origin, 1.0f));
    ray2.Direction = glm::vec3(Inverse * glm::vec4(ray.Direction, 1.0f));
    
    //Check if the child object intersects.
    if (Child->Intersect(ray2, hit) == false) return false;
    
    //If intersection is true, set the hit position.
    hit.Position = glm::vec3(Matrix * glm::vec4(hit.Position, 1.0f));
    hit.Normal = glm::vec3(Matrix * glm::vec4(hit.Normal, 1.0f));
    hit.HitDistance = glm::distance(ray.Origin, hit.Position); //Correct for any scaling.
    return true;
}


