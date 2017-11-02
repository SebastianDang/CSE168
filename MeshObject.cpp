////////////////////////////////////////
// MeshObject.cpp
////////////////////////////////////////

#include "stdafx.h"
#include "MeshObject.h"
#include "LambertMaterial.h"
#include <stdio.h>
#include <string.h>
#include <glm/glm.hpp>

//Perlin Noise
struct perlin {
    int p[512];
    perlin(void);
    static perlin & getInstance(){ static perlin instance; return instance; }
};

static int permutation[] = { 151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23, 190,6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33, 88,237,149,56,87,174,20,125,136,171,168,68,175,74,165,71,134,139,48,27,166, 77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244, 102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208,89,18,169,200,196, 135,130,116,188,159,86,164,100,109,198,173,186,3,64,52,217,226,250,124,123, 5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42, 23,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,43,172,9, 129,22,39,253,19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228, 251,34,242,193,238,210,144,12,191,179,162,241,81,51,145,235,249,14,239,107, 49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127,4,150,254, 138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
};

static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }

static double lerp(double t, double a, double b) { return a + t * (b - a); }

static double grad(int hash, double x, double y, double z) {
    int h = hash & 15; // CONVERT LO 4 BITS OF HASH CODE
    double u = h<8||h==12||h==13 ? x : y; // INTO 12 GRADIENT DIRECTIONS.
    double v = h < 4||h == 12||h == 13 ? y : z;
    return ((h & 1) == 0 ? u : -u) + ((h&2) == 0 ? v : -v);
}

double noise(double x, double y, double z) {
    
    perlin & myPerlin = perlin::getInstance(); //Get Perlin instance.
    
    int X = (int)floor(x) & 255;
    int Y = (int)floor(y) & 255;
    int Z = (int)floor(z) & 255;
    
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);
    
    double u = fade(x);
    double v = fade(y);
    double w = fade(z);
    
    int A = myPerlin.p[X]+Y;
    int AA = myPerlin.p[A]+Z;
    int AB = myPerlin.p[A+1]+Z;
    int B = myPerlin.p[X+1]+Y;
    int BA = myPerlin.p[B]+Z;
    int BB = myPerlin.p[B+1]+Z;
    
    return lerp(w, lerp(v, lerp(u, grad(myPerlin.p[AA], x, y, z), grad(myPerlin.p[BA], x-1, y, z)), lerp(u, grad(myPerlin.p[AB], x, y-1, z), grad(myPerlin.p[BB], x-1, y-1, z))), lerp(v, lerp(u, grad(myPerlin.p[AA+1], x, y, z-1), grad(myPerlin.p[BA+1], x-1, y, z-1)), lerp(u, grad(myPerlin.p[AB+1], x, y-1, z-1 ), grad(myPerlin.p[BB+1], x-1, y-1, z-1 ))));
    
}

perlin::perlin (void) {
    for (int i=0; i < 256 ; i++) {
        p[256+i] = p[i] = permutation[i];
    }
}

double invsqrt(const double x)
{
    return 1.0 / std::sqrt(x);
}

////////////////////////////////////////////////////////////////////////////////

MeshObject::MeshObject() {
    NumVertexes=0;
    NumTriangles=0;
    NumMaterials=0;
    
    Vertexes=0;
    Triangles=0;
    Materials=0;
}

////////////////////////////////////////////////////////////////////////////////

MeshObject::~MeshObject() {
    delete []Vertexes;
    delete []Triangles;
    delete []Materials;
}

////////////////////////////////////////////////////////////////////////////////

bool MeshObject::Intersect(const Ray &ray, Intersection &hit) {
    bool success=false;
    for(int i=0;i<NumTriangles;i++)
        if(Triangles[i].Intersect(ray,hit)) success=true;
    return success;
}

////////////////////////////////////////////////////////////////////////////////

