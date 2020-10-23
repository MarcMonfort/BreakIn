#include "Level.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"



#define SCREEN_X 32 //tiene que se igual al de PlayGameState.cpp
#define SCREEN_Y 48

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

#define INIT_BALL_X_TILES 5
#define INIT_BALL_Y_TILES 5

void Level::createLevel(int numLevel)
{
	initShaders();
	string pathLevel = "levels/level0" + to_string(numLevel) + ".txt";
	map = TileMap::createTileMap(pathLevel, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	spritesheet.loadFromFile(map->getBackgroundFile(), TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::ivec2(22*16, 23*16), glm::vec2(11.f, 11.f), &spritesheet, &texProgram);
	background->setPosition(glm::vec2(SCREEN_X+16, SCREEN_Y+16));


	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

}

void Level::update(int deltaTime)
{
	currentTime += deltaTime;
	if (transTime > 0)
		transTime -= deltaTime;

}

void Level::render()
{


	glm::mat4 modelview;

	texProgram.use();

	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);


	modelview = glm::mat4(1.0f);
	float transY = 0;
	if (transTime > 0)
	{
		switch (transition)
		{
			case upCenter:
				transY = -(2 * transTime);
				modelview = glm::translate(modelview, glm::vec3(0.f, -(2*transTime), 0.0f));
				break;
			case downCenter:
				transY = 2 * transTime;
				modelview = glm::translate(modelview, glm::vec3(0.f, 2* transTime, 0.0f));
				break;
			case centerUp:
				transY = -2 * (200 - transTime);
				modelview = glm::translate(modelview, glm::vec3(0.f, -2 * (200 - transTime), 0.0f));
				break;
			case centerDown:
				transY = 2 * (200 - transTime);
				break;
		}
	}
	background->setPosition(glm::vec2(SCREEN_X+16, SCREEN_Y+16+transY));
	background->render();

	modelview = glm::translate(modelview, glm::vec3(0.f, transY, 0.0f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	map->render();


}

void Level::setTransition(int transition)
{
	this->transition = transition;
	transTime = 200;
}


TileMap* Level::getMap()
{
	return map;
}

void Level::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}