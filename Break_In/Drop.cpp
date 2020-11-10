#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Drop.h"
#include "Game.h"
#include "PlayGameState.h"
#include <algorithm>    // std::max


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 20
#define FALL_STEP 4

enum DropAnim
{

	DOWN,
	LEFT,
	RIGHT,
	SPLASH_LEFT,
	SPLASH_RIGHT,

};

void Drop::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram, float startVelocity)
{
	spritesheet.loadFromFile("images/drops-set.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(DOWN, 1);
	sprite->addKeyframe(DOWN, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(RIGHT, 1);
	sprite->addKeyframe(RIGHT, glm::vec2(0.25, 0.f));

	sprite->setAnimationSpeed(LEFT, 1);
	sprite->addKeyframe(LEFT, glm::vec2(0.5, 0.f));

	sprite->setAnimationSpeed(SPLASH_LEFT, 20);
	sprite->addKeyframe(SPLASH_LEFT, glm::vec2(0.f, 0.25));
	sprite->addKeyframe(SPLASH_LEFT, glm::vec2(0.25, 0.25));
	sprite->addKeyframe(SPLASH_LEFT, glm::vec2(0.5, 0.25));
	sprite->addKeyframe(SPLASH_LEFT, glm::vec2(0.75, 0.25));

	sprite->setAnimationSpeed(SPLASH_RIGHT, 20);
	sprite->addKeyframe(SPLASH_RIGHT, glm::vec2(0.f, 0.5));
	sprite->addKeyframe(SPLASH_RIGHT, glm::vec2(0.25, 0.5));
	sprite->addKeyframe(SPLASH_RIGHT, glm::vec2(0.5, 0.5));
	sprite->addKeyframe(SPLASH_RIGHT, glm::vec2(0.75, 0.5));


	sprite->changeAnimation(DOWN);

	posDrop.y = -16;
	posDrop.x = (rand() % 320) + 24;

	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDrop.x), float(tileMapDispl.y + posDrop.y)));

	this->startVelocity = startVelocity;
	velocity.x = startVelocity;
	//velocity.x = rand() % 3 - 1;
	velocity.y = 1;

	jumpAngle = 0;

	radi = 7;
	shortSide = 6;
	longSide = 9;

	bJumping = false;
	isDead = true;
	countAnim = 0;

	timeToFall = rand() % 5000;
	//timeToFall = 0;

	isSplash = false;

	soundManager = Game::instance().getSoundManager();
	music_player = soundManager->loadSound("sounds/player.mp3", FMOD_DEFAULT);
}

