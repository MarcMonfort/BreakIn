#include "Storm.h"

#define SCREEN_X 32 //tiene que se igual al de PlayGameState.cpp
#define SCREEN_Y 48

void Storm::init(TileMap* tileMap, ShaderProgram& shaderProgram)
{

	for (int i = 0; i < 30; ++i) {
		Drop* drop = new Drop(); //usar punteros?
		drop->init(glm::ivec2(SCREEN_X, SCREEN_Y), shaderProgram);
		int posX = (rand() % 21) + 1;
		drop->setPosition(glm::vec2(posX * tileMap->getTileSize(), 50));
		drop->setTileMap(tileMap);
		drops.push_back(drop);
	}

	lightning = new Lightning();
	lightning->init(glm::ivec2(SCREEN_X, SCREEN_Y), shaderProgram);
	lightning->setPosition(glm::vec2(50, 16));

	currentTime = 0;
}


void Storm::update(int deltaTime)
{
	currentTime += deltaTime;


	for (int i = 0; i < 30; ++i) {
		drops[i]->update(deltaTime, glm::vec2(50, 50));
	}

	lightning->update(deltaTime);
}

void Storm::render()
{

	for (int i = 0; i < 30; ++i) {
		drops[i]->render();
	}

	lightning->render();
}