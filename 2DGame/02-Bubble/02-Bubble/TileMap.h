#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include <set>



// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.


class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program);

	~TileMap();

	void render() const;
	void free();
	
	std::pair<int,int> getTileSize() const { 
		
		std::pair<char, double> tileSize;
		tileSize.first = tileSizeX;
		tileSize.second = tileSizeY; 
		return tileSize;

	}

	string getTrapsFile() { return trapsFile; }
	string getColumnsFile() { return columnsFile; }
	string getTorchesFile() { return torchesFile; }

	bool collisionMoveLeft(int posx, int posy, const glm::ivec2 &size) const;
	bool collisionMoveRight(int posx, int posy, const glm::ivec2 &size) const;
	bool collisionMoveDown(int posx, int posy, const glm::ivec2 &size, char dir) const;
	bool canIMoveUpRight(int posx, int posy, const glm::ivec2 &size) const;
	bool canIMoveUpLeft(int posx, int posy, const glm::ivec2 &size) const;

	void addTrapCollision(int pox, int posy, int type);
	void deleteTrapCollision(int pox, int posy);

private:

	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSizeX, tileSizeY, blockSize;
	string trapsFile, columnsFile, torchesFile, IAFile;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;
	int *mapTraps;
	
};


#endif // _TILE_MAP_INCLUDE


