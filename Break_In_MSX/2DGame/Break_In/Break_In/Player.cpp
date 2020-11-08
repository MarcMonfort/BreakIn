#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"
#include "PlayGameState.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

#define SIZE_X_PLAYER 38
#define SIZE_Y_PLAYER 52

#define INIT_PLAYER_X 11
#define INIT_PLAYER_Y 20.5

#define PI 3.1415



enum PlayerAnims
{
	DOWN_CENTER, 
	DOWN_RIGHT, 
	DOWN_LEFT,

	MIDDLE_CENTER,
	MIDDLE_RIGHT_1,
	MIDDLE_RIGHT_2,
	MIDDLE_RIGHT_3,
	MIDDLE_LEFT_1,
	MIDDLE_LEFT_2,
	MIDDLE_LEFT_3,

	UP_CENTER,
	UP_LEFT_1,
	UP_LEFT_2,
	UP_RIGHT_1,
	UP_RIGHT_2,

	DEAD


};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/eyes_set_3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(SIZE_X_PLAYER, SIZE_Y_PLAYER), glm::vec2(0.2, 0.2), &spritesheet, &shaderProgram); //glm::vec2(0.1484375, 0.203125)
	sprite->setNumberAnimations(16);
	
		sprite->setAnimationSpeed(DOWN_CENTER, 1);
		sprite->addKeyframe(DOWN_CENTER, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(DOWN_RIGHT, 1);
		sprite->addKeyframe(DOWN_RIGHT, glm::vec2(0.2, 0.6));
		
		sprite->setAnimationSpeed(DOWN_LEFT, 1);
		sprite->addKeyframe(DOWN_LEFT, glm::vec2(0.4, 0.6));

		sprite->setAnimationSpeed(MIDDLE_CENTER, 1);
		sprite->addKeyframe(MIDDLE_CENTER, glm::vec2(0.2f, 0.f));

		sprite->setAnimationSpeed(MIDDLE_RIGHT_1, 1);
		sprite->addKeyframe(MIDDLE_RIGHT_1, glm::vec2(0.4, 0.f));

		sprite->setAnimationSpeed(MIDDLE_RIGHT_2, 1);
		sprite->addKeyframe(MIDDLE_RIGHT_2, glm::vec2(0.6, 0.f));

		sprite->setAnimationSpeed(MIDDLE_RIGHT_3, 1);
		sprite->addKeyframe(MIDDLE_RIGHT_3, glm::vec2(0.f, 0.2f));

		sprite->setAnimationSpeed(MIDDLE_LEFT_1, 1);
		sprite->addKeyframe(MIDDLE_LEFT_1, glm::vec2(0.2f, 0.2f));

		sprite->setAnimationSpeed(MIDDLE_LEFT_2, 1);
		sprite->addKeyframe(MIDDLE_LEFT_2, glm::vec2(0.4f, 0.2f));

		sprite->setAnimationSpeed(MIDDLE_LEFT_3, 1);
		sprite->addKeyframe(MIDDLE_LEFT_3, glm::vec2(0.6f, 0.2f));

		sprite->setAnimationSpeed(UP_CENTER, 1);
		sprite->addKeyframe(UP_CENTER, glm::vec2(0.f, 0.4));

		sprite->setAnimationSpeed(UP_RIGHT_1, 1);
		sprite->addKeyframe(UP_RIGHT_1, glm::vec2(0.2, 0.4));

		sprite->setAnimationSpeed(UP_RIGHT_2, 1);
		sprite->addKeyframe(UP_RIGHT_2, glm::vec2(0.4, 0.4));

		sprite->setAnimationSpeed(UP_LEFT_1, 1);
		sprite->addKeyframe(UP_LEFT_1, glm::vec2(0.6, 0.4));

		sprite->setAnimationSpeed(UP_LEFT_2, 1);
		sprite->addKeyframe(UP_LEFT_2, glm::vec2(0.f, 0.6));

		sprite->setAnimationSpeed(DEAD, 4);
		sprite->addKeyframe(DEAD, glm::vec2(0.8, 0.f));
		sprite->addKeyframe(DEAD, glm::vec2(0.0, 0.8));
		sprite->addKeyframe(DEAD, glm::vec2(0.8, 0.2));
		sprite->addKeyframe(DEAD, glm::vec2(0.2, 0.8));
		sprite->addKeyframe(DEAD, glm::vec2(0.8, 0.4));
		sprite->addKeyframe(DEAD, glm::vec2(0.4, 0.8));
		sprite->addKeyframe(DEAD, glm::vec2(0.8, 0.6));

	isDead = false;

	soundManager = Game::instance().getSoundManager();
	music_dead = soundManager->loadSound("sounds/life_lost.mp3", FMOD_DEFAULT);
		
	sprite->changeAnimation(DOWN_CENTER);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	velocity.x = 6;
	velocity.y = 4;
}

