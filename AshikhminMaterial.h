//
//  AshikhminMaterial.hpp
//  CSE168
//
//  Created by Sebastian Dang on 5/19/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#pragma once

#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

class AshikhminMaterial : public Material {
    
public:
    
    void ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit) {
        
        //Calculate perfect specular reflection.
        glm::vec3 specular = (glm::dot(in, hit.Normal) * hit.Normal) - (2.0f * in);
        float brdf = 1.0f;
        
        //BRDF is 0 everywhere except in the direction of perfect specular.
        if (specular.x != out.x && specular.y != out.y && specular.z != out.z){
            brdf = 0.0f;
        }
        
        //Color is constant all around.
        col.Scale(DiffuseColor, brdf);
    }
    
    void GenerateSample(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit) {
        
        glm::vec3 incidence = in;
        glm::vec3 reflection = (2 * glm::dot(hit.Normal, incidence) * hit.Normal) - incidence;
        out = reflection;
        
        //Calculate the reflected color. (Fresnel Metal)
        float nt = 0.617, kt = 2.63;
        float nd = glm::dot(hit.Normal, in);
        
        float rpar_num = ((powf(nt,2) + powf(kt,2)) * powf(nd, 2)) - (2 * nt * nd) + 1;
        float rpar_denum = ((powf(nt,2) + powf(kt,2)) * powf(nd, 2)) + (2 * nt * nd) + 1;
        
        float rperp_num = (powf(nt,2) + powf(kt,2)) + powf(nd, 2) - (2 * nt * nd);
        float rperp_denum = (powf(nt,2) + powf(kt,2)) + powf(nd, 2) + (2 * nt * nd);
        
        float fresnel = ((rpar_num / rpar_denum) + (rperp_num / rperp_denum)) / 2;
        
        fresnel = 1.0f;//Testing
        
        col.Scale(DiffuseColor, fresnel);
    }
    
    void SetRoughness(float f, float f2) { }
    
    void SetSpecularLevel(float f) { }
    
    void setSpecular(Color diffuseCol){ }
    
    void SetDiffuseLevel(float f) { }
    
    void setDiffuse(Color diffuseCol){ DiffuseColor = diffuseCol; }
    
private:
    Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////
