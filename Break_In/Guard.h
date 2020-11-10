#pragma once

#ifndef _GUARD_INCLUDE
#define _GUARD_INCLUDE

#include "Sprite.h"
#include "TileMap.h"


class Guard
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition();

	void reset();

private:
	glm::ivec2 tileMapDispl, posGuard;
	Texture spritesheet;
	Sprite* sprite;

	int counter = 0;
	int counter2 = 0;

	glm::vec2 goTo;

	glm::vec2 posPlayer;

	bool isDead;
};

#endif
