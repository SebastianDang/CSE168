//
//  Camera.cpp
//  PA1
//
//  Created by Sebastian Dang on 4/5/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#include "Camera.h"


Camera::Camera(): BMP(0,0) {
    
    XRes = 0;
    YRes = 0;
    
    //glm::mat4x4 WorldMatrix;
    
    VerticalFOV = 0.0f;
    Aspect = 0.0f;

}


void Camera::LookAt(glm::vec3 &pos,glm::vec3 &target,glm::vec3 &up){
    
}

void Camera::Render(Scene &s){
    
}

void Camera::SaveBitmap(char *filename){
    
}
