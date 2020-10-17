#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"

#define ANGLE_STEP 4

enum BallAnims
{
	EXISTIR
};

void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(22,22), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));

	vX = 2;
	vY = 1;
}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);

	posBall.y += vY;
	if (map->collisionMoveUp(posBall, glm::ivec2(22, 22))) {
		cout << "Up" << endl;
		posBall.y -= vY;
		vY = abs(vY);
	}
	else if (map->collisionMoveDown(posBall, glm::ivec2(22, 22))) {
		cout << "Down" << endl;
		posBall.y -= vY;
		vY = -abs(vY);
	}

	posBall.x += vX;
	if (map->collisionMoveLeft(posBall, glm::ivec2(22, 22))) {
		cout << "Left" << endl;
		posBall.x -= vX;
		vX = abs(vX);
	}
	else if (map->collisionMoveRight(posBall, glm::ivec2(22, 22))) {
		cout << "Right" << endl;
		posBall.x -= vX;
		vX = -abs(vX);
	}

	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::render()
{
	sprite->render();
}

void Ball::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

glm::fvec2 Ball::getPosition()
{
	return posBall;
}

void Ball::addVelocity(float increase)
{
	vX *= increase;
	vY *= increase;
}

void Ball::setPosition(const glm::vec2& pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}




