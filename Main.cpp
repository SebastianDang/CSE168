////////////////////////////////////////
// Main.cpp
////////////////////////////////////////

#include "LambertMaterial.h"
#include "MeshObject.h"
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
void test();

////////////////////////////////////////////////////////////////////////////////

int main(int argc,char **argv) {
    project1();
    //test();
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
/*
void project3() {
    // Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f,0.9f,1.0f));
    
    // Create ground
    LambertMaterial groundMtl;
    groundMtl.setDiffuse(Color(0.25f,0.25f,0.25f));
    
    MeshObject ground;
    ground.MakeBox(2.0f,0.11f,2.0f,&groundMtl);
    scn.AddObject(ground);
    
    // Load dragon mesh
    MeshObject dragon;
    dragon.LoadPLY("dragon.ply");
    
    // Create box tree
    BoxTreeObject tree;
    tree.Construct(dragon);
    
    // Materials
    LambertMaterial white;
    white.setDiffuse(Color(0.7f,0.7f,0.7f));
    
    LambertMaterial red;
    red.setDiffuse(Color(0.7f,0.1f,0.1f));
    
    MetalMaterial metal;
    metal.setDiffuse(Color(0.95f,0.64f,0.54f));
    
    const int numDragons=4;
    Material *mtl[numDragons]={&white,&metal,&red,&white};
    
    // Create dragon instances
    glm::mat4 mtx;
    for(int i=0;i<numDragons;i++) {
        InstanceObject *inst=new InstanceObject(tree);
        mtx[3]=glm::vec4(0.0f,0.0f,0.3f*(float(i)/float(numDragons-1)-0.5f),1.0f);
        inst->SetMatrix(mtx);
        //inst->SetMaterial(mtl[i]);
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
    cam.SetSuperSample(10,10);
    cam.SetJitter(true);
    cam.SetShirley(true);
    
    // Render image
    cam.Render(scn);
    cam.SaveBitmap("PA3/project3.bmp");
}
*/

////////////////////////////////////////////////////////////////////////////////

void test(){
    
    // Create scene
    Scene scn;
    scn.SetSkyColor(Color(0.8f, 0.9f, 1.0f));
    
    // Create boxes
    MeshObject box1;
    box1.MakeBox(5.0f,0.1f,5.0f);
    BoxTreeObject tree1;
    tree1.Construct(box1);
    scn.AddObject(tree1);
    
    MeshObject box2;
    box2.MakeBox(1.0f,1.0f,1.0f);
    BoxTreeObject tree2;
    tree2.Construct(box2);
    
    // Create instances
    InstanceObject inst1(tree2);
    glm::mat4x4 mtx=glm::rotate(glm::mat4x4(),0.5f,glm::vec3(1,0,0));
    mtx[3][1]=1.0f;
    inst1.SetMatrix(mtx);
    scn.AddObject(inst1);
    
    InstanceObject inst2(tree2);
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
    cam.SaveBitmap("PA2/test.bmp");
}
