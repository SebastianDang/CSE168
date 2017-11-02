////////////////////////////////////////
// Light.h
////////////////////////////////////////

#pragma once

#include "Color.h"
#include <glm/glm.hpp>

////////////////////////////////////////////////////////////////////////////////

class Light {
public:
	Light()									{Intensity=1.0; BaseColor=Color::WHITE;}
	void SetBaseColor(const Color &col)		{BaseColor=col;}
	void SetIntensity(float i)				{Intensity=i;}

	virtual float Illuminate(const glm::vec3 &pos, Color &col, glm::vec3 &toLight, glm::vec3 &ltPos)=0;

protected:
	float Intensity;
	Color BaseColor;		// Actual color is Intensity*BaseColor
};

////////////////////////////////////////////////////////////////////////////////
