#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Ball.h"
#include "Game.h"
#include "PlayGameState.h"


void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	spritesheet.loadFromFile("images/ball.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(22,22), glm::vec2(1.0, 1.0), &spritesheet, &shaderProgram);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));

	velocity.x = 1;
	velocity.y = -5;

	radi = 11;
	shortSide = 6; 
	longSide = 9;

	soundManager = Game::instance().getSoundManager();
	music_player = soundManager->loadSound("sounds/player.mp3", FMOD_DEFAULT);
}

void Ball::update(int deltaTime, glm::vec2 posPlayer)
{
	sprite->update(deltaTime);

	if (PlayGameState::instance().getStarted()) {

		glm::fvec2 center_0 = glm::fvec2(posBall.x + radi, posBall.y + radi);
		posBall.y += velocity.y;
		posBall.x += velocity.x;
		center = glm::fvec2(center_0.x + velocity.x, center_0.y + velocity.y);

		if (map->checkCollision(glm::ivec2(center.x, center.y - radi))) {
			if (map->collisionPoint(glm::ivec2(center.x, center.y - radi),0)) {	//up
				posBall.y -= velocity.y;
				if (map->checkCollision(glm::ivec2(center.x, center.y - radi - velocity.y))) {
					posBall.x -= velocity.x;
					velocity.x = - velocity.x;
				}
				velocity.y = abs(velocity.y);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x, center.y + radi))) {
			if (map->collisionPoint(glm::ivec2(center.x, center.y + radi),0)) {	//down
				posBall.y -= velocity.y;
				if (map->checkCollision(glm::ivec2(center.x, center.y + radi - velocity.y))) {
					posBall.x -= velocity.x;
					velocity.x = -velocity.x;
				}
				velocity.y = -abs(velocity.y);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x + shortSide, center.y - longSide))) {	//up-right-1
			if (map->collisionPoint(glm::ivec2(center.x + shortSide, center.y - longSide),0)) {	//up-right-1
				posBall.y -= velocity.y;
				if (map->checkCollision(glm::ivec2(center.x + shortSide, center.y - longSide - velocity.y))) {
					posBall.x -= velocity.x;
					velocity.x = -velocity.x;
				}
				velocity.y = abs(velocity.y);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x + shortSide, center.y + longSide))) {	//right-down-2
			if (map->collisionPoint(glm::ivec2(center.x + shortSide, center.y + longSide),0)) {	//right-down-2
				posBall.y -= velocity.y;
				if (map->checkCollision(glm::ivec2(center.x + shortSide, center.y + longSide - velocity.y))) {
					posBall.x -= velocity.x;
					velocity.x = -velocity.x;
				}
				velocity.y = -abs(velocity.y);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x - shortSide, center.y + longSide))) {	//down-left
			if (map->collisionPoint(glm::ivec2(center.x - shortSide, center.y + longSide),0)) {	//down-left
				posBall.y -= velocity.y;
				if (map->checkCollision(glm::ivec2(center.x - shortSide, center.y + longSide - velocity.y))) {
					posBall.x -= velocity.x;
					velocity.x = -velocity.x;
				}
				velocity.y = -abs(velocity.y);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x - shortSide, center.y - longSide))) {	//left-up
			if (map->collisionPoint(glm::ivec2(center.x - shortSide, center.y - longSide),0)) {	//left-up
				posBall.y -= velocity.y;
				if (map->checkCollision(glm::ivec2(center.x - shortSide, center.y - longSide - velocity.y))) {
					posBall.x -= velocity.x;
					velocity.x = -velocity.x;
				}
				velocity.y = abs(velocity.y);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x - radi, center.y))) {
			if (map->collisionPoint(glm::ivec2(center.x - radi, center.y),0)) {	//left
				posBall.x -= velocity.x;
				if (map->checkCollision(glm::ivec2(center.x - radi - velocity.x, center.y))) {
					posBall.y -= velocity.y;
					velocity.y = -velocity.y;
				}
				velocity.x = abs(velocity.x);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x + radi, center.y))) {	//right
			if (map->collisionPoint(glm::ivec2(center.x + radi, center.y),0)) {	//right
				posBall.x -= velocity.x;
				if (map->checkCollision(glm::ivec2(center.x + radi - velocity.x, center.y))) {
					posBall.y -= velocity.y;
					velocity.y = -velocity.y;
				}
				velocity.x = -abs(velocity.x);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x + longSide, center.y - shortSide))) {	//up-right-2
			if (map->collisionPoint(glm::ivec2(center.x + longSide, center.y - shortSide),0)) {	//up-right-2
				posBall.x -= velocity.x;
				if (map->checkCollision(glm::ivec2(center.x + longSide - velocity.x, center.y - shortSide))) {
					posBall.y -= velocity.y;
					velocity.y = -velocity.y;
				}
				velocity.x = -abs(velocity.x);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x + longSide, center.y + shortSide))) {	//right-down-1
			if (map->collisionPoint(glm::ivec2(center.x + longSide, center.y + shortSide),0)) {	//right-down-1
				posBall.x -= velocity.x;
				if (map->checkCollision(glm::ivec2(center.x + longSide - velocity.x, center.y + shortSide))) {
					posBall.y -= velocity.y;
					velocity.y = -velocity.y;
				}
				velocity.x = -abs(velocity.x);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x - longSide, center.y + shortSide))) {	//down-left
			if (map->collisionPoint(glm::ivec2(center.x - longSide, center.y + shortSide),0)) {	//down-left
				posBall.x -= velocity.x;
				if (map->checkCollision(glm::ivec2(center.x - longSide - velocity.x, center.y + shortSide))) {
					posBall.y -= velocity.y;
					velocity.y = -velocity.y;
				}
				velocity.x = abs(velocity.x);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x - longSide, center.y - shortSide))) {	//left-up
			if (map->collisionPoint(glm::ivec2(center.x - longSide, center.y - shortSide),0)) {	//left-up
				posBall.x -= velocity.x;
				if (map->checkCollision(glm::ivec2(center.x - longSide - velocity.x, center.y - shortSide))) {
					posBall.y -= velocity.y;
					velocity.y = -velocity.y;
				}
				velocity.x = abs(velocity.x);
			}
		}
		else if (collisionPlayer(posPlayer)) {
			velocity.y = -abs(velocity.y);
		}
	}
	else {
		if (!PlayGameState::instance().getGodMode()) {
			velocity.x = 1;
			velocity.y = -5;
		}
		else {
			velocity.x = 0;
			velocity.y = -6;
		}
		posBall = posPlayer + glm::vec2(8, -22); //Posicio inicial de la pilota sobre la barra del jugador
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
	velocity.x *= increase;
	velocity.y *= increase;
}

