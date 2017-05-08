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
        col = DiffuseColor;
    }
    
    void GenerateSample(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit) {
        
        glm::vec3 reflection = (2 * glm::dot(hit.Normal, in) * hit.Normal) - in;
        out = reflection;
        
        //Calculate the reflected color. (Fresnel Metal)
        float nt = 0.617, kt = 2.63;
        float nd = glm::dot(hit.Normal, in);

        float rpar_num = ((powf(nt,2) + powf(kt,2)) * powf(nd, 2)) - (2 * nt * nd) + 1;
        float rpar_denum = ((powf(nt,2) + powf(kt,2)) * powf(nd, 2)) + (2 * nt * nd) + 1;
        
        float rperp_num = (powf(nt,2) + powf(kt,2)) + powf(nd, 2) - (2 * nt * nd);
        float rperp_denum = (powf(nt,2) + powf(kt,2)) + powf(nd, 2) + (2 * nt * nd);
        
        float fresnel = ((rpar_num / rpar_denum) + (rperp_num / rperp_denum)) / 2;
        
        fresnel = 1;//Testing
        
        col.Scale(DiffuseColor, fresnel);
    }
    
    void setDiffuse(Color diffuseCol){ DiffuseColor = diffuseCol; }
    
private:
    Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////