void MeshObject::MakeBox(float x,float y,float z,Material *mtl) {
    // Allocate arrays
    NumVertexes=24;
    NumTriangles=12;
    Vertexes=new Vertex[NumVertexes];
    Triangles=new Triangle[NumTriangles];
    if(mtl==0) mtl=new LambertMaterial;
    
    x*=0.5f;
    y*=0.5f;
    z*=0.5f;
    
    // Corners
    glm::vec3 p000(-x,-y,-z);
    glm::vec3 p001(-x,-y,z);
    glm::vec3 p010(-x,y,-z);
    glm::vec3 p011(-x,y,z);
    glm::vec3 p100(x,-y,-z);
    glm::vec3 p101(x,-y,z);
    glm::vec3 p110(x,y,-z);
    glm::vec3 p111(x,y,z);
    
    // Tex coords
    glm::vec3 t00(0.0f,0.0f,0.0f);
    glm::vec3 t01(0.0f,1.0f,0.0f);
    glm::vec3 t10(1.0f,0.0f,0.0f);
    glm::vec3 t11(1.0f,1.0f,0.0f);
    
    // Axis vectors
    glm::vec3 xAxis(1,0,0);
    glm::vec3 yAxis(0,1,0);
    glm::vec3 zAxis(0,0,1);
    
    // Right
    Vertexes[0].Set(p101,xAxis,t00);
    Vertexes[1].Set(p100,xAxis,t10);
    Vertexes[2].Set(p110,xAxis,t11);
    Vertexes[3].Set(p111,xAxis,t01);
    Triangles[0].Init(&Vertexes[0],&Vertexes[1],&Vertexes[2],mtl);
    Triangles[1].Init(&Vertexes[0],&Vertexes[2],&Vertexes[3],mtl);
    
    // Left
    Vertexes[4].Set(p000,-xAxis,t00);
    Vertexes[5].Set(p001,-xAxis,t10);
    Vertexes[6].Set(p011,-xAxis,t11);
    Vertexes[7].Set(p010,-xAxis,t01);
    Triangles[2].Init(&Vertexes[4],&Vertexes[5],&Vertexes[6],mtl);
    Triangles[3].Init(&Vertexes[4],&Vertexes[6],&Vertexes[7],mtl);
    
    // Top
    Vertexes[8].Set(p011,yAxis,t00);
    Vertexes[9].Set(p111,yAxis,t10);
    Vertexes[10].Set(p110,yAxis,t11);
    Vertexes[11].Set(p010,yAxis,t01);
    Triangles[4].Init(&Vertexes[8],&Vertexes[9],&Vertexes[10],mtl);
    Triangles[5].Init(&Vertexes[8],&Vertexes[10],&Vertexes[11],mtl);
    
    // Bottom
    Vertexes[12].Set(p000,-yAxis,t00);
    Vertexes[13].Set(p100,-yAxis,t10);
    Vertexes[14].Set(p101,-yAxis,t11);
    Vertexes[15].Set(p001,-yAxis,t01);
    Triangles[6].Init(&Vertexes[12],&Vertexes[13],&Vertexes[14],mtl);
    Triangles[7].Init(&Vertexes[12],&Vertexes[14],&Vertexes[15],mtl);
    
    // Front
    Vertexes[16].Set(p001,zAxis,t00);
    Vertexes[17].Set(p101,zAxis,t10);
    Vertexes[18].Set(p111,zAxis,t11);
    Vertexes[19].Set(p011,zAxis,t01);
    Triangles[8].Init(&Vertexes[16],&Vertexes[17],&Vertexes[18],mtl);
    Triangles[9].Init(&Vertexes[16],&Vertexes[18],&Vertexes[19],mtl);
    
    // Back
    Vertexes[20].Set(p100,-zAxis,t00);
    Vertexes[21].Set(p000,-zAxis,t10);
    Vertexes[22].Set(p010,-zAxis,t11);
    Vertexes[23].Set(p110,-zAxis,t01);
    Triangles[10].Init(&Vertexes[20],&Vertexes[21],&Vertexes[22],mtl);
    Triangles[11].Init(&Vertexes[20],&Vertexes[22],&Vertexes[23],mtl);
}

////////////////////////////////////////////////////////////////////////////////

