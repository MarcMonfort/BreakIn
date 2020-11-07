#include "Guard.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "PlayGameState.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

enum PlayerAnims
{
	MOVE_RIGHT, STAND_RIGHT, MOVE_LEFT
};

void Guard::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/guard.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(24, 16), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVE_RIGHT, 50);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.50, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.25));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.50, 0.25));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75, 0.25));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.50));

	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.50));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75, 0.25));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.50, 0.25));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.25));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.75, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.50, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.0f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.0f));

	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.0f, 0.50));

	isDead = false;

	sprite->changeAnimation(MOVE_RIGHT);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGuard.x), float(tileMapDispl.y + posGuard.y)));

}

void Guard::update(int deltaTime)
{
	sprite->update(deltaTime);

	if (!isDead) {
		counter += deltaTime;
		counter2 += deltaTime;

		posPlayer = PlayGameState::instance().getPlayerPosition();

		if (sprite->animation() == MOVE_RIGHT && counter > 2000) {
			sprite->changeAnimation(STAND_RIGHT);
			counter = 0;
			goTo = glm::vec2((posPlayer.x + 19) - (posGuard.x + 12), (posPlayer.y + 15) - (posGuard.y + 8));
			int aux;
			if (max(abs(goTo.x), abs(goTo.y)) > 200)
				aux = 4;
			else if (max(abs(goTo.x), abs(goTo.y)) > 100)
				aux = 3;
			else
				aux = 2;
			goTo = glm::normalize(goTo);
			goTo.x *= aux;
			goTo.y *= aux;
			if (goTo.x > 0)
				goTo.x = ceil(goTo.x);
			else
				goTo.x = floor(goTo.x);
			if (goTo.y > 0)
				goTo.y = ceil(goTo.y);
			else
				goTo.y = floor(goTo.y);
		}
		if (sprite->animation() == STAND_RIGHT && counter2 > 30) {
			posGuard.x += goTo.x;
			posGuard.y += goTo.y;
			counter2 = 0;
		}
		if (sprite->animation() == STAND_RIGHT && counter > 2000) {
			sprite->changeAnimation(MOVE_RIGHT);
			counter = 0;
		}


		if ((posGuard.x + 24) > posPlayer.x && (posPlayer.x + 38) > posGuard.x &&
			(posGuard.y + 16) > posPlayer.y && (posPlayer.y + 52) > posGuard.y)
		{
			PlayGameState::instance().lost_life();
			isDead = true;
		}

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGuard.x), float(tileMapDispl.y + posGuard.y)));
	}
}

void Guard::render()
{
	sprite->render();
}

void Guard::reset() {
	sprite->changeAnimation(MOVE_RIGHT);
	counter = 0;
	isDead = false;
}


void Guard::setPosition(const glm::vec2& pos)
{
	posGuard = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posGuard.x), float(tileMapDispl.y + posGuard.y)));
}

glm::vec2 Guard::getPosition() {
	return posGuard;
}