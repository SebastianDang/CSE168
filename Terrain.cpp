#include "Terrain.h"
#include <math.h>

using namespace std;

#define SIZE 500
#define VERTEX_COUNT 128
#define MAX_HEIGHT 40
#define MAX_DISPLACEMENT 0.01f

/* Flat Terrain. Ability to input a height map: either real or generated from different applications. Shader that adds at least 3 different type of terrain(grass, desert, snow). */
Terrain::Terrain(int x_d, int z_d, const char* terrain_0, const char* terrain_1, const char* terrain_2, const char* terrain_3, const char* blend_map)
{
	//Setup the terrain.
	this->x = x_d * SIZE;
	this->z = z_d * SIZE;

	//Setup HeightMap
	this->setupHeightMap();
    //this->setupHeightMap(height_map, 16.0f, 4.0f);
    
}

/* Deconstructor to safely delete when finished. */
Terrain::~Terrain()
{

}

/* Setup a default flat terrain. */
void Terrain::setupHeightMap()
{
	//Create the height map: v, vn, texCoords
	//Generate vertices, normals, and texCoords for a terrain map. vertex = (j, i).
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
			//Push back to vectors.
			vertices.push_back(glm::vec3(vertex_x, vertex_y, vertex_z));
			normals.push_back(glm::vec3(normal_x, normal_y, normal_z));
			texCoords.push_back(glm::vec2(texCoord_x, texCoord_y));
		}
	}
	//Setup the indices to draw based on indice points.
	for (int gz = 0; gz < VERTEX_COUNT - 1; gz++)
	{
		for (int gx = 0; gx < VERTEX_COUNT - 1; gx++)
		{
			int topLeft = (gz*VERTEX_COUNT) + gx;//0//1
			int topRight = topLeft + 1;//1//2
			int bottomLeft = ((gz + 1)*VERTEX_COUNT) + gx;//128//129
			int bottomRight = bottomLeft + 1;//129//130
			//Push back to indices.
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}
	//Add into container structs for rendering.
	for (int i = 0; i < VERTEX_COUNT * VERTEX_COUNT; i++)
	{
		//Throw everything into a container to hold all values.
		Container container;
		container.vertex = vertices[i];
		container.normal = normals[i];
		container.texCoord = texCoords[i];
		containers.push_back(container);
	}
}

/* Setup the terrain based on loaded height map. */
void Terrain::setupHeightMap(const char* filename, float n_smooth, float n_range)
{
	//Define variables to hold height map's width, height, pixel information.
	int width, height;
	unsigned char * image;
	//Generate the texture.
	image = loadPPM(filename, width, height);//Load the ppm file.
	//Create the height map: v, vn, texCoords
	//Generate vertices, normals, and texCoords for a terrain map.
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		for (int j = 0; j < VERTEX_COUNT; j++)
		{
			//Setup the vertices.
			float vertex_x = (float)j / ((float)VERTEX_COUNT - 1) * SIZE;
			float vertex_y = getHeightFromMap(j, i, image, width, height);
			float vertex_z = (float)i / ((float)VERTEX_COUNT - 1) * SIZE;
			//Setup the normals.
			float normal_x = 0;
			float normal_y = 1.0f;
			float normal_z = 0;
			//Setup the texcoords.
			float texCoord_x = (float)j / ((float)VERTEX_COUNT - 1);
			float texCoord_y = (float)i / ((float)VERTEX_COUNT - 1);
			//Push back to vectors.
			vertices.push_back(glm::vec3(vertex_x, vertex_y, vertex_z));
			normals.push_back(glm::vec3(normal_x, normal_y, normal_z));
			texCoords.push_back(glm::vec2(texCoord_x, texCoord_y));
		}
	}
	//Setup the indices to draw based on indice points.
	for (int gz = 0; gz < VERTEX_COUNT - 1; gz++)
	{
		for (int gx = 0; gx < VERTEX_COUNT - 1; gx++)
		{
			int topLeft = (gz*VERTEX_COUNT) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1)*VERTEX_COUNT) + gx;
			int bottomRight = bottomLeft + 1;
			//Push back to indices.
			indices.push_back(topLeft);
			indices.push_back(bottomLeft);
			indices.push_back(topRight);
			indices.push_back(topRight);
			indices.push_back(bottomLeft);
			indices.push_back(bottomRight);
		}
	}
	//Add into container structs for rendering.
	for (int i = 0; i < VERTEX_COUNT * VERTEX_COUNT; i++)
	{
		//Throw everything into a container to hold all values.
		Container container;
		container.vertex = vertices[i];
		container.normal = normals[i];
		container.texCoord = texCoords[i];
		containers.push_back(container);
	}
	//Perform smoothing.
	diamond_square(0, VERTEX_COUNT-1, 0, VERTEX_COUNT-1, (int)glm::pow(2, n_smooth), (float)n_range);
	//Update normals and calculate max/min height.
	updateNormals();
	updateMaxMinHeight();
}

