//
//  Camera.cpp
//  PA1
//
//  Created by Sebastian Dang on 4/5/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#include "Camera.h"
#include "Ray.h"

//Empty Constructor.
Camera::Camera(): BMP(0,0) {
    
    //Initial values.
    VerticalFOV = 0.0f;
    Aspect = 0.0f;
    XRes = 0;
    YRes = 0;
    
    //Initial matrix.
    WorldMatrix = glm::mat4x4(0.0f);
}

//Camera lookat matrix from position, target, and up vectors.
void Camera::LookAt(const glm::vec3 &pos, const glm::vec3 &target, const glm::vec3 &up){
    
    glm::vec3 d = pos;
    glm::vec3 c = glm::normalize(d - target);
    glm::vec3 a = glm::normalize(glm::cross(up,c));
    glm::vec3 b = glm::cross(c,a);
    
    WorldMatrix = glm::mat4x4(a.x,a.y,a.z,0.0f, b.x,b.y,b.z,0.0f, c.x,c.y,c.z,0.0f, d.x,d.y,d.z,1.0f);
}

//Render the scene onto the Bitmap.
void Camera::Render(Scene &s){
    
    //Variables.
    int x,y;
    
    //Camera vectors.
    glm::vec3 a = glm::vec3(WorldMatrix[0]);
    glm::vec3 b = glm::vec3(WorldMatrix[1]);
    glm::vec3 c = glm::vec3(WorldMatrix[2]);
    glm::vec3 d = glm::vec3(WorldMatrix[3]);
    
    //Construct fovs.
    float vfov = VerticalFOV;
    float hfov = 2 * atan(Aspect * tan(vfov/2));
    
    //Scale factors.
    float scaleX = 2 * tan(hfov/2);
    float scaleY = 2 * tan(vfov/2);
    
    //Loop through each pixel.
    for (y = 0; y < YRes; y++){
        for (x = 0; x < XRes; x++){
            
            //Get the center x and y coordinates.
            float fx = ((float(x) + 0.5f) / float(XRes)) - 0.5f;
            float fy = ((float(y) + 0.5f) / float(YRes)) - 0.5f;
            
            //Create the ray.
            Ray camera_ray;
            camera_ray.Origin = glm::vec3(d);
            camera_ray.Direction = glm::normalize( (fx * scaleX * a) + (fy * scaleY * b) - c);
            
            //Check if the camera ray intersects with any objects in the scene.
            Intersection hit;
            hit.HitDistance = 1000;
            
            //Scene.Intersect( ... )
            if (s.Intersect(camera_ray, hit)){
                
                //Result pixel color.
                Color resultPixel = Color();
                
                //Compute with lighting.
                
                
                
                
                
                
                
                //We set the pixel accordingly.
                BMP.SetPixel(x, y, resultPixel.ToInt());
            }
            else{
                BMP.SetPixel(x, y, s.GetSkyColor().ToInt());
            }
            
            
            
        }
    }

}

//Save the Bitmap by writing it to a file.
void Camera::SaveBitmap(char *filename){
    BMP.SaveBMP(filename);
    
}
