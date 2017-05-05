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

        
    }
    
    void GenerateSample(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit) {

        
    }
    
    void setDiffuse(Color diffuseCol){ DiffuseColor = diffuseCol; }
    
private:
    Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////
