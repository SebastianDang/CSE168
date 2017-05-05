//
//  RayTrace.cpp
//  CSE168
//
//  Created by Sebastian Dang on 4/13/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#include "RayTrace.h"
#include "Material.h"
#include "LambertMaterial.h"
#include "MetalMaterial.h"

RayTrace::RayTrace(Scene &s){
    Scn = &s;
    MaxDepth = 5;
}

bool RayTrace::TraceRay(const Ray &ray, Intersection &hit, int depth){
    
    //Find the ray intersection
    if(Scn->Intersect(ray,hit)==false) {
        hit.Shade=Scn->GetSkyColor();//Set to sky color.
        return false;
    }
    
    //Compute shade due to lighting
    hit.Shade = Color::BLACK;
    
    //For each light.
    for (int i = 0; i < Scn->GetNumLights(); i++){
        
        //Light variables.
        glm::vec3 pos = hit.Position;
        Color col;//Returned color set in illuminate.
        glm::vec3 toLight;//Shadows
        glm::vec3 ltPos;//Shadows
        
        //Calculate light intensity.
        float intensity = Scn->GetLight(i).Illuminate(pos, col, toLight, ltPos);
        float shadow = glm::dot(toLight, hit.Normal);//Check shadow > 0
        
        //Check if intensity == 0 or hit.Normal doesn't face the light.
        if (intensity == 0 || shadow < 0){//dot product must be positive, 0 is fine.
            continue;
        }
        
        //Create shadow ray towards the light.
        Ray shadow_ray;
        shadow_ray.Origin = hit.Position;
        shadow_ray.Direction = toLight;
        
        //If shadow ray isn't blocked, add contribution to hit.Shade.
        Intersection s_hit;
        s_hit.HitDistance = glm::length(ltPos-shadow_ray.Origin);//We don't intersect past light source.
        
        if (Scn->Intersect(shadow_ray, s_hit)){
            //Intersect, check if blocked.
            float object_distance = glm::length(s_hit.Position-shadow_ray.Origin);//Shadow ray to object
            float light_distance = glm::length(ltPos-shadow_ray.Origin);//Shadow ray to light
            
            //If the object hit is less than the light distance, there's an obstruction.
            if (object_distance < light_distance){
                continue;
            }
        }
        
        //Add in reflectance intensity to the shadows.
        if (hit.Mtl != NULL) {
            hit.Mtl->ComputeReflectance(col, -ray.Direction, toLight, hit);
        }

        //Add contribution to hit.Shade.
        col.Scale(shadow * intensity);
        hit.Shade.Add(col);
    }
    
    if(depth == MaxDepth) return true;
    
    //Compute shade due to reflections/refractions
    if (hit.Mtl != NULL){
        
        //Generate newRay & newHit.
        Ray newRay;
        newRay.Origin = hit.Position;
        Intersection newHit;
        
        //Create color from reflection/refraction.
        Color outColor;
        
        //Compute ray intensity (based on fR or fT).
        hit.Mtl->GenerateSample(outColor, -ray.Direction, newRay.Direction, hit);
        
        //Recursive TraceRay.
        TraceRay(newRay, newHit, depth + 1);
        
        //hit.Shade += intensity * newhit.Shade
        newHit.Shade.Multiply(outColor);
        
        hit.Shade.Add(newHit.Shade);
    }
    
    return true;
}
