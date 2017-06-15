////////////////////////////////////////
// Main.cpp
////////////////////////////////////////

#include "LambertMaterial.h"
#include "MetalMaterial.h"
#include "AshikhminMaterial.h"
#include "MeshObject.h"
#include "ProceduralObject.h"
#include "BoxTreeObject.h"
#include "InstanceObject.h"
#include "Camera.h"
#include "PointLight.h"
#include "DirectLight.h"
#include "Scene.h"
#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"

void project1();
void project2();
void project3();
void project4();
void project5();
void test();

////////////////////////////////////////////////////////////////////////////////

int main(int argc,char **argv) {
    project5();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////

void project1() {
    // Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));
    
    // Create boxes
    MeshObject box1;
    box1.MakeBox(5.0f,0.1f,5.0f);
    scn.AddObject(box1);
    
    MeshObject box2;
    box2.MakeBox(1.0f,1.0f,1.0f);
    
    // Create instances
    InstanceObject inst1(box2);
    glm::mat4x4 mtx=glm::rotate(glm::mat4x4(),0.5f,glm::vec3(1,0,0));
    mtx[3][1]=1.0f;
    inst1.SetMatrix(mtx);
    scn.AddObject(inst1);
    
    InstanceObject inst2(box2);
    mtx=glm::rotate(glm::mat4x4(),1.0f,glm::vec3(0,1,0));
    mtx[3]=glm::vec4(-1,0,1,1);
    inst2.SetMatrix(mtx);
    scn.AddObject(inst2);
    
    // Create lights
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
    sunlgt.SetIntensity(0.5f);
    sunlgt.SetDirection(glm::vec3(-0.5f, -1.0f, -0.5f));
    scn.AddLight(sunlgt);
    
    PointLight redlgt;
    redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
    redlgt.SetIntensity(2.0f);
    redlgt.SetPosition(glm::vec3(2.0f, 2.0f, 0.0f));
    scn.AddLight(redlgt);
    
    // Create camera
    Camera cam;
    cam.LookAt(glm::vec3(2.0f,2.0f,5.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0,1,0));
    cam.SetResolution(800,600);
    cam.SetFOV(40.0f);
    cam.SetAspect(1.33f);
    
    // Render image
    cam.Render(scn);
    cam.SaveBitmap("PA1/project1.bmp");
}

////////////////////////////////////////////////////////////////////////////////

void project2(){
    // Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.8f, 1.0f));
    
    // Create ground
    MeshObject ground;
    ground.MakeBox(5.0f,0.1f,5.0f);
    scn.AddObject(ground);
    
    // Create dragon
    MeshObject dragon;
    dragon.LoadPLY("Assets/dragon.ply");
    dragon.Smooth();
    
    BoxTreeObject tree;
    tree.Construct(dragon);
    scn.AddObject(tree);
    
    // Create instance
    InstanceObject inst(tree);
    glm::mat4x4 mtx=glm::eulerAngleY(M_PI);
    mtx[3]=glm::vec4(-0.05f,0.0f,-0.1f,1.0f);
    inst.SetMatrix(mtx);
    scn.AddObject(inst);
    
    // Create lights
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
    sunlgt.SetIntensity(1.0f);
    sunlgt.SetDirection(glm::vec3(2.0f, -3.0f, -2.0f));
    scn.AddLight(sunlgt);
    
    PointLight redlgt;
    redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
    redlgt.SetIntensity(0.02f);
    redlgt.SetPosition(glm::vec3(-0.2f, 0.2f, 0.2f));
    scn.AddLight(redlgt);
    
    PointLight bluelgt;
    bluelgt.SetBaseColor(Color(0.2f, 0.2f, 1.0f));
    bluelgt.SetIntensity(0.02f);
    bluelgt.SetPosition(glm::vec3(0.1f, 0.1f, 0.3f));
    scn.AddLight(bluelgt);
    
    // Create camera
    Camera cam;
    cam.LookAt(glm::vec3(-0.1f,0.1f,0.2f),glm::vec3(-0.05f,0.12f,0.0f), glm::vec3(0,1.0f,0));
    cam.SetFOV(40.0f);
    cam.SetAspect(1.33f);
    cam.SetResolution(800,600);
    
    // Render image
    cam.Render(scn);
    cam.SaveBitmap("PA2/project2.bmp");
}

////////////////////////////////////////////////////////////////////////////////

void project3() {
    // Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f,0.9f,1.0f));
    
    // Create ground
    LambertMaterial groundMtl;
    groundMtl.SetDiffuse(Color(0.25f,0.25f,0.25f));
    
    MeshObject ground;
    ground.MakeBox(2.0f,0.11f,2.0f,&groundMtl);
    scn.AddObject(ground);
    
    // Load dragon mesh
    MeshObject dragon;
    dragon.LoadPLY("Assets/dragon.ply");
    
    // Create box tree
    BoxTreeObject tree;
    tree.Construct(dragon);
    
    // Materials
    LambertMaterial white;
    white.SetDiffuse(Color(0.7f,0.7f,0.7f));
    
    LambertMaterial red;
    red.SetDiffuse(Color(0.7f,0.1f,0.1f));
    
    MetalMaterial metal;
    metal.SetDiffuse(Color(0.95f,0.64f,0.54f));
    
    const int numDragons=4;
    Material *mtl[numDragons]={&white,&metal,&red,&white};
    
    // Create dragon instances
    glm::mat4 mtx;
    for(int i=0;i<numDragons;i++) {
        InstanceObject *inst=new InstanceObject(tree);
        mtx[3]=glm::vec4(0.0f,0.0f,0.3f*(float(i)/float(numDragons-1)-0.5f),1.0f);
        inst->SetMatrix(mtx);
        inst->SetMaterial(*mtl[i]);
        scn.AddObject(*inst);
    }
    
    
    // Create lights
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
    sunlgt.SetIntensity(1.0f);
    sunlgt.SetDirection(glm::vec3(2.0f, -3.0f, -2.0f));
    scn.AddLight(sunlgt);
    
    // Create camera
    Camera cam;
    cam.SetResolution(640,480);
    cam.SetAspect(1.33f);
    cam.LookAt(glm::vec3(-0.5f,0.25f,-0.2f),glm::vec3(0.0f,0.15f,0.0f), glm::vec3(0,1,0));
    cam.SetFOV(40.0f);
    cam.SetSuperSample(2,2);
    cam.SetJitter(true);
    cam.SetShirley(true);
    
    struct timespec start, finish;
    double elapsed;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("Start Render\n");
    
    // Render image
    cam.Render(scn);
    
    clock_gettime(CLOCK_MONOTONIC, &finish);
    printf("Finish Render\n");
    
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("elapsed: %f\n", elapsed);
    
    //Save
    cam.SaveBitmap("PA3/project3.bmp");
}

////////////////////////////////////////////////////////////////////////////////

void project4(){
    
    // Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));
    
    // Create ground
    LambertMaterial groundMtl;
    groundMtl.SetDiffuse(Color(0.3f,0.3f,0.35f));
    
    MeshObject ground;
    ground.MakeBox(2.0f,0.11f,2.0f,&groundMtl);
    scn.AddObject(ground);
    
    // Load dragon mesh
    MeshObject dragon;
    dragon.LoadPLY("Assets/dragon.ply");
    
    // Create box tree
    BoxTreeObject tree;
    tree.Construct(dragon);
    
    // Materials
    const int nummtls=4;
    AshikhminMaterial mtl[nummtls];
    
    // Diffuse
    mtl[0].SetSpecularLevel(0.0f);
    mtl[0].SetDiffuseLevel(1.0f);
    mtl[0].SetDiffuse(Color(0.7f,0.7f,0.7f));
    // Roughened copper
    mtl[1].SetDiffuseLevel(0.0f);
    mtl[1].SetSpecularLevel(1.0f);
    mtl[1].SetSpecular(Color(0.9f,0.6f,0.5f));
    mtl[1].SetRoughness(100.0f,100.0f);
    // Anisotropic gold
    mtl[2].SetDiffuseLevel(0.0f);
    mtl[2].SetSpecularLevel(1.0f);
    mtl[2].SetSpecular(Color(0.95f,0.7f,0.3f));
    mtl[2].SetRoughness(1.0f,1000.0f);
    // Red plastic
    mtl[3].SetDiffuse(Color(1.0f,0.1f,0.1f));
    mtl[3].SetDiffuseLevel(0.8f);
    mtl[3].SetSpecularLevel(0.2f);
    mtl[3].SetSpecular(Color(1.0f,1.0f,1.0f));
    mtl[3].SetRoughness(1000.0f,1000.0f);
    
    // Create dragon instances
    glm::mat4 mtx = glm::mat4(1.0f);
    for(int i=0;i<nummtls;i++) {
        InstanceObject *inst=new InstanceObject(tree);
        mtx[3]=glm::vec4(0.0f,0.0f,-0.1f*float(i),1.0f);
        inst->SetMatrix(mtx);
        inst->SetMaterial(mtl[i]);
        scn.AddObject(*inst);
    }
    
    // Create lights
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f));
    sunlgt.SetIntensity(1.0f);
    sunlgt.SetDirection(glm::vec3 (2.0f, -3.0f, -2.0f));
    scn.AddLight(sunlgt);
    
    // Create camera
    Camera cam;
    cam.SetResolution(800,600);
    cam.SetAspect(1.33f);
    cam.LookAt(glm::vec3(-0.5f,0.25f,-0.2f), glm::vec3(0.0f,0.15f,-0.15f), glm::vec3(0,1,0));
    cam.SetFOV(40.0f);
    cam.SetSuperSample(2,2);
    cam.SetJitter(true);
    cam.SetShirley(true);
    
    //Time
    struct timespec start, finish;
    double elapsed;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("Start Render\n");
    
    // Render image
    cam.Render(scn);
    
    clock_gettime(CLOCK_MONOTONIC, &finish);
    printf("Finish Render\n");
    
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("elapsed: %f\n", elapsed);
    
    //Save
    cam.SaveBitmap("PA4/project4.bmp");
}


