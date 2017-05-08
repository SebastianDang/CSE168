////////////////////////////////////////
// LambertMaterial.h
////////////////////////////////////////

#pragma once

#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

class LambertMaterial:public Material {

public:
    
	void ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit) {
//		float pi=3.1415926f;
//		col.Scale(DiffuseColor,1.0f/pi);//Original Color.
        col = DiffuseColor;
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
        glm::vec3 y = glm::vec3(hit.Normal);
        
        //Calculate h.
        glm::vec3 h = y;
        if (fabsf(h.x) <= fabsf(h.y) && fabsf(h.x) <= fabsf(h.z)){
            h.x = 1.0f;
        }
        else if (fabsf(h.y) <= fabsf(h.x) && fabsf(h.y) <= fabsf(h.z)){
            h.y = 1.0f;
        }
        else{
            h.z = 1.0f;
        }
        
        //Calculate x and z.
        glm::vec3 x = glm::cross(h, y);
        glm::vec3 z = glm::cross(x, y);
        
        //Return the new direction.
        out = point.x * x + point.y * y + point.z * z;
        //out = glm::dot(point, hit.Normal) * in;//Old
        
        //Constant color.
        col = DiffuseColor;

    }
    
    void setDiffuse(Color diffuseCol){ DiffuseColor = diffuseCol; }

private:
	Color DiffuseColor;
};

////////////////////////////////////////////////////////////////////////////////
