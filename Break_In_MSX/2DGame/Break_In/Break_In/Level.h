#pragma once
#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE


#include <glm/glm.hpp>
#include "GameState.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "GameState.h"


class Level
{

public:
	void createLevel(int numLevel);
	void update(int deltaTime);
	void render();


private:
	void initShaders();

private:
	TileMap* map;
	Player* player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int numLevel;
};


#endif
