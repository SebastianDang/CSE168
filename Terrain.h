#pragma once
#ifndef TERRAIN_H
#define TERRAIN_H

#include <stdio.h>
#include <vector>
#include "Object.h"

class Terrain : public Object{
private:

	//Variables to keep track of information.
	std::vector<Container> containers;//[v, vn, (s,t)]
	std::vector<glm::vec3> vertices;//v
	std::vector<glm::vec3> normals;//vn
	std::vector<glm::vec2> texCoords;//(s,t)
	std::vector<unsigned int> indices;//indices.
	//Keep track of the max and min height if height map is loaded.
	float max_height;
	float min_height;
	GLuint VAO, VBO, EBO;
	//Flat terrain map.
	void setupHeightMap();
	//Functions for procedural terrain modeling.
	void setupHeightMap(const char* filename, float n_smooth, float n_range);
	float getHeightFromMap(int x, int y, unsigned char * image, int width, int height);
	float getHeightFromVertex(int x, int y);
	void diamond_square(int x1, int x2, int y1, int y2, int level, float range);
	void updateNormals();
	void updateMaxMinHeight();
	//Load and setup the textures and heightmaps.
	unsigned char * loadPPM(const char* filename, int& width, int& height);
	GLuint loadTerrain(const char* filename, int index);
	void setupTerrain(const char* terrain_0, const char* terrain_1, const char* terrain_2, const char* terrain_3, const char* blend_map);
	//Misc.
	float BaryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);
	int draw_mode;

public:
	//Constructor methods.
	Terrain(int x_d, int z_d, const char* terrain_0, const char* terrain_1, const char* terrain_2, const char* terrain_3, const char* blend_map);
	Terrain(int x_d, int z_d, const char* terrain_0, const char* terrain_1, const char* terrain_2, const char* terrain_3, const char* blend_map, const char* height_map);
	~Terrain();
	//Determine the terrain's position in the world.
	float x, z;
	glm::mat4 toWorld;
	//Draw and update methods.
	void toggleDrawMode();
	void draw(GLuint shaderProgram);
	void update();
	//Keep track of surrounding terrains to stitch them together.
	Terrain * terrain_top;
	Terrain * terrain_bottom;
	Terrain * terrain_left;
	Terrain * terrain_right;
	//Functions to stitch the edges.
	void stitch_all();
	void stitch_left();
	void stitch_right(); 
	void stitch_top();
	void stitch_bottom();
	//Functions to get the height of the terrain.
	float getHeight(glm::vec3 position);
};
#endif