/* Returns the RGB value of the position (height). */
float Terrain::getHeightFromMap(int x, int y, unsigned char * image, int width, int height)
{
	if (x < 0 || (3*x) >= (width) || y < 0 || y >= (height))
	{
		return 0;
	}
	int index = (3*x) + (width*y);
	float value = (float)image[index]+(float)image[index + 1]+(float)image[index + 2];
	float result = (float)fmod(value, (float)MAX_HEIGHT);
	return result;
}

/* Return the height at a given x, y coordinate. */
float Terrain::getHeightFromVertex(int x, int y)
{
	if (x < 0 || (x) >= VERTEX_COUNT || y < 0 || y >= VERTEX_COUNT)
	{
		return 0;
	}
	return this->vertices[(y*VERTEX_COUNT) + x].y;
}

/* Perform the diamond square algorithm at most 2^n steps. Pass in input level: 2^n, and small number for the range. */
void Terrain::diamond_square(int x1, int x2, int y1, int y2, int level, float range)
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
			glm::vec3 vec_a = this->vertices[(j - level)*VERTEX_COUNT + (i - level)];
			glm::vec3 vec_b = this->vertices[(j - level)*VERTEX_COUNT + i];
			glm::vec3 vec_c = this->vertices[(j)*VERTEX_COUNT + (i - level)];
			glm::vec3 vec_d = this->vertices[(j)*VERTEX_COUNT + i];
			//Get the middle vertex.
			glm::vec3 vec_e = this->vertices[(j - level / 2)*VERTEX_COUNT + (i - level / 2)];
			//Get the 4 heights.
			float height_a = vec_a.y;
			float height_b = vec_b.y;
			float height_c = vec_c.y;
			float height_d = vec_d.y;
			//Calculate the average height in the middle and set it to E.
			float height_e = (float)(height_a + height_b + height_c + height_d) / 4;
			height_e += fmod(((float)(rand()) / 1000), MAX_DISPLACEMENT)*range;
			vec_e.y = height_e;
			this->vertices[(j - level / 2)*VERTEX_COUNT + (i - level / 2)] = vec_e;
			this->containers[(j - level / 2)*VERTEX_COUNT + (i - level / 2)].vertex = vec_e;
		}
	}
	//Square algorithm
	for (int i = x1 + 2 * level; i < x2; i += level)
	{
		for (int j = y1 + 2 * level; j < y2; j += level)
		{
			//Get the 4 main vertices.
			glm::vec3 vec_a = this->vertices[(j - level)*VERTEX_COUNT + (i - level)];
			glm::vec3 vec_b = this->vertices[(j - level)*VERTEX_COUNT + i];
			glm::vec3 vec_c = this->vertices[(j)*VERTEX_COUNT + (i - level)];
			glm::vec3 vec_d = this->vertices[(j)*VERTEX_COUNT + i];
			//Get the middle vertex.
			glm::vec3 vec_e = this->vertices[(j - level / 2)*VERTEX_COUNT + (i - level / 2)];
			//Get the 5 heights.
			float height_a = vec_a.y;
			float height_b = vec_b.y;
			float height_c = vec_c.y;
			float height_d = vec_d.y;
			float height_e = vec_e.y;
			//Calculate the average height and set it to F.
			glm::vec3 vec_f = this->vertices[(j - level / 2)*VERTEX_COUNT + (i - level)];//
			float height_f = (float)(height_a + height_c + height_e + this->vertices[(j - level / 2)*VERTEX_COUNT + (i - 3 * level / 2)].y) / 3;
			height_f += fmod(((float)(rand()) / 1000), MAX_DISPLACEMENT)*range;
			vec_f.y = height_f;
			this->vertices[(j - level / 2)*VERTEX_COUNT + (i - level)] = vec_f;
			this->containers[(j - level / 2)*VERTEX_COUNT + (i - level)].vertex = vec_f;
			//Calculate the average height and set it to G.
			glm::vec3 vec_g = this->vertices[(j - level)*VERTEX_COUNT + (i - level / 2)];
			float height_g = (float)(height_a + height_b + height_e + this->vertices[(j - 3 * level / 2)*VERTEX_COUNT + (i - level / 2)].y) / 3;
			height_g += fmod(((float)(rand()) / 1000), MAX_DISPLACEMENT)*range;
			vec_g.y = height_g;
			this->vertices[(j - level)*VERTEX_COUNT + (i - level / 2)] = vec_g;
			this->containers[(j - level)*VERTEX_COUNT + (i - level / 2)].vertex = vec_g;
		}
	}
	//Begin Recursion.
	diamond_square(x1, x2, y1, y2, level / 2, range / 2);
}