////////////////////////////////////////////////////////////////////////////////

void project5(){
    
    //----- Setup Lighting and the Environment -----//
    
    //Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));
    scn.SetGradientColor(Color(0.90f, 0.3f, 0.3f));
    
    //Direct light
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(0.5f, 1.0f, 0.9f));
    sunlgt.SetIntensity(1.0f);
    sunlgt.SetDirection(glm::vec3 (2.0f, -3.0f, -2.0f));
    scn.AddLight(sunlgt);
    
    //Point light
    PointLight pntlgt;
    pntlgt.SetBaseColor(Color(1.0f, 0.0f, 0.0f));
    pntlgt.SetIntensity(10.0f);
    pntlgt.SetPosition(glm::vec3(6.0f, 4.0f, -4.0f));
    scn.AddLight(pntlgt);

    
    //----- Setup Camera -----//
    
    Camera cam;
    cam.SetFOV(40.0f);
    cam.SetAspect(1.33f);
    cam.SetResolution(800,600);
    cam.LookAt(glm::vec3(2.0f, 5.0f, 10.0f), glm::vec3(2.0f, 1.0f, 0.0f), glm::vec3(0,1,0));
    cam.SetSuperSample(2,2);
    cam.SetJitter(true);
    cam.SetShirley(true);
    
    //----- Procedural Object -----//
    
    //Generate the Object.
    ProceduralObject proc;
    proc.Generate(4.0f);
    
    //Create the Boxtree Object.
    BoxTreeObject proc_tree;
    proc_tree.Construct(proc);
    
    //Create the Material.
    AshikhminMaterial proc_mtl;
    proc_mtl.SetDiffuseLevel(0.0f);
    proc_mtl.SetSpecularLevel(1.0f);
    proc_mtl.SetDiffuse(Color(0.9f, 0.6f, 0.5f));
    proc_mtl.SetSpecular(Color(0.9f,0.6f,0.5f));
    proc_mtl.SetRoughness(100.0f,100.0f);
    
    
    //Instance 1.
    InstanceObject proc_inst1(proc_tree);
    proc_inst1.SetMaterial(proc_mtl);
    scn.AddObject(proc_inst1);

    //----- Other Objects -----//
    
    //Sphere Mesh
    MeshObject sphere;
    sphere.LoadPLY("Assets/ball.ply");
    
    BoxTreeObject sphere_tree;
    sphere_tree.Construct(sphere);
    
    LambertMaterial sphere_mtl;
    sphere_mtl.SetDiffuse(Color(1.0f,0.3f,0.35f));
    
    InstanceObject sphere_inst1(sphere_tree);
    glm::mat4x4 mtx = glm::scale(glm::mat4x4(), glm::vec3(0.01f,0.01f,0.01f));
    mtx = glm::scale(glm::mat4x4(), glm::vec3(5.0f, 5.0f, 5.0f)) * mtx;
    mtx = glm::translate(glm::mat4x4(), glm::vec3(6.0f, 4.0f, -4.0f)) * mtx;
    sphere_inst1.SetMatrix(mtx);
    sphere_inst1.SetMaterial(sphere_mtl);
    scn.AddObject(sphere_inst1);
    

    //----- Render -----//

    //Time
    struct timespec start, finish;
    double elapsed;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("Start Render\n");
    
    //Render image
    cam.Render(scn);
    
    clock_gettime(CLOCK_MONOTONIC, &finish);
    printf("Finish Render\n");
    
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("elapsed: %f\n", elapsed);
    
    //Save to file.
    cam.SaveBitmap("PA5/project5.bmp");
    
}

