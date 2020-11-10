#pragma once

#ifndef _CLOUD_INCLUDE
#define _CLOUD_INCLUDE

#include <random>
#include "Sprite.h"
#include "ShaderProgram.h"
#include "Player.h"


class Cloud
{
public:
	void init();
	void reinitCloud();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	ShaderProgram texProgram;
	glm::mat4 projection;

	Sprite* sprite;
	Texture spritesheet;

	glm::vec2 posCloud;
	glm::vec2 goTo;

	int counter = 0;
	int waitTime;
	int cloudVel;

	int xmin, xmax, ymin, ymax;

	glm::vec2 posPlayer;
};

#endif
