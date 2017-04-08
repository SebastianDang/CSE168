//
//  PointLight.h
//  PA1
//
//  Created by Sebastian Dang on 4/5/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#ifndef PointLight_h
#define PointLight_h

#include <stdio.h>
#include "Light.h"

class PointLight: public Light {

public:
    PointLight();
    
    float Illuminate(const glm::vec3 &pos, Color &col, glm::vec3 &toLight, glm::vec3 &ltPos) {
        toLight = Position - pos;
        float bright = Intensity / (glm::length(toLight)*glm::length(toLight)); // Inverse square falloff
        toLight = glm::normalize(toLight);
        col = BaseColor;
        ltPos = Position;
        return bright;
    }
    
    void SetPosition(const glm::vec3 &pos) { Position = pos; }

private:
    glm::vec3 Position;
    
};

#endif /* PointLight_h */
