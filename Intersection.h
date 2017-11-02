////////////////////////////////////////
// Intersection.h
////////////////////////////////////////

#pragma once

#include "Color.h"
#include <glm/glm.hpp>
class Material;

////////////////////////////////////////////////////////////////////////////////

class Intersection {
public:
    Intersection() {HitDistance=1e10; Position=glm::vec3(0.0f); Normal=glm::vec3(0.0f); Mtl=0; Shade = Color::BLACK; }

public:
	// Ray intersection data
	float HitDistance;
	glm::vec3 Position;
	glm::vec3 Normal;
	Material *Mtl;
    
    // Texture Coords 
    glm::vec3 TangentU, TangentV;

	// Shaded color
	Color Shade;
};

////////////////////////////////////////////////////////////////////////////////