glm::vec2 Ball::getVelocity()
{
	return velocity;
}

void Ball::setVelocity(glm::vec2 velocity)
{
	this->velocity.x = velocity.x;
	this->velocity.y = velocity.y;
}

void Ball::setPosition(const glm::vec2& pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

bool Ball::collisionPlayer(const glm::ivec2& posPlayer)
{
	if (velocity.y >= -0.5) {

		float ball_size = 22.0;
		float playerWidth = 38.0;
		float pxmin = posPlayer.x;
		float pxmax = posPlayer.x + playerWidth;
		float pxcenter = posPlayer.x + playerWidth / 2.0;
		float py = posPlayer.y;

		float movement = abs(velocity.x) + abs(velocity.y);

		if (center.y + radi >= py && center.y + radi <= py + 10) {
			if (center.x >= pxmin && center.x <= pxmax) {
				if (!PlayGameState::instance().getGodMode()) {
					float moveX = (abs(pxcenter - center.x) / (playerWidth / 2)) * 0.9;
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
					float moveX = (abs(pxcenter - (center.x + shortSide)) / (playerWidth / 2)) * 0.9;
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
					float moveX = (abs(pxcenter - (center.x - shortSide)) / (playerWidth / 2)) * 0.9;
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
					float moveX = (abs(pxcenter - (center.x + longSide)) / (playerWidth / 2)) * 0.9;
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
					float moveX = (abs(pxcenter - (center.x - longSide)) / (playerWidth / 2)) * 0.9;
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
