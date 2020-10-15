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
	spritesheet.loadFromFile("images/ball_ampliada.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	angle = 25;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (map->collisionMoveLeft(posBall, glm::ivec2(32, 32))) {
		cout << "Left" << endl;
		if (angle >= 90 && angle <= 180)
			angle = 90 - (angle - 90);
		else if (angle >= 180 && angle <= 270)
			angle = 270 + (270 - angle);
	}
	else if (map->collisionMoveRight(posBall, glm::ivec2(32, 32))) {
		cout << "Right" << endl;
		if (angle >= 0 && angle <= 90)
			angle = 90 + (90 - angle);
		else if (angle >= 270 && angle <= 360)
			angle = 270 - (angle - 270);
	}

	if (map->collisionMoveUp(posBall, glm::ivec2(32, 32))) {
		cout << "Up" << endl;
		if (angle >= 0 && angle <= 90)
			angle = 360 - angle;
		else if (angle >= 90 && angle <= 180)
			angle = 180 + (180 - angle);
	}
	else if (map->collisionMoveDown(posBall, glm::ivec2(32, 32))) {
		cout << "Down" << endl;
		if (angle >= 180 && angle <= 270)
			angle = 180 - (angle - 180);
		else if (angle >= 270 && angle <= 360)
			angle = 360 - angle;
	}

	const double pi = 3.14159265358979323846;
	posBall.x += float(1 * cos(2 * pi * angle / 360));
	posBall.y -= float(1 * sin(2 * pi * angle / 360));

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

void Ball::setPosition(const glm::vec2& pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}