/* Updates the normals for the entire terrain. */
void Terrain::updateNormals()
{
	for (int i = 0; i < VERTEX_COUNT; i++)
	{
		for (int j = 0; j < VERTEX_COUNT; j++)
		{
			//Get the proper heights.
			float heightL = getHeightFromVertex(j - 1, i);
			float heightR = getHeightFromVertex(j + 1, i);
			float heightD = getHeightFromVertex(j, i + 1);
			float heightU = getHeightFromVertex(j, i - 1);
			//Check if we're at the left edge.
			if (j == 0 && (terrain_left != nullptr)) {
				heightL = terrain_left->getHeightFromVertex((VERTEX_COUNT - 1), i);
			}
			//Check if we're at the right edge.
			if (j == (VERTEX_COUNT - 1) && (terrain_right != nullptr)) {
				heightR = terrain_right->getHeightFromVertex(0, i);
			}
			//Check if we're at the top edge.
			if (i == 0  && (terrain_top != nullptr)) {
				heightU = terrain_top->getHeightFromVertex(j, VERTEX_COUNT-1);
			}
			//Check if we're at the bottom edge.
			if (i == (VERTEX_COUNT - 1) && (terrain_bottom != nullptr)) {
				heightD = terrain_bottom->getHeightFromVertex(j, 0);
			}
			//Update the normal.
			glm::vec3 normal = glm::normalize(glm::vec3(heightL - heightR, 2.0f, heightU - heightD));
			this->normals[(i*VERTEX_COUNT) + j] = normal;
			this->containers[(i*VERTEX_COUNT) + j].normal = normal;
		}
	}
}

/* Updates and finds the max and min height of the terrain. */
void Terrain::updateMaxMinHeight()
{
	float max = -INFINITY, min = INFINITY;
	for (int i = 0; i < vertices.size(); i++)
	{
		float cur_height = vertices[i].y;
		if (cur_height > max)
		{
			max = cur_height;
		}
		if (cur_height < min)
		{
			min = cur_height;
		}
	}
	//Set the max and min heights found.
	this->max_height = max;
	this->min_height = min;
}

/** Load a ppm file from disk.
@input filename The location of the PPM file.  If the file is not found, an error message
will be printed and this function will return 0
@input width This will be modified to contain the width of the loaded image, or 0 if file not found
@input height This will be modified to contain the height of the loaded image, or 0 if file not found
@return Returns the RGB pixel data as interleaved unsigned chars (R0 G0 B0 R1 G1 B1 R2 G2 B2 .... etc) or 0 if an error ocured
**/
unsigned char * Terrain::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;
	//Read in the ppm file.
	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}
	//Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);
	//Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);
	//Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	//Read image data:
	rawData = new unsigned char[width * height * 3];
	read = (unsigned int)fread(rawData, width * height * 3, 1, fp);
	fclose(fp);
	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}
	return rawData;//Return rawData or 0 if failed.
}

/* Returns the interpolated height for BaryCentric coordinates. */
float Terrain::BaryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos)
{
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);

	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;

	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}

/* Gets the height of the terrain at a given position. */
float Terrain::getHeight(glm::vec3 position)
{
	//Get the coordinates in terms of the terrain.
	float terrain_x = position.x - this->x;
	float terrain_z = position.z - this->z;
	//Check for boundaries.
	if (terrain_x > SIZE || terrain_z > SIZE) {
		printf("Accessing a point out of bounds of this terrain. Please check to make sure we're accessing the proper terrain.\n");
		return 0;
	}
	//Get grid coordinate to determine which vertices to interpolate.
	float gridSize = (float)(SIZE) / (float)(VERTEX_COUNT-1);
	float gridX = floor(terrain_x / gridSize);
	float gridZ = floor(terrain_z / gridSize);
	//Get xCoord and zCoord within the square.
	float xCoord = fmod(terrain_x, gridSize) / gridSize;
	float zCoord = fmod(terrain_z, gridSize) / gridSize;
	//Compute the height between the triangles.
	float answer;
	if (xCoord <= (1 - zCoord))
	{
		answer = BaryCentric(glm::vec3(0.0f, this->vertices[gridZ*VERTEX_COUNT + gridX].y, 0.0f), glm::vec3(1.0f, this->vertices[gridZ*VERTEX_COUNT + gridX + 1].y, 0.0f), glm::vec3(0.0f, this->vertices[(gridZ + 1)*VERTEX_COUNT + gridX + 1].y, 1.0f), glm::vec2(xCoord, zCoord));
	}
	else
	{
		answer = BaryCentric(glm::vec3(1.0f, this->vertices[gridZ*VERTEX_COUNT + gridX + 1].y, 0.0f), glm::vec3(1.0f, this->vertices[(gridZ + 1)*VERTEX_COUNT + gridX + 1].y, 1.0f), glm::vec3(0.0f, this->vertices[(gridZ + 1)*VERTEX_COUNT + gridX].y, 1.0f), glm::vec2(xCoord, zCoord));
	}
	//Return the result.
	return answer;
}
