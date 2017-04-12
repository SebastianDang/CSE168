////////////////////////////////////////
// Main.cpp
////////////////////////////////////////

#include "MeshObject.h"
#include "InstanceObject.h"
#include "Camera.h"
#include "PointLight.h"
#include "DirectLight.h"
#include "Scene.h"
#include "glm/gtc/matrix_transform.hpp"

void project1();
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
	cam.SaveBitmap("project1.bmp");
}

////////////////////////////////////////////////////////////////////////////////

void test(){
    
    // Set up scene
    Scene scn;
    scn.SetSkyColor(Color(0.8, 0.9, 1.0));
    
    // Create box
    MeshObject box;
    box.MakeBox(0.5, 0.2, 0.3);
    scn.AddObject(box);
    
    // Create light
    PointLight redlgt;
    redlgt.SetBaseColor(Color(1.0, 0.2, 0.2));
    redlgt.SetIntensity(10.0);
    redlgt.SetPosition(glm::vec3(1.0, 2.0, 1.5));
    scn.AddLight(redlgt);
    
    // Create camera
    Camera cam;
    cam.LookAt(glm::vec3(0.0f,0.0f,5.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
    cam.SetFOV(50.0); // NOTE: this is in degrees for UI purposes. Internally, it should be stored as radians
    cam.SetAspect(1.33);
    cam.SetResolution(800,600);
    // Render image
    cam.Render(scn);
    cam.SaveBitmap("test.bmp");
}
