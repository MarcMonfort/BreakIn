#ifndef _BALL_INCLUDE
#define _BALL_INCLUDE


#include "Sprite.h"
#include "TileMap.h"


class Ball
{

public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

private:
	glm::ivec2 tileMapDispl;
	glm::fvec2 posBall;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;

	float vX;
	float vY;

};


#endif // _BALL_INCLUDE