void Player::update(int deltaTime, glm::vec2 posBall)
{
	sprite->update(deltaTime);


	if (isDead) {
		if (sprite->animation() != DEAD) {
			sprite->changeAnimation(DEAD);
			deadCounter = 0;
			channel = soundManager->playSound(music_dead);

		}
		else {
			deadCounter += deltaTime;
			if (deadCounter > 1700) {
				isDead = false;
				setPosition(glm::vec2(176, 328));
				PlayGameState::instance().setStarted(false);
				PlayGameState::instance().setIsDead(false);
				PlayGameState::instance().setAlarm(false);
			}
		}

	}
	else {

		float distX = -(posPlayer.x + 19) + (posBall.x + 11);
		float distY = (posPlayer.y + 26) - (posBall.y + 11);

		float angle = atan2(distY, distX) * 180.0 / PI;

		if (angle < 0)
			angle = 360 + angle;

		//cout << angle << endl;

		if (angle < 30 || angle > 330) {
			if (abs(distX) > 200)
				sprite->changeAnimation(MIDDLE_RIGHT_3);
			else if (abs(distX) > 100)
				sprite->changeAnimation(MIDDLE_RIGHT_2);
			else
				sprite->changeAnimation(MIDDLE_RIGHT_1);
		}
		else if (angle < 50) {
			sprite->changeAnimation(UP_RIGHT_2);
		}
		else if (angle < 70) {
			sprite->changeAnimation(UP_RIGHT_1);
		}
		else if (angle < 110) {
			if (abs(distY) > 100)
				sprite->changeAnimation(UP_CENTER);
			else
				sprite->changeAnimation(MIDDLE_CENTER);
		}
		else if (angle < 130) {
			sprite->changeAnimation(UP_LEFT_1);
		}
		else if (angle < 150) {
			sprite->changeAnimation(UP_LEFT_1);
		}
		else if (angle < 210) {
			if (abs(distX) > 200)
				sprite->changeAnimation(MIDDLE_LEFT_3);
			else if (abs(distX) > 100)
				sprite->changeAnimation(MIDDLE_LEFT_2);
			else
				sprite->changeAnimation(MIDDLE_LEFT_1);
		}
		else if (angle < 240) {
			sprite->changeAnimation(DOWN_LEFT);
		}
		else if (angle < 300) {
			sprite->changeAnimation(DOWN_CENTER);
		}
		else if (angle < 330) {
			sprite->changeAnimation(DOWN_RIGHT);
		}



		// Esquerra o dreta
		if (Game::instance().getGameState()->getSpecialKey(GLUT_KEY_LEFT))
		{
			posPlayer.x -= velocity.x;
			if (posPlayer.x < 16)
			{
				if (posPlayer.x + (velocity.x-1) >= 16) 
					posPlayer.x += (velocity.x-1);
				else
					posPlayer.x += velocity.x;

			}
		}
		else if (Game::instance().getGameState()->getSpecialKey(GLUT_KEY_RIGHT))
		{
			posPlayer.x += velocity.x;
			if (posPlayer.x > 330)
			{
				if (posPlayer.x - (velocity.x - 1) <= 330)
					posPlayer.x -= (velocity.x - 1);
				else
					posPlayer.x -= velocity.x;
			}
		}
		/*else
		{
			if(sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if(sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}*/

		// Amunt o avall
		if (Game::instance().getGameState()->getSpecialKey(GLUT_KEY_UP))
		{
			PlayGameState::instance().setStarted(true);
			posPlayer.y -= velocity.y;
			if (posPlayer.y < 16)
			{
				if (posPlayer.y + (velocity.y - 1) >= 16)
					posPlayer.y += (velocity.y - 1);
				else
					posPlayer.y += velocity.y;
			}
		}
		else if (Game::instance().getGameState()->getSpecialKey(GLUT_KEY_DOWN))
		{
			PlayGameState::instance().setStarted(true);

			posPlayer.y += velocity.y;
			if (posPlayer.y > 332)
			{
				if (posPlayer.y - (velocity.y-1) <= 332)
					posPlayer.y -= (velocity.y-1);
				else
					posPlayer.y -= velocity.y;
			}

		}
		/*else
		{
			if (sprite->animation() == MOVE_LEFT)
				sprite->changeAnimation(STAND_LEFT);
			else if (sprite->animation() == MOVE_RIGHT)
				sprite->changeAnimation(STAND_RIGHT);
		}*/

		sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
	}
}

void Player::render()
{
	sprite->render();
}

void Player::dead()
{
	isDead = true;
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
}

glm::vec2 Player::getPosition()
{
	return(posPlayer);
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}


void Player::addVelocity(float increase)
{
	velocity.x *= increase;
	velocity.y *= increase;
}