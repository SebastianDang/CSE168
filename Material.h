////////////////////////////////////////
// Material.h
////////////////////////////////////////

#pragma once

#include "Intersection.h"

////////////////////////////////////////////////////////////////////////////////

class Material {
    
public:
    
	virtual void ComputeReflectance(Color &col, const glm::vec3 &in, const glm::vec3 &out, const Intersection &hit)=0;
    
    virtual void GenerateSample(Color &col, const glm::vec3 &in, glm::vec3 &out, const Intersection &hit)=0;
    
private:
    
    
};

////////////////////////////////////////////////////////////////////////////////
