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
#define SIZE 2
#define MAX_DISPLACEMENT 0.2f

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
    
    //Perform smoothing.
    float n_smooth = 2.0f;
    float n_range = 1.0f;
    diamond_square(0, VERTEX_COUNT-1, 0, VERTEX_COUNT-1, (int)glm::pow(2, n_smooth), (float)n_range);

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

/* Perform the diamond square algorithm at most 2^n steps. Pass in input level: 2^n, and small number for the range. */
void ProceduralObject::diamond_square(int x1, int x2, int y1, int y2, int level, float range)
{

    //Check base case to stop recursion.
    if (level < 1)
        return;
    //Start random number generation.
    srand((unsigned int)time(NULL));
    //Diamond Algorithm
    for (int i = x1 + level; i < x2; i += level)
    {
        for (int j = y1 + level; j < y2; j += level)
        {
            //Get the 4 main vertices.
            glm::vec3 vec_a = Vertexes[(j - level)*VERTEX_COUNT + (i - level)].Position;
            glm::vec3 vec_b = Vertexes[(j - level)*VERTEX_COUNT + i].Position;
            glm::vec3 vec_c = Vertexes[(j)*VERTEX_COUNT + (i - level)].Position;
            glm::vec3 vec_d = Vertexes[(j)*VERTEX_COUNT + i].Position;
            //Get the middle vertex.
            glm::vec3 vec_e = Vertexes[(j - level / 2)*VERTEX_COUNT + (i - level / 2)].Position;
            //Get the 4 heights.
            float height_a = vec_a.y;
            float height_b = vec_b.y;
            float height_c = vec_c.y;
            float height_d = vec_d.y;
            //Calculate the average height in the middle and set it to E.
            float height_e = (float)(height_a + height_b + height_c + height_d) / 4;
            height_e += fmod(((float)(rand()) / 1000), MAX_DISPLACEMENT)*range;
            vec_e.y = height_e;
            Vertexes[(j - level / 2)*VERTEX_COUNT + (i - level / 2)].Position = vec_e;
        }
    }
    //Square algorithm
    for (int i = x1 + 2 * level; i < x2; i += level)
    {
        for (int j = y1 + 2 * level; j < y2; j += level)
        {
            //Get the 4 main vertices.
            glm::vec3 vec_a = Vertexes[(j - level)*VERTEX_COUNT + (i - level)].Position;
            glm::vec3 vec_b = Vertexes[(j - level)*VERTEX_COUNT + i].Position;
            glm::vec3 vec_c = Vertexes[(j)*VERTEX_COUNT + (i - level)].Position;
            glm::vec3 vec_d = Vertexes[(j)*VERTEX_COUNT + i].Position;
            //Get the middle vertex.
            glm::vec3 vec_e = Vertexes[(j - level / 2)*VERTEX_COUNT + (i - level / 2)].Position;
            //Get the 5 heights.
            float height_a = vec_a.y;
            float height_b = vec_b.y;
            float height_c = vec_c.y;
            float height_d = vec_d.y;
            float height_e = vec_e.y;
            //Calculate the average height and set it to F.
            glm::vec3 vec_f = Vertexes[(j - level / 2)*VERTEX_COUNT + (i - level)].Position;//
            float height_f = (float)(height_a + height_c + height_e + Vertexes[(j - level / 2)*VERTEX_COUNT + (i - 3 * level / 2)].Position.y) / 3;
            height_f += fmod(((float)(rand()) / 1000), MAX_DISPLACEMENT)*range;
            vec_f.y = height_f;
            Vertexes[(j - level / 2)*VERTEX_COUNT + (i - level)].Position = vec_f;
            //Calculate the average height and set it to G.
            glm::vec3 vec_g = Vertexes[(j - level)*VERTEX_COUNT + (i - level / 2)].Position;
            float height_g = (float)(height_a + height_b + height_e + Vertexes[(j - 3 * level / 2)*VERTEX_COUNT + (i - level / 2)].Position.y) / 3;
            height_g += fmod(((float)(rand()) / 1000), MAX_DISPLACEMENT)*range;
            vec_g.y = height_g;
            Vertexes[(j - level)*VERTEX_COUNT + (i - level / 2)].Position = vec_g;
        }
    }
    //Begin Recursion.
    diamond_square(x1, x2, y1, y2, level / 2, range / 2);
}


