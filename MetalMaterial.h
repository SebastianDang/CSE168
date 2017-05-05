//
//  MetalMaterial.h
//  CSE168
//
//  Created by Sebastian Dang on 5/1/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#pragma once

#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

class MetalMaterial: public Material {
    
public:
    
    void ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit) {
        float pi=3.1415926f;
        col.Scale(DiffuseColor,1.0f/pi);
    }
    
    void GenerateSample(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit) {
        
        //Generate random points s and t. (Both kept in ranges 0 and 1).
        float s = ((rand()%100) / 100.0f);
        float t = ((rand()%100) / 100.0f);
        
        //Cosine Weighted Hemisphere
        float u = 2 * M_PI * s;
        float v = sqrtf(1-t);
        
        glm::vec3 point;
        point.x = v * cosf(u);
        point.y = sqrtf(t);
        point.z = v * sinf(u);
        
        //Return the cosine weighted vector in the direction around the normal.
        out = glm::dot(point, hit.Normal) * in;
        
        //Return the color.
        col = DiffuseColor;
    }
    
    void setDiffuse(Color diffuseCol){ DiffuseColor = diffuseCol; }
    
private:
    Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////
