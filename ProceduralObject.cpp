//
//  ProceduralObject.cpp
//  CSE168
//
//  Created by Sebastian Dang on 6/3/17.
//  Copyright © 2017 Sebastian Dang. All rights reserved.
//

#include "ProceduralObject.h"
#include "LambertMaterial.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include "glm/glm.hpp"


#define VERTEX_COUNT 32
#define SIZE 1

ProceduralObject::ProceduralObject() {
    NumVertexes=0;
    NumTriangles=0;
    NumMaterials=0;
    
    Vertexes=0;
    Triangles=0;
    Materials=0;
}

////////////////////////////////////////////////////////////////////////////////

ProceduralObject::~ProceduralObject() {
    delete []Vertexes;
    delete []Triangles;
    delete []Materials;
}

////////////////////////////////////////////////////////////////////////////////

bool ProceduralObject::Intersect(const Ray &ray, Intersection &hit) {
    bool success=false;
    for(int i=0;i<NumTriangles;i++)
        if(Triangles[i].Intersect(ray,hit)) success=true;
    return success;
}

////////////////////////////////////////////////////////////////////////////////

void ProceduralObject::Generate(float x, float y, float z, Material *mtl){
    
    //Allocate arrays
    NumVertexes = VERTEX_COUNT*VERTEX_COUNT;
    NumTriangles = (VERTEX_COUNT-1)*(VERTEX_COUNT-1)*2;
    Vertexes = new Vertex[NumVertexes];
    Triangles = new Triangle[NumTriangles];
    if(mtl==0) mtl=new LambertMaterial;
    
    //Create the height map: v, vn, texCoords
    for (int i = 0; i < VERTEX_COUNT; i++)
    {
        for (int j = 0; j < VERTEX_COUNT; j++)
        {
            //Setup the vertices.
            float vertex_x = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
            float vertex_y = 0.0f;
            float vertex_z = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
            //Setup the normals.
            float normal_x = 0;
            float normal_y = 1.0f;
            float normal_z = 0;
            //Setup the texcoords.
            float texCoord_x = (float)j / ((float)VERTEX_COUNT - 1);
            float texCoord_y = (float)i / ((float)VERTEX_COUNT - 1);
            
            int index = i*VERTEX_COUNT + j;
            Vertexes[index].Set(glm::vec3(vertex_x, vertex_y, vertex_z),
                                glm::vec3(normal_x, normal_y, normal_z),
                                glm::vec3(texCoord_x, texCoord_y, 0.0f));
        }
    }
    
    //Add it into Vertexes in proper order.
    int i = 0;
    for (int gz = 0; gz < VERTEX_COUNT - 1; gz++)
    {
        for (int gx = 0; gx < VERTEX_COUNT - 1; gx++)
        {
            int topLeft = (gz*VERTEX_COUNT) + gx;//0//1
            int topRight = topLeft + 1;//1//2
            int bottomLeft = ((gz + 1)*VERTEX_COUNT) + gx;//128//129
            int bottomRight = bottomLeft + 1;//129//130
            
            Triangles[i++].Init(&Vertexes[topLeft],&Vertexes[bottomLeft],&Vertexes[topRight],mtl);
            Triangles[i++].Init(&Vertexes[topRight],&Vertexes[bottomLeft],&Vertexes[bottomRight],mtl);
        }
    }

}


