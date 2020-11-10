#include "Lightning.h"
#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "PlayGameState.h"



enum PlayerAnims
{
	STRIKE
};

void Lightning::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/ray-set2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(166, 416), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(STRIKE, 20);
	sprite->addKeyframe(STRIKE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(STRIKE, glm::vec2(0.25, 0.f));
	sprite->addKeyframe(STRIKE, glm::vec2(0.5, 0.f));
	sprite->addKeyframe(STRIKE, glm::vec2(0.75, 0.f));
	sprite->addKeyframe(STRIKE, glm::vec2(0.f, 0.25));
	sprite->addKeyframe(STRIKE, glm::vec2(0.25, 0.25));
	sprite->addKeyframe(STRIKE, glm::vec2(0.5, 0.25));
	sprite->addKeyframe(STRIKE, glm::vec2(0.75, 0.25));
	sprite->addKeyframe(STRIKE, glm::vec2(0.f, 0.5));
	sprite->addKeyframe(STRIKE, glm::vec2(0.25, 0.5));
	sprite->addKeyframe(STRIKE, glm::vec2(0.5, 0.5));
	sprite->addKeyframe(STRIKE, glm::vec2(0.75, 0.5));

	sprite->changeAnimation(STRIKE);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLightning.x), float(tileMapDispl.y + posLightning.y)));

}

void Lightning::update(int deltaTime)
{
	counter += deltaTime;
	if (counter > 580) {
		//posLightning.x = (rand() % 310) -40;
		posLightning.x = 50;
		sprite->changeAnimation(STRIKE);
		counter = 0;
	}

	if (counter < 250 && !PlayGameState::instance().getGodMode() && !PlayGameState::instance().getIsDead()) {

		glm::vec2 posPlayer = PlayGameState::instance().getPlayerPosition();
		if ((posLightning.x + 91) > posPlayer.x && (posPlayer.x + 38) > (posLightning.x + 75))
		{
			PlayGameState::instance().lost_life();
			//isDead = true;
		}
	}

	sprite->update(deltaTime);

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLightning.x), float(tileMapDispl.y + posLightning.y)));
}

void Lightning::render()
{
	if (counter < 580) {
		sprite->render();
	}
}


void Lightning::setPosition(const glm::vec2& pos)
{
	posLightning = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLightning.x), float(tileMapDispl.y + posLightning.y)));
}

glm::vec2 Lightning::getPosition() {
	return posLightning;
}