////////////////////////////////////////////////////////////////////////////////

void test(){
    
    //Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));
    
    // Material of the Procedural Object.
    AshikhminMaterial mtl_1;
    mtl_1.SetDiffuseLevel(0.2f);
    mtl_1.SetSpecularLevel(0.8f);
    mtl_1.SetDiffuse(Color(0.5f, 0.5f, 0.5f));
    mtl_1.SetSpecular(Color(0.95f,0.7f,0.3f));
    mtl_1.SetRoughness(1.0f,100000.0f);
    
    //Create Object.
    ProceduralObject proc;
    proc.Generate(4.0f);
    
    BoxTreeObject proc_tree;
    proc_tree.Construct(proc);
    
    InstanceObject inst1(proc_tree);
    inst1.SetMaterial(mtl_1);
    scn.AddObject(inst1);
    
    InstanceObject inst2(proc_tree);
    glm::mat4 toWorld2 = glm::mat4(1.0f);
    float angle = 90.0f * (M_PI/180.f);
    toWorld2 = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0,0,1)) * toWorld2;
    inst2.SetMatrix(toWorld2);
    inst2.SetMaterial(mtl_1);
    scn.AddObject(inst2);
    
    InstanceObject inst3(proc_tree);
    glm::mat4 toWorld3 = glm::mat4(1.0f);
    float angle3 = 90.0f * (M_PI/180.f);
    toWorld3 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle3, glm::vec3(0,0,-1)) * toWorld3;
    inst3.SetMatrix(toWorld3);
    inst3.SetMaterial(mtl_1);
    scn.AddObject(inst3);
    
    InstanceObject inst4(proc_tree);
    glm::mat4 toWorld4 = glm::mat4(1.0f);
    float angle4 = 90.0f * (M_PI/180.f);
    toWorld4 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), angle4, glm::vec3(1,0,0)) * toWorld4;
    inst4.SetMatrix(toWorld4);
    inst4.SetMaterial(mtl_1);
    scn.AddObject(inst4);
    
    //Create light
    DirectLight sunlgt;
    sunlgt.SetBaseColor(Color(0.5f, 1.0f, 0.9f));
    sunlgt.SetIntensity(1.0f);
    sunlgt.SetDirection(glm::vec3 (2.0f, -3.0f, -2.0f));
    scn.AddLight(sunlgt);
    
    //Point light
    PointLight redlgt;
    redlgt.SetBaseColor(Color(1.0f, 1.0f, 1.0f));
    redlgt.SetIntensity(60.0f);
    redlgt.SetPosition(glm::vec3(2.0f, 2.0f, 2.0f));
    scn.AddLight(redlgt);
    
    //Create camera
    Camera cam;
    cam.SetResolution(800,600);
    cam.SetFOV(40.0f);
    cam.SetAspect(1.33f);
    cam.LookAt(glm::vec3(2.0f, 5.0f, 10.0f), glm::vec3(2.0f, 1.0f, 0.0f), glm::vec3(0,1,0));
    cam.SetSuperSample(2,2);
    cam.SetJitter(true);
    cam.SetShirley(true);
    
    //Time
    struct timespec start, finish;
    double elapsed;
    
    clock_gettime(CLOCK_MONOTONIC, &start);
    printf("Start Render\n");
    
    // Render image
    cam.Render(scn);
    
    clock_gettime(CLOCK_MONOTONIC, &finish);
    printf("Finish Render\n");
    
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("elapsed: %f\n", elapsed);
    
    //Save
    cam.SaveBitmap("PA5/test.bmp");
}
