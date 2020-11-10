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

	posLightning.y = 0;
	posLightning.x = (rand() % 310) - 40;

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


	spritesheet2.loadFromFile("images/start_lightning.png", TEXTURE_PIXEL_FORMAT_RGBA);
	electric_ball = Sprite::createSprite(glm::ivec2(40, 40), glm::vec2(0.5, 0.5), &spritesheet2, &shaderProgram);
	electric_ball->setNumberAnimations(1);
	electric_ball->setAnimationSpeed(STRIKE, 20);
	electric_ball->addKeyframe(STRIKE, glm::vec2(0.f, 0.f));
	electric_ball->addKeyframe(STRIKE, glm::vec2(0.5, 0.f));
	electric_ball->addKeyframe(STRIKE, glm::vec2(0.f, 0.5));
	electric_ball->addKeyframe(STRIKE, glm::vec2(0.5, 0.5));

	electric_ball->setPosition(glm::vec2(float(tileMapDispl.x + 50), float(tileMapDispl.y + 50)));
	electric_ball->changeAnimation(STRIKE);


	counter_restart = rand() % 3000;
	eBall = false;
	isStrike = false;



}

void Lightning::update(int deltaTime)
{
	//counter += deltaTime;
	//if (counter > 580) {
	//	posLightning.x = (rand() % 310) -40;
	//	posLightning.x = 50;
	//	sprite->changeAnimation(STRIKE);
	//	counter = 0;
	//}

	//if (counter < 250 && !PlayGameState::instance().getGodMode() && !PlayGameState::instance().getIsDead()) {

	//	glm::vec2 posPlayer = PlayGameState::instance().getPlayerPosition();
	//	if ((posLightning.x + 91) > posPlayer.x && (posPlayer.x + 38) > (posLightning.x + 75))
	//	{
	//		PlayGameState::instance().lost_life();
	//		//isDead = true;
	//	}
	//}


	if (PlayGameState::instance().getStarted())
	{

		if (isStrike)
		{
			counter_strike -= deltaTime;

			if (counter_strike <= 0 && !PlayGameState::instance().getIsDead()) {
				isStrike = false;
				counter_restart = rand() % 3000;
				posLightning.x = (rand() % 310) - 40;
			}
			else if (!PlayGameState::instance().getGodMode() && !PlayGameState::instance().getIsDead())
			{
				glm::vec2 posPlayer = PlayGameState::instance().getPlayerPosition();
				if ((posLightning.x + 91) > posPlayer.x && (posPlayer.x + 38) > (posLightning.x + 75))
				{
					PlayGameState::instance().lost_life();
					//isDead = true;
				}
			}
		}


		else if (eBall)
		{
			counter_ball -= deltaTime;

			if (counter_ball <= 0)
			{
				eBall = false;
				isStrike = true;
				counter_strike = 580;
				sprite->changeAnimation(STRIKE);
			}
		}

		else if (counter_restart <= 0) {
			eBall = true;
			counter_ball = 2000;
		}

		else
		{
			counter_restart -= deltaTime;
		}


		sprite->update(deltaTime);
		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posLightning.x), float(tileMapDispl.y + posLightning.y)));

		electric_ball->update(deltaTime);
		electric_ball->setPosition(glm::vec2(float(tileMapDispl.x + posLightning.x + 64), float(tileMapDispl.y - 32)));

	}

}

void Lightning::render()
{
	if (isStrike) {
		sprite->render();
	}

	else if (eBall)
	{
		electric_ball->render();
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