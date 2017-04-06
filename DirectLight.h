//
//  DirectLight.h
//  PA1
//
//  Created by Sebastian Dang on 4/5/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#ifndef DirectLight_h
#define DirectLight_h

#include <stdio.h>
#include "Light.h"

class DirectLight:public Light {
    
public:
    DirectLight();
    
    float Illuminate(const glm::vec3 &pos, Color &col, glm::vec3 &toLight, glm::vec3 &ltPos) {
        toLight = -Direction;
        col = BaseColor;
        ltPos = pos - (1000000.0f * Direction); //Create virtual position.
        return Intensity;
    }
    
    void SetDirection(const glm::vec3 &dir) { Direction = glm::normalize(dir); }
    
private:
    glm::vec3 Direction;
    
};

#endif /* DirectLight_h */
