#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"
#include "SoundManager.h"


class Ball
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime, glm::vec2 posPlayer);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);
	glm::fvec2 getPosition();

	bool collisionPlayer(const glm::ivec2& posPlayer);

	void addVelocity(float increase);

	void setVelocity(glm::vec2 velocity);
	glm::vec2 getVelocity();

private:
	glm::ivec2 tileMapDispl;
	glm::fvec2 posBall;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	float vX;
	float vY;

	glm::fvec2 velocity;

	int radi;
	int shortSide;
	int longSide;
	glm::ivec2 center;

	const SoundManager* soundManager;
	FMOD::Sound* music;
	FMOD::Channel* channel;
};


#endif // _BALL_INCLUDE



