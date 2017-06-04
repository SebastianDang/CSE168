//
//  BoxTreeObject.h
//  CSE168
//
//  Created by Sebastian Dang on 4/16/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#ifndef BoxTreeObject_h
#define BoxTreeObject_h

#include <stdio.h>
#include "BoxTreeNode.h"
#include "MeshObject.h"
#include "ProceduralObject.h"

class BoxTreeObject: public Object {

public:
    BoxTreeObject();
    ~BoxTreeObject();
    
    void Construct(MeshObject &mesh);
    void Construct(ProceduralObject &proc);
    
    bool Intersect(const Ray &ray, Intersection &hit);

private:
    BoxTreeNode *RootNode;
};

#endif /* BoxTreeObject_h */
