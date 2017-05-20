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
        
        //----- Compute Diffuse (pd) -----//
        float pd_factor = ((28.0f * DiffuseLevel) / (23.0f * M_PI)) * (1.0f - SpecularLevel);
        float diffuse_k1 = 1.0f - powf((1.0f - (glm::dot(hit.Normal, in) / 2.0f)), 5);
        float diffuse_k2 = 1.0f - powf((1.0f - (glm::dot(hit.Normal, out) / 2.0f)), 5);
        
        float pd = pd_factor * diffuse_k1 * diffuse_k2;
        
        //----- Compute Specular (ps) -----//
        
        //Calculate initial factor
        float ps_factor = sqrtf((RoughnessU + 1.0f) * (RoughnessV + 1.0f));
        ps_factor /= (8.0f * M_PI);
        
        //Calculate exponent
        glm::vec3 h = glm::normalize(in + out);
        float hu = glm::dot(h, hit.TangentU);
        float hv = glm::dot(h, hit.TangentV);
        float hn = glm::dot(h, hit.Normal);
        
        float ps_exp = ((RoughnessU * powf(hu, 2)) + (RoughnessV * powf(hv, 2)));
        ps_exp /= (1.0f - powf(hn, 2));
        
        //Calculate numerator
        float ps_num = powf(glm::dot(hit.Normal, h), ps_exp);
        
        //Calculate denominator
        float ps_den = glm::dot(h, in) * fmaxf(glm::dot(hit.Normal, in), glm::dot(hit.Normal, out));
        
        //Calculate fresnel fraction
        float fresnel = SpecularLevel + ((1.0f - SpecularLevel) * powf((1.0f - glm::dot(h, in)), 5));
        
        float ps = ps_factor * (ps_num / ps_den) * fresnel;
        
        //Combine colors
        Color result = Color::BLACK;
        
        Color diffuseFactor = DiffuseColor;
        Color specularFactor = SpecularColor;
        
        diffuseFactor.Scale(pd);
        specularFactor.Scale(ps);
        
        result.Add(diffuseFactor);
        result.Add(specularFactor);
        
        //Result
        col.Scale(result, 1.0f);
    }
    
    void GenerateSample(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit) {
        glm::vec3 incidence = in;
        glm::vec3 reflection = (2 * glm::dot(hit.Normal, incidence) * hit.Normal) - incidence;
        out = reflection;
        
    }
    
    void SetDiffuse(Color diffuseCol){ DiffuseColor = diffuseCol; }
    
    void SetSpecular(Color diffuseCol){ SpecularColor = diffuseCol; }
    
    void SetDiffuseLevel(float f) { DiffuseLevel = f; }
    
    void SetSpecularLevel(float f) { SpecularLevel = f; }
    
    void SetRoughness(float u, float v) { RoughnessU = u; RoughnessV = v; }
    
private:
    //Both colors.
    Color DiffuseColor;
    Color SpecularColor;
    
    //Both levels for mixing.
    float DiffuseLevel;
    float SpecularLevel;
    
    //Roughness
    float RoughnessU, RoughnessV;
};

////////////////////////////////////////////////////////////////////////////////
