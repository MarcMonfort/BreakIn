#include "Lightning.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


enum PlayerAnims
{
	MOVE_RIGHT, STAND_LEFT, STAND_RIGHT, MOVE_LEFT
};

void Lightning::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/ray-set2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(141, 352), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(MOVE_RIGHT, 20);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75, 0.f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.25));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.25));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75, 0.25));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5, 0.5));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75, 0.5));

	sprite->changeAnimation(MOVE_RIGHT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLightning.x), float(tileMapDispl.y + posLightning.y)));

}

void Lightning::update(int deltaTime)
{
	sprite->update(deltaTime);
	counter += deltaTime;

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLightning.x), float(tileMapDispl.y + posLightning.y)));
}

void Lightning::render()
{
	sprite->render();
}


void Lightning::setPosition(const glm::vec2& pos)
{
	posLightning = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLightning.x), float(tileMapDispl.y + posLightning.y)));
}

glm::vec2 Lightning::getPosition() {
	return posLightning;
}