////////////////////////////////////////
// Ray.h
////////////////////////////////////////

#pragma once

#include <glm/glm.hpp>

////////////////////////////////////////////////////////////////////////////////

class Ray {
public:
	glm::vec3 Origin;
	glm::vec3 Direction;
    float Type; // 0 = Primary, 1 = Shadows, 2 = Reflection.
};

////////////////////////////////////////////////////////////////////////////////
