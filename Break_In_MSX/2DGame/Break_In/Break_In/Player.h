#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "SoundManager.h"


// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.


class Player
{

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime, glm::vec2 posBall);
	void render();
	
	void setTileMap(TileMap *tileMap);
	glm::vec2 getPosition();
	void setPosition(const glm::vec2 &pos);

	void dead();
	
private:
	glm::ivec2 tileMapDispl, posPlayer;
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

	bool isDead;
	int deadCounter;

	const SoundManager* soundManager;
	FMOD::Sound* music_dead;
	FMOD::Channel* channel;

};


#endif // _PLAYER_INCLUDE


