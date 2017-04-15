//
//  RayTrace.h
//  CSE168
//
//  Created by Sebastian Dang on 4/13/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#ifndef RayTrace_h
#define RayTrace_h

#include <stdio.h>
#include "Scene.h"

class RayTrace {

public:

    RayTrace(Scene &s);
    
    bool TraceRay(const Ray &ray, Intersection &hit, int depth=1);
    
private:
    
    //Scene
    Scene *Scn;
    
    //Settings
    int MaxDepth;
    
    //Statistics
    int PrimaryRays;
    int SecondaryRays;
    int ShadowRays;
    
};





#endif /* RayTrace_h */