void Drop::update(int deltaTime, glm::vec2 posPlayer)
{
	sprite->update(deltaTime);



	if (PlayGameState::instance().getStarted()) {

		if (timeToFall <= 0) {

			isDead = false;

			glm::fvec2 center_0 = glm::fvec2(posDrop.x + radi, posDrop.y + radi);
			//posDrop.y += velocity.y;
			posDrop.x += velocity.x;
			if (posDrop.x > 352) {
				velocity.x = -abs(velocity.x);
			}
			else if (posDrop.x < 16)
			{
				velocity.x = abs(velocity.x);
			}

			center = glm::fvec2(center_0.x + velocity.x, center_0.y + velocity.y);


			if (energy > 0.10) {
				if (map->checkCollision(glm::ivec2(center.x, center.y + radi))) {
					if (posDrop.y > 16) {
						if (map->collisionPoint(glm::ivec2(center.x, center.y + radi), 1)) {	//down
							//posDrop.y -= velocity.y;
							if (map->checkCollision(glm::ivec2(center.x, center.y + radi - velocity.y))) {
								//posDrop.x -= velocity.x;
								//velocity.x = -velocity.x;
							}
							//velocity.y = -abs(velocity.y);
							if (!bJumping) {
								bJumping = true;
								jumpAngle = 0;
								startY = posDrop.y;
								float aux = (velocity.y / 10.f) + 0.3;
								if (aux > energy)
									energy = 0;
								else
									energy -= aux;

								if (velocity.x > 0)
									sprite->changeAnimation(RIGHT);
								else
									sprite->changeAnimation(LEFT);



							}
						}
					}
				}
			}

			else {
				if (countAnim > 200) { //restart drop
					isDead = true;
					//timeToFall = rand() % 2;
					countAnim = 0;
					energy = 1.f;
					bJumping = false;
					//velocity.x = rand() % 3 - 1;
					velocity.x = startVelocity;
					velocity.y = 1;
					posDrop.y = -16;
					posDrop.x = (rand() % 320) + 24;
					sprite->changeAnimation(DOWN);
					isSplash = false;
				}
				else if (map->checkCollision(glm::ivec2(center.x, center.y + radi)) || energy <= 0.01) {
					if (velocity.x > 0 && sprite->animation() != SPLASH_RIGHT) {
						sprite->changeAnimation(SPLASH_RIGHT);
						countAnim = 0;
						isSplash = true;
					}
					else if (velocity.x <= 0 && sprite->animation() != SPLASH_LEFT && sprite->animation() != SPLASH_RIGHT) {
						sprite->changeAnimation(SPLASH_LEFT);
						countAnim = 0;
						isSplash = true;

					}
				}
			}

			if (!isSplash)
			{

				// else
				if (map->checkCollision(glm::ivec2(center.x - 5, center.y))) {
					if (posDrop.y > 16) {
						if (map->collisionPoint(glm::ivec2(center.x - 5, center.y), 1)) {	//left
							posDrop.x -= velocity.x;
							/*if (map->checkCollision(glm::ivec2(center.x - radi - velocity.x, center.y))) {
								posDrop.y -= velocity.y;
								velocity.y = -velocity.y;
							}*/
							velocity.x = abs(velocity.x);
							if (velocity.y < 0)
								sprite->changeAnimation(RIGHT);

						}
					}
				}
				else if (map->checkCollision(glm::ivec2(center.x + 5, center.y))) {	//right
					if (posDrop.y > 16) {
						if (map->collisionPoint(glm::ivec2(center.x + 5, center.y), 1)) {	//right
							posDrop.x -= velocity.x;
							/*if (map->checkCollision(glm::ivec2(center.x + radi - velocity.x, center.y))) {
								posDrop.y -= velocity.y;
								velocity.y = -velocity.y;
							}*/
							velocity.x = -abs(velocity.x);
							if (velocity.y < 0)
								sprite->changeAnimation(LEFT);

						}
					}
				}


				if (bJumping)
				{
					jumpAngle += JUMP_ANGLE_STEP;
					if (jumpAngle == 180)
					{
						bJumping = false;
					}
					else
					{
						posDrop.y = int(startY - (JUMP_HEIGHT * energy) * sin(3.14159f * jumpAngle / 180.f));
						if (jumpAngle > 90) {
							bJumping = false;
							velocity.y = 0;
							sprite->changeAnimation(DOWN);

						}
					}
				}
				else
				{
					posDrop.y += velocity.y;
					velocity.y += gravity;
				}
			}
			else {

				countAnim += deltaTime;

				velocity.x = 0;
				velocity.y = 0;
			}
			
			

		}
		else {
		timeToFall -= deltaTime;

		}


	}
	else {

		/*velocity.x = 0;
		velocity.y = 0;
		jumpAngle = 0;
		startY = posDrop.y;*/
		//posDrop = posPlayer + glm::vec2(8, -22); //Posicio inicial de la pilota sobre la barra del jugador
	}

	
	

	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDrop.x), float(tileMapDispl.y + posDrop.y)));
}

void Drop::render()
{
	if (!isDead) {

		sprite->render();
	}
}

void Drop::setStartVelocity(float v) {
	startVelocity = v;
}

void Drop::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

glm::fvec2 Drop::getPosition()
{
	return posDrop;
}

void Drop::addVelocity(float increase)
{
	velocity.x *= increase;
	velocity.y *= increase;
}

glm::vec2 Drop::getVelocity()
{
	return velocity;
}

void Drop::setVelocity(glm::vec2 velocity)
{
	this->velocity.x = velocity.x;
	this->velocity.y = velocity.y;
}

void Drop::setPosition(const glm::vec2& pos)
{
	posDrop = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posDrop.x), float(tileMapDispl.y + posDrop.y)));
}

