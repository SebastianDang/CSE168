//
//  InstanceObject.h
//  PA1
//
//  Created by Sebastian Dang on 4/5/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#ifndef InstanceObject_h
#define InstanceObject_h

#include <stdio.h>
#include "Object.h"

class InstanceObject: public Object {

public:
    
    InstanceObject();
    
    InstanceObject(Object &obj);
    
    ~InstanceObject();
    
    void SetMatrix(glm::mat4x4 &mtx) { Matrix = mtx; Inverse = glm::inverse(mtx); }
    
    void SetChild(Object &obj) { Child = &obj; }
    
    void SetMaterial(Material &mat) { Material = &mat; }
    
    bool Intersect(const Ray &ray,Intersection &hit);
    
private:
    
    glm::mat4x4 Matrix;
    glm::mat4x4 Inverse; //Pre-computed inverse of Matrix.
    
    Object *Child; //Pointer to child.
    Material *Material;
    
};
#endif /* InstanceObject_h */
