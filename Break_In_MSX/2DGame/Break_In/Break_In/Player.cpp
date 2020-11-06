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
	UP_RIGHT_2


};


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/eyes_set.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(SIZE_X_PLAYER, SIZE_Y_PLAYER), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram); //glm::vec2(0.1484375, 0.203125)
	sprite->setNumberAnimations(15);
	
		sprite->setAnimationSpeed(DOWN_CENTER, 8);
		sprite->addKeyframe(DOWN_CENTER, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(DOWN_RIGHT, 8);
		sprite->addKeyframe(DOWN_RIGHT, glm::vec2(0.25, 0.75));
		
		sprite->setAnimationSpeed(DOWN_LEFT, 8);
		sprite->addKeyframe(DOWN_LEFT, glm::vec2(0.5, 0.75));

		sprite->setAnimationSpeed(MIDDLE_CENTER, 8);
		sprite->addKeyframe(MIDDLE_CENTER, glm::vec2(0.25f, 0.f));

		sprite->setAnimationSpeed(MIDDLE_RIGHT_1, 8);
		sprite->addKeyframe(MIDDLE_RIGHT_1, glm::vec2(0.5, 0.f));

		sprite->setAnimationSpeed(MIDDLE_RIGHT_2, 8);
		sprite->addKeyframe(MIDDLE_RIGHT_2, glm::vec2(0.75, 0.f));

		sprite->setAnimationSpeed(MIDDLE_RIGHT_3, 8);
		sprite->addKeyframe(MIDDLE_RIGHT_3, glm::vec2(0.f, 0.25f));

		sprite->setAnimationSpeed(MIDDLE_LEFT_1, 8);
		sprite->addKeyframe(MIDDLE_LEFT_1, glm::vec2(0.25f, 0.25f));

		sprite->setAnimationSpeed(MIDDLE_LEFT_2, 8);
		sprite->addKeyframe(MIDDLE_LEFT_2, glm::vec2(0.5f, 0.25f));

		sprite->setAnimationSpeed(MIDDLE_LEFT_3, 8);
		sprite->addKeyframe(MIDDLE_LEFT_3, glm::vec2(0.75f, 0.25f));

		sprite->setAnimationSpeed(UP_CENTER, 8);
		sprite->addKeyframe(UP_CENTER, glm::vec2(0.f, 0.5));

		sprite->setAnimationSpeed(UP_RIGHT_1, 8);
		sprite->addKeyframe(UP_RIGHT_1, glm::vec2(0.25, 0.5));

		sprite->setAnimationSpeed(UP_RIGHT_2, 8);
		sprite->addKeyframe(UP_RIGHT_2, glm::vec2(0.5, 0.5));

		sprite->setAnimationSpeed(UP_LEFT_1, 8);
		sprite->addKeyframe(UP_LEFT_1, glm::vec2(0.75, 0.5));

		sprite->setAnimationSpeed(UP_LEFT_2, 8);
		sprite->addKeyframe(UP_LEFT_2, glm::vec2(0.f, 0.75));


		
	sprite->changeAnimation(DOWN_CENTER);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime, glm::vec2 posBall)
{
	sprite->update(deltaTime);

	float distX = - (posPlayer.x+19) + (posBall.x+11);
	float distY = (posPlayer.y+26) - (posBall.y+11);

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
	if(Game::instance().getGameState()->getSpecialKey(GLUT_KEY_LEFT))
	{
		posPlayer.x -= 4;
		/*if(map->collisionMoveLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += 2;
			sprite->changeAnimation(STAND_LEFT);
		}*/
	}
	else if(Game::instance().getGameState()->getSpecialKey(GLUT_KEY_RIGHT))
	{
		posPlayer.x += 4;
		/*if(map->collisionMoveRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}*/
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

		posPlayer.y -= 4;
		/*if (map->collisionMoveUp(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.y += 2;
			sprite->changeAnimation(STAND_LEFT);
		}*/
	}
	else if (Game::instance().getGameState()->getSpecialKey(GLUT_KEY_DOWN))
	{
		PlayGameState::instance().setStarted(true);

		posPlayer.y += 4;
		/*if (map->collisionMoveDown(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.y -= 2;
			sprite->changeAnimation(STAND_RIGHT);
		}*/
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

void Player::render()
{
	sprite->render();
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
