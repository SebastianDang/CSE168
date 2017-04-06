////////////////////////////////////////
// Scene.h
////////////////////////////////////////

#pragma once

#include "Object.h"
#include "Light.h"
#include <vector>

////////////////////////////////////////////////////////////////////////////////

class Scene {
public:
	Scene()										{SkyColor.Set(0.2f,0.2f,0.5f);}

	void AddObject(Object &obj)					{Objects.push_back(&obj);}
	void AddLight(Light &lgt)					{Lights.push_back(&lgt);}
	void SetSkyColor(const Color sky)			{SkyColor=sky;}

	int GetNumLights()							{return Lights.size();}
	Light &GetLight(int i)						{return *Lights[i];}
	Color GetSkyColor()							{return SkyColor;}

	bool Intersect(const Ray &ray,Intersection &hit) {
		bool success=false;
		for(unsigned int i=0;i<Objects.size();i++)
			if(Objects[i]->Intersect(ray,hit)) success=true;
		return success;
	}

private:
	std::vector<Object*> Objects;
	std::vector<Light*> Lights;
	Color SkyColor;
};

////////////////////////////////////////////////////////////////////////////////
