//
//  Camera.cpp
//  PA1
//
//  Created by Sebastian Dang on 4/5/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#include "stdafx.h"
#include <future>
#include <thread>
#include "Camera.h"
#include "Ray.h"
#include "RayTrace.h"

//Empty Constructor.
Camera::Camera(): BMP(0,0) {
    
    //Initial values.
    VerticalFOV = 0.0f;
    Aspect = 0.0f;
    XRes = 0;
    YRes = 0;
    
    //Initial matrix.
    WorldMatrix = glm::mat4x4(0.0f);
    
    //Super Sampling.
    X_Samples = 1;
    Y_Samples = 1;
    Jitter = false;
    Shirley = false;
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
    
    //Multi-threading.
    std::size_t max = YRes * XRes;
    std::size_t cores = std::thread::hardware_concurrency();
    std::vector<std::future<void>> future_vector;
    
    for (std::size_t core(0); core < cores; ++core){
        future_vector.emplace_back(std::async([=, &s](){
            for (std::size_t index(core); index < max; index += cores){
                
                //Recalculate x and y.
                std::size_t x = index % XRes;
                std::size_t y = index / XRes;
                
                //----- Perform Rendering here -----//
                
                //Accumulated Color.
                Color accumCol = Color::BLACK;
                
                //SuperSample. For each super sample...
                for (int i = 0; i < X_Samples; i++){
                    for (int j = 0; j < Y_Samples; j++){
                        
                        //Compute subpixel position (0...1 range in x & y).
                        float sub_x = float(i) / float(X_Samples);
                        float sub_y = float(j) / float(Y_Samples);
                        
                        //Apply Shirley to subpixel position.
                        if (Shirley){
                            //X
                            if (sub_x < 0.5){
                                sub_x = -0.5f + sqrtf(2.0f*sub_x);
                            }
                            else if (sub_x >= 0.5){
                                sub_x = 1.5f - sqrtf(2.0f - 2.0f*sub_x);
                            }
                            //Y
                            if (sub_y < 0.5){
                                sub_y = -0.5f + sqrtf(2.0f*sub_y);
                            }
                            else if (sub_y >= 0.5){
                                sub_y = 1.5f - sqrtf(2.0f - 2.0f*sub_y);
                            }
                        }
                        
                        //Apply Jitter to subpixel position.
                        if (Jitter){
                            //Get jitter vals.
                            float jitter_x = (((rand()%200) - 100) / 100.0f) / X_Samples;
                            float jitter_y = (((rand()%200) - 100) / 100.0f) / Y_Samples;
                            
                            //Apply.
                            sub_x += jitter_x;
                            sub_y += jitter_y;
                        }
                        
                        //Turn the subpixel position into pixel position & trace path.
                        float sample_x = x + sub_x;
                        float sample_y = y + sub_y;
                        
                        //Get the center x and y coordinates.
                        float fx = ((float(sample_x) + 0.5f) / float(XRes)) - 0.5f;
                        float fy = ((float(sample_y) + 0.5f) / float(YRes)) - 0.5f;
                        
                        //Create the ray.
                        Ray camera_ray;
                        camera_ray.Origin = glm::vec3(d);
                        camera_ray.Direction = glm::normalize( (fx * scaleX * a) + (fy * scaleY * b) - c);
                        
                        //Check if the camera ray intersects with any objects in the scene.
                        Intersection hit;
                        
                        //Calculate color.
                        RayTrace raytrace = RayTrace(s);
                        raytrace.TraceRay(camera_ray, hit, 1);
                        
                        //Add to accumulation.
                        accumCol.Add(hit.Shade);
                    }
                }
                
                //Scale from the super sample.
                float scale = float(1)/(X_Samples * Y_Samples);
                accumCol.Scale(scale);
                
                //Set the pixel.
                //BMP.SetPixel((int)x, (int)y, GammaCorrection(accumCol, 1.8f));
                BMP.SetPixel((int)x, (int)y, accumCol.ToInt());

                //----- End Rendering here -----//
            }
        }));
    }

}

//Gamma.
int Camera::GammaCorrection(Color sample, float gamma){
    
    Color gammaColor = sample;
    
    float r = sample.getRed();
    float g = sample.getGreen();
    float b = sample.getBlue();
    
    r = powf(r, 1.0f/gamma);
    g = powf(g, 1.0f/gamma);
    b = powf(b, 1.0f/gamma);

    gammaColor.setRed(r);
    gammaColor.setGreen(g);
    gammaColor.setBlue(b);
    
    return gammaColor.ToInt();
}


//Set the super sample size.
void Camera::SetSuperSample(int xsamples, int ysamples){
    X_Samples = xsamples;
    Y_Samples = ysamples;
}

//Set Jitter to true or false.
void Camera::SetJitter(bool enable){
    Jitter = enable;
}

//Set Shirley to true or false.
void Camera::SetShirley(bool enable){
    Shirley = enable;
}

//Save the Bitmap by writing it to a file.
void Camera::SaveBitmap(char *filename){
    BMP.SaveBMP(filename);
}
