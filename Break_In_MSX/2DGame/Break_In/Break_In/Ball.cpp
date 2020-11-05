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

	vX = 0; //4
	vY = 2;

	radi = 11+2;
	shortSide = 6; 
	longSide = 9;

}

void Ball::update(int deltaTime, glm::vec2 posPlayer)
{
	sprite->update(deltaTime);

	if (PlayGameState::instance().getStarted()) {

		/*posBall.y += vY;
		if (map->collisionMoveUp(posBall, glm::ivec2(22, 22))) {
			posBall.y -= vY;
			vY = abs(vY);
		}
		else if (map->collisionMoveDown(posBall, glm::ivec2(22, 22))) {
			posBall.y -= vY;
			vY = -abs(vY);
		}

		if (collisionPlayer(posPlayer)) {
			posBall.y -= vY;
			vY = -abs(vY);
		}

		posBall.x += vX;
		if (map->collisionMoveLeft(posBall, glm::ivec2(22, 22))) {
			posBall.x -= vX;
			vX = abs(vX);
		}
		else if (map->collisionMoveRight(posBall, glm::ivec2(22, 22))) {
			posBall.x -= vX;
			vX = -abs(vX);
		}*/
		posBall.x += vX;
		posBall.y += vY;

		center = glm::ivec2(posBall.x + 11, posBall.y + 11);

		if (map->checkCollision(glm::ivec2(center.x, center.y - radi))) {
			if (map->collisionPoint(glm::ivec2(center.x, center.y - radi))) {	//up
				posBall.y += vY;
				vY = abs(vY);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x, center.y + radi))) {
			if (map->collisionPoint(glm::ivec2(center.x, center.y + radi))) {	//down
				posBall.y += vY;
				vY = -abs(vY);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x - radi, center.y))) {
			if (map->collisionPoint(glm::ivec2(center.x - radi, center.y))) {	//left
				posBall.x -= vX;
				vX = abs(vX);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x + radi, center.y))) {	//right
			if (map->collisionPoint(glm::ivec2(center.x + radi, center.y))) {	//right
				posBall.x -= vX;
				vX = -abs(vX);
			}
		}

		 //4.21 - 10.16
		else if (map->checkCollision(glm::ivec2(center.x + shortSide, center.y - longSide))) {	//up-right-1
			if (map->collisionPoint(glm::ivec2(center.x + shortSide, center.y - longSide))) {	//up-right-1
				posBall.y += vY;
				vY = abs(vY);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x + longSide, center.y - shortSide))) {	//up-right-2
			if (map->collisionPoint(glm::ivec2(center.x + longSide, center.y - shortSide))) {	//up-right-2
				posBall.x -= vX;
				vX = -abs(vX);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x + longSide, center.y + shortSide))) {	//right-down-1
			if (map->collisionPoint(glm::ivec2(center.x + longSide, center.y + shortSide))) {	//right-down-1
				posBall.x -= vX;
				vX = -abs(vX);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x + shortSide, center.y + longSide))) {	//right-down-2
			if (map->collisionPoint(glm::ivec2(center.x + shortSide, center.y + longSide))) {	//right-down-2
				posBall.y += vY;
				vY = -abs(vY);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x - shortSide, center.y + longSide))) {	//down-left
			if (map->collisionPoint(glm::ivec2(center.x - shortSide, center.y + longSide))) {	//down-left
				posBall.y += vY;
				vY = -abs(vY);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x - longSide, center.y + shortSide))) {	//down-left
			if (map->collisionPoint(glm::ivec2(center.x - longSide, center.y + shortSide))) {	//down-left
				posBall.x -= vX;
				vX = abs(vX);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x - longSide, center.y - shortSide))) {	//left-up
			if (map->collisionPoint(glm::ivec2(center.x - longSide, center.y - shortSide))) {	//left-up
				posBall.x -= vX;
				vX = abs(vX);
			}
		}
		else if (map->checkCollision(glm::ivec2(center.x - shortSide, center.y - longSide))) {	//left-up
			if (map->collisionPoint(glm::ivec2(center.x - shortSide, center.y - longSide))) {	//left-up
				posBall.y += vY;
				vY = abs(vY);
			}
		}

		////7.788 square
		//else if (map->collisionPoint(glm::ivec2(center.x + 7.79, center.y -7.79))) {	//up-right
		//	vX = -vX;
		//	vY = -vY;
		//}
		//else if (map->collisionPoint(glm::ivec2(center.x + 7.79, center.y + 7.79))) {	//right-down
		//	vX = -vX;
		//	vY = -vY;
		//}
		//else if (map->collisionPoint(glm::ivec2(center.x - 7.79, center.y + 7.79))) {	//down-left
		//	vX = -vX;
		//	vY = -vY;
		//}
		//else if (map->collisionPoint(glm::ivec2(center.x - 7.79, center.y - 7.79))) {	//left-up
		//	vX = -vX;
		//	vY = -vY;
		//}

		else if (collisionPlayer(posPlayer)) {
			vY = -abs(vY);
		}

		

	}
	else {
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
	vX *= increase;
	vY *= increase;
}

void Ball::setPosition(const glm::vec2& pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

bool Ball::collisionPlayer(const glm::ivec2& posPlayer)
{
	if (vY >= 0) { //nomes si la pilota esta baixant

		float ball_size = 22.0; //es podria crear un atribut de la classe
		//float bxmin = posBall.x;
		//float bxmax = posBall.x + ball_size;
		float bxcenter = posBall.x + ball_size / 2.0;

		float bymin = posBall.y;
		float bymax = posBall.y + ball_size;

		float playerWidth = 38.0; //es podria passar per parametre
		float pxmin = posPlayer.x;
		float pxmax = posPlayer.x + playerWidth;
		float pxcenter = posPlayer.x + playerWidth / 2.0;
		float py = posPlayer.y;

		if (bymax >= py && bymin < py)
			//if ((bxmin >= pxmin && bxmin <= pxmax) || (bxmax >= pxmin && bxmax <= pxmax))
			if (bxcenter >= pxmin && bxcenter <= pxmax) {

				if (bxcenter < pxcenter)
					vX = -0.2 * (pxcenter - bxcenter);
				else if (bxcenter > pxcenter)
					vX = 0.2 * (bxcenter - pxcenter);
				else
					vX = 0;
				return true;
			}
	}
	
	return false;
}