bool Drop::collisionPlayer(const glm::ivec2& posPlayer)
{
	if (velocity.y >= -0.5) { //nomes si la pilota esta baixant

		float ball_size = 22.0; //es podria crear un atribut de la classe

		float playerWidth = 38.0; //es podria passar per parametre
		float pxmin = posPlayer.x;
		float pxmax = posPlayer.x + playerWidth;
		float pxcenter = posPlayer.x + playerWidth / 2.0;
		float py = posPlayer.y;

		float movement = abs(velocity.x) + abs(velocity.y);

		if (center.y + radi >= py && center.y + radi <= py + 10) {
			if (center.x >= pxmin && center.x <= pxmax) {
				if (!PlayGameState::instance().getGodMode()) {
					float moveX = (abs(pxcenter - center.x) / (playerWidth / 2)) * 0.9;  // %%% 
					float moveY = 1.f - moveX;

					if (center.x < pxcenter) {
						velocity.x = -moveX * movement;
						velocity.y = moveY * movement;
					}
					else if (center.x > pxcenter) {
						velocity.x = moveX * movement;
						velocity.y = moveY * movement;
					}
					else {
						velocity.x = 0;
						velocity.y = movement;
					}
				}
				else {
					velocity.x = 0;
					velocity.y = movement;
				}
				channel = soundManager->playSound(music_player);
				return true;
			}
		}

		if (center.y + longSide >= py && center.y + longSide <= py + 10) {
			if (center.x + shortSide >= pxmin && center.x + shortSide <= pxmax) {
				if (!PlayGameState::instance().getGodMode()) {
					float moveX = (abs(pxcenter - (center.x + shortSide)) / (playerWidth / 2)) * 0.9;  // %%% 
					float moveY = 1.f - moveX;

					if (center.x < pxcenter) {
						velocity.x = -moveX * movement;
						velocity.y = moveY * movement;
					}
					else if (center.x > pxcenter) {
						velocity.x = moveX * movement;
						velocity.y = moveY * movement;
					}
					else {
						velocity.x = 0;
						velocity.y = movement;
					}
				}
				else {
					velocity.x = 0;
					velocity.y = movement;
				}
				channel = soundManager->playSound(music_player);
				return true;
			}
			else if (center.x - shortSide >= pxmin && center.x - shortSide <= pxmax) {
				if (!PlayGameState::instance().getGodMode()) {
					float moveX = (abs(pxcenter - (center.x - shortSide)) / (playerWidth / 2)) * 0.9;  // %%% 
					float moveY = 1.f - moveX;

					if (center.x < pxcenter) {
						velocity.x = -moveX * movement;
						velocity.y = moveY * movement;
					}
					else if (center.x > pxcenter) {
						velocity.x = moveX * movement;
						velocity.y = moveY * movement;
					}
					else {
						velocity.x = 0;
						velocity.y = movement;
					}
				}
				else {
					velocity.x = 0;
					velocity.y = movement;
				}
				channel = soundManager->playSound(music_player);
				return true;
			}
		}

		if (center.y + shortSide >= py && center.y + shortSide <= py + 10) {
			if (center.x + longSide >= pxmin && center.x + longSide <= pxmax) {
				if (!PlayGameState::instance().getGodMode()) {
					float moveX = (abs(pxcenter - (center.x + longSide)) / (playerWidth / 2)) * 0.9;  // %%% 
					float moveY = 1.f - moveX;

					if (center.x < pxcenter) {
						velocity.x = -moveX * movement;
						velocity.y = moveY * movement;
					}
					else if (center.x > pxcenter) {
						velocity.x = moveX * movement;
						velocity.y = moveY * movement;
					}
					else {
						velocity.x = 0;
						velocity.y = movement;
					}
				}
				else {
					velocity.x = 0;
					velocity.y = movement;
				}
				channel = soundManager->playSound(music_player);
				return true;
			}
			else if (center.x - longSide >= pxmin && center.x - longSide <= pxmax) {
				if (!PlayGameState::instance().getGodMode()) {
					float moveX = (abs(pxcenter - (center.x - longSide)) / (playerWidth / 2)) * 0.9;  // %%% 
					float moveY = 1.f - moveX;

					if (center.x < pxcenter) {
						velocity.x = -moveX * movement;
						velocity.y = moveY * movement;
					}
					else if (center.x > pxcenter) {
						velocity.x = moveX * movement;
						velocity.y = moveY * movement;
					}
					else {
						velocity.x = 0;
						velocity.y = movement;
					}
				}
				else {
					velocity.x = 0;
					velocity.y = movement;
				}
				channel = soundManager->playSound(music_player);
				return true;
			}
		}
	}

	return false;
}



