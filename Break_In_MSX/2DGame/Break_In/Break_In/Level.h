#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE


#include <glm/glm.hpp>
#include "GameState.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"


class Level
{

public:
	void createLevel(int numLevel, int numMap);
	void update(int deltaTime);
	void render();

	TileMap* getMap();


	enum transitions
	{
		upCenter,
		downCenter,
		centerUp,
		centerDown
	};

	void setTransition(int transition);

private:
	void initShaders();

private:
	TileMap* map;
	//Player* player;
	//Ball* ball;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int numLevel;

	float transTime;
	int transition;

	Sprite* background;
	Texture spritesheet;



};


#endif
