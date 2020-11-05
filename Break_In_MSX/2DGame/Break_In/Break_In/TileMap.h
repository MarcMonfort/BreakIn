#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE


#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "SoundManager.h"


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
	
	int getTileSize() const { return tileSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) ;	//why was const???
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) ;
	bool collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size) ;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size) ;

	bool collisionPoint(const glm::ivec2& pos);
	bool checkCollision(const glm::ivec2& pos);


	enum block
	{
		wall,
		lose,
		basic,
		strong,
		key,
		ring,
		arrow,
		bag,
		coin,
	};

	string getBackgroundFile();

	bool hasRing();
	bool alarmOn();
	glm::vec2 getRingPosition();
	
	
private:
	bool loadLevel(const string &levelFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
	int checkBlock(int block);
	bool treatCollision(int pos);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;

	//ShaderProgram b;

	glm::vec2 a;
	ShaderProgram b;

	string backgroundFile;

	bool bRing = false;
	bool bAlarm = false;
	glm::vec2 ringPosition;

	const SoundManager* soundManager;
	FMOD::Sound* music_wall_coin;
	FMOD::Sound* music_bag;
	FMOD::Sound* music_basic_block;
	FMOD::Channel* channel;
};


#endif // _TILE_MAP_INCLUDE