bool MeshObject::LoadPLY(const char *filename,Material *mtl) {
    
    //Open file
    FILE *f=fopen(filename,"r");
    if(f==0) {
        printf("ERROR: MeshObject::LoadPLY()- Can't open '%s'\n",filename);
        return false;
    }
    
    //Read header
    char tmp[256];
    int numverts=0,numtris=0;
    int posprop=-99,normprop=-99;
    int props=0;
    while(1) {
        fgets(tmp,256,f);
        if(strncmp(tmp,"element vertex",14)==0)
            numverts=atoi(&tmp[14]);
        if(strncmp(tmp,"element face",12)==0)
            numtris=atoi(&tmp[12]);
        if(strncmp(tmp,"property",8)==0) {
            int len=(int)strlen(tmp);
            if(strncmp(&tmp[len-3]," x",2)==0) posprop=props;
            if(strncmp(&tmp[len-3],"nx",2)==0) normprop=props;
            props++;
        }
        if(strcmp(tmp,"end_header\n")==0) break;
    }
    if(posprop==-1) {
        printf("ERROR: MeshObject::LoadPLY()- No vertex positions found\n");
        fclose(f);
        return false;
    }
    
    //Read verts
    int i=0;
    if(numverts>0) {
        NumVertexes=numverts;
        Vertexes=new Vertex[NumVertexes];
        
        for(i=0;i<NumVertexes;i++) {
            fgets(tmp,256,f);
            char *pch=strtok(tmp," ");
            int prop=0;
            while(pch) {
                if(prop==posprop) Vertexes[i].Position.x=float(atof(pch));
                if(prop==posprop+1) Vertexes[i].Position.y=float(atof(pch));
                if(prop==posprop+2) Vertexes[i].Position.z=float(atof(pch));
                if(prop==normprop) Vertexes[i].Normal.x=float(atof(pch));
                if(prop==normprop+1) Vertexes[i].Normal.y=float(atof(pch));
                if(prop==normprop+2) Vertexes[i].Normal.z=float(atof(pch));
                pch=strtok(0," ");
                prop++;
            }
        }
    }
    
    //Read tris
    if(numtris>0) {
        if(mtl==0) mtl=new LambertMaterial;
        NumTriangles=numtris;
        Triangles=new Triangle[numtris];
        for(i=0;i<numtris;i++) {
            int count,i0,i1,i2;
            fscanf(f,"%d %d %d %d\n",&count,&i0,&i1,&i2);
            if(count!=3) {
                printf("ERROR: MeshObject::LoadPLY()- Only triangles are supported\n");
                fclose(f);
                return false;
            }
            Triangles[i].Init(&Vertexes[i0],&Vertexes[i1],&Vertexes[i2],mtl);
        }
    }
    
    //Smooth
    if(normprop<0) Smooth();
    
    //Close file
    fclose(f);
    printf("Loaded %d triangles from file '%s'\n",numtris,filename);
    return true;
}

////////////////////////////////////////////////////////////////////////////////

void MeshObject::Smooth() {
    int i,j;
    for(i=0;i<NumVertexes;i++)
        Vertexes[i].Normal=glm::vec3(0);
    for(i=0;i<NumTriangles;i++) {
        Triangle &tri=Triangles[i];
        glm::vec3 e1=tri.GetVtx(1).Position-tri.GetVtx(0).Position;
        glm::vec3 e2=tri.GetVtx(2).Position-tri.GetVtx(0).Position;
        glm::vec3 cross=glm::cross(e1,e2);
        for(j=0;j<3;j++)
            tri.GetVtx(j).Normal+=cross;
    }
    for(i=0;i<NumVertexes;i++)
        Vertexes[i].Normal=glm::normalize(Vertexes[i].Normal);
}

////////////////////////////////////////////////////////////////////////////////

void MeshObject::AddDisplacement() {
    
    for (int i=0;i<NumVertexes;i++){
        
        //Displace the point.
        Vertexes[i].Position += ((rand() % 100) / 100.0f) * Vertexes[i].Normal;
        
    }
    
}

////////////////////////////////////////////////////////////////////////////////

void MeshObject::AddNormalMap(){
    
    for (int i=0;i<NumVertexes;i++){
        
        //Get the new point.
        glm::vec3 point = Vertexes[i].Position;
        
        //Generate noise from perlin noise.
        float noise_x = float(noise(0.1*double(point.x), 0.1*double(point.y), 0.1*double(point.z)));
        float noise_y = float(noise(0.1*double(point.y), 0.1* double(point.z), 0.1*double(point.x)));
        float noise_z = float(noise(0.1*double(point.z), 0.1*double(point.x), 0.1*double(point.y)));
        
        //Get the normal.
        glm::vec3 normal = Vertexes[i].Normal;
        
        //Generate a random procedure for bump map.
        float bump = (rand()%100) / 100.0f;
        
        //Normal mapping.
        normal.x = (1.0f - bump) * normal.x + bump * noise_x;
        normal.y = (1.0f - bump) * normal.y + bump * noise_y;
        normal.z = (1.0f - bump) * normal.z + bump * noise_z;
        
        //Transform the original normal.
        float temp = glm::length(normal*normal);
        if (temp == 0.0f)
            break;
        temp = (float)invsqrt(temp);
        
        Vertexes[i].Normal = temp * normal;
    }

}

////////////////////////////////////////////////////////////////////////////////

Triangle** MeshObject::getTriangles(){
    Triangle** tris = new Triangle*[NumTriangles];
    for (int i = 0; i < NumTriangles; i++) {
        tris[i] = &Triangles[i];
    }
    return tris;
}

////////////////////////////////////////////////////////////////////////////////
