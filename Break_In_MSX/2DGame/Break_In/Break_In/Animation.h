#ifndef _ANIMATION_INCLUDE
#define _ANIMATION_INCLUDE


#include <glm/glm.hpp>
#include "GameState.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Thief.h"

class Animation
{

public:
	void init();
	void update(int deltaTime);
	void render();
	void restart();


private:
	void initShaders();

	Sprite* background;
	Texture spritesheet;


	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Thief* thief;

};

#endif
