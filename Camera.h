//
//  Camera.h
//  PA1
//
//  Created by Sebastian Dang on 4/5/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#ifndef Camera_h
#define Camera_h

#include <stdio.h>
#include <glm/glm.hpp>
#include "Scene.h"
#include "Bitmap.h"


class Camera {

public:
    Camera();
    
    void SetFOV(float f) { VerticalFOV = M_PI * f / 180.0f; }
    
    void SetAspect(float a) { Aspect = a; }
    
    void SetResolution(int x,int y) { XRes = x; YRes = y; BMP.Resize(x,y); }
    
    void LookAt(const glm::vec3 &pos, const glm::vec3 &target, const glm::vec3 &up);
    
    void Render(Scene &s);
    
    void SetSuperSample(int xsamples, int ysamples); //Supersample each pixel in a x*y grid.
    
    void SetJitter(bool enable); //Toggle Jittering for each supersample.
    
    void SetShirley(bool enable); //Toggle Shirley mapping.
    
    void SaveBitmap(char *filename);
    
private:
    
    //Camera properties.
    float VerticalFOV;
    float Aspect;
    int XRes, YRes;
    
    //Output Bitmap.
    Bitmap BMP;
    
    //Camera's Matrix in world space.
    glm::mat4x4 WorldMatrix;
    
    //Anti Aliasing.
    int X_Samples, Y_Samples;
    bool Jitter, Shirley;
    
    //Gamma.
    int GammaCorrection(Color sample, float gamma);
    
};

#endif /* Camera_h */
