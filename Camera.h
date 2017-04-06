//
//  Camera.h
//  PA1
//
//  Created by Sebastian Dang on 4/5/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include <stdio.h>
#include "glm/glm.hpp"
#include "Scene.h"
#include "Bitmap.h"


class Camera {

public:
    Camera();
    
    void SetFOV(float f) { VerticalFOV = f; }
    
    void SetAspect(float a) { Aspect = a; }
    
    void SetResolution(int x,int y) { XRes = x; YRes = y; }
    
    void LookAt(glm::vec3 &pos,glm::vec3 &target,glm::vec3 &up);
    
    void Render(Scene &s);
    
    void SaveBitmap(char *filename);
    
private:
    int XRes, YRes;
    glm::mat4x4 WorldMatrix;
    float VerticalFOV;
    float Aspect;
    Bitmap BMP;
};

#endif /* Camera_h */
