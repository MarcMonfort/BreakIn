#ifndef _LIGHTING_INCLUDE
#define _LIGHTING_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

class Lightning
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition();

private:
	glm::ivec2 tileMapDispl, posLightning;
	Texture spritesheet;
	Sprite* sprite;

	int counter = 0;
};


#endif