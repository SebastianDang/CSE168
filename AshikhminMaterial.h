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
        float pd_factor = ((28.0f * DiffuseLevel) / (10.0f * M_PI)) * (1.0f - SpecularLevel);
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

        float random = ((rand()%100) / 100.0f);
        
        //Specular component.
        if (random < SpecularLevel){
            
            //Calculate the direction.
            CalculateSpecularDirection(in, out, hit);
            
            //Set the color.
            if (glm::dot(out, hit.Normal) < 0.0f) {
                //col = Color::BLACK;
                col.Scale(SpecularColor, SpecularLevel);
            }
            else {
                col.Scale(SpecularColor, SpecularLevel);
            }
        }
        //Diffuse component.
        else {
            
            //Calculate the direction.
            CalculateDiffuseDirection(in, out, hit);
            
            //Set the color.
            col.Scale(DiffuseColor, 1.0f);
        }
       
    }
    
    //Only sets out.
    void CalculateSpecularDirection(const glm::vec3 &in, glm::vec3 &out, const Intersection &hit){
        
        //Given random numbers (Kept in ranges 0 and 1)
        float rand_1 = ((rand()%100) / 100.0f);
        float rand_2 = ((rand()%100) / 100.0f);
        float rand_3 = ((rand()%100) / 100.0f);
        
        //Calculate cos/sin of phi (Equation 9)
        float phi = atan(sqrtf((RoughnessU + 1.0f) / (RoughnessV + 1.0f)) * tan((M_PI * rand_1) / 2.0f));
        float cos_phi = cos(phi);
        float sin_phi = sin(phi);
        
        //Sample the entire hemisphere.
        if (rand_3 > 0.75f) {
            phi = 2.0f * M_PI - phi;
        } else if (rand_3 > 0.5f) {
            //phi = 2.0f * M_PI - (M_PI/2.0f) - phi;
            phi = M_PI + phi;
        } else if (rand_3 > 0.25f) {
            //phi = 2.0f * M_PI - M_PI - phi;
            phi = M_PI - phi;
        }
        
        //Calculate cos/sin of theta (Equation 10)
        float cos_exp = 1.0f / (RoughnessU * powf(cos(phi), 2) + RoughnessV * powf(sin(phi), 2) + 1);
        float cos_theta = powf((1.0f - rand_2), cos_exp);
        float sin_theta = sqrt(1.0f - cos_theta*cos_theta);//Pythagorean
        
        //Calculate k2 using h (Equation 7)
        glm::vec3 h = (hit.Normal * cos_theta) + (hit.TangentU * sin_theta * cos_phi) + (hit.TangentV * sin_theta * sin_phi);
        glm::vec3 k2 = (2 * glm::dot(h, in) * h) - in;
        out = k2;
        
        /*
         
        //Calculate ph (Equation 6)
        float h_factor = sqrtf((RoughnessU + 1.0f) * (RoughnessV + 1.0f)) / (2 * M_PI);
        float nh = glm::dot(hit.Normal, h);
        float h_exp = (RoughnessU * powf(cos(phi), 2)) + (RoughnessV * powf(sin(phi), 2));
        float p_h = h_factor * powf(nh, h_exp);
        
        //Calculate pk (Equation 8)
        float p_k2 = p_h / (4 * glm::dot(in, h));
        
        */
        
    }
    
    //Only sets out.
    void CalculateDiffuseDirection(const glm::vec3 &in, glm::vec3 &out, const Intersection &hit){
        
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
        
        //Return the new direction.
        out = point.x * hit.TangentU + point.y * hit.Normal + point.z * hit.TangentV;
        out = glm::normalize(out);
        
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
