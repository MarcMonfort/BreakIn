#ifndef _DROP_INCLUDE
#define _DROP_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "SoundManager.h"


class Drop
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, float startVelocity);
	void update(int deltaTime, glm::vec2 posPlayer);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::fvec2 getPosition();

	bool collisionPlayer(const glm::ivec2& posPlayer);

	void addVelocity(float increase);

	void setVelocity(glm::vec2 velocity);
	glm::vec2 getVelocity();

	void setStartVelocity(float v);

private:
	glm::fvec2 posDrop;
	glm::fvec2 velocity;
	glm::fvec2 center;

	glm::ivec2 tileMapDispl;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	bool bJumping;
	int jumpAngle, startY;

	float gravity = 0.1;
	float energy = 1.f;

	int countAnim;
	bool isDead;

	int radi;
	int shortSide;
	int longSide;

	int timeToFall;

	bool isSplash;

	const SoundManager* soundManager;
	FMOD::Sound* music_player;
	FMOD::Channel* channel;

	float startVelocity;
};

#endif
