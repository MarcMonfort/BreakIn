#ifndef _RING_INCLUDE
#define _RING_INCLUDE

#include "Sprite.h"
#include "TileMap.h"


class Ring
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition();

private:
	glm::ivec2 tileMapDispl, posPlayer;
	Texture spritesheet;
	Sprite* sprite;

	int counter = 0;
};

#endif
