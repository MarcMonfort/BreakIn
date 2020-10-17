#include "PlayGameState.h"

#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <GL\freeglut_std.h>


#define SCREEN_X 32
#define SCREEN_Y 48

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

#define LAST_LEVEL 3

#define INIT_BALL_X_TILES 12
#define INIT_BALL_Y_TILES 21



void PlayGameState::init()
{
	initShaders();

	currentLevel = 0; // = 0!!
	Level* first = new Level();
	first->createLevel(currentLevel+1);
	levels.push_back(first);

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * levels[currentLevel]->getMap()->getTileSize(), INIT_PLAYER_Y_TILES * levels[currentLevel]->getMap()->getTileSize()));
	player->setTileMap(levels[currentLevel]->getMap());

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_BALL_X_TILES * levels[currentLevel]->getMap()->getTileSize(), INIT_BALL_Y_TILES * levels[currentLevel]->getMap()->getTileSize()));
	ball->setTileMap(levels[currentLevel]->getMap());

}

void PlayGameState::update(int deltaTime)
{
	currentTime += deltaTime;
	levels[currentLevel]->update(deltaTime);

	player->update(deltaTime);
	ball->update(deltaTime);
}

void PlayGameState::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	levels[currentLevel]->render();
	player->render(); //creo que es mejor que este render lo haga el Level
	ball->render();		//con una funcion setPlayer(Player* player)
}

void PlayGameState::nextLevel()
{
	currentLevel += 1;
	if (levels.size() <= currentLevel)
	{
		Level* nextLevel = new Level();  //recordar liberar espacio delete()
		nextLevel->createLevel(currentLevel + 1);
		levels.push_back(nextLevel);
	}
	player->setTileMap(levels[currentLevel]->getMap());

	ball->setPosition(glm::vec2(ball->getPosition().x, INIT_BALL_Y_TILES * levels[currentLevel]->getMap()->getTileSize()));
	ball->setTileMap(levels[currentLevel]->getMap());
}

void PlayGameState::lastLevel()
{
	if (currentLevel > 0) {
		currentLevel -= 1;
		player->setTileMap(levels[currentLevel]->getMap());
		ball->setTileMap(levels[currentLevel]->getMap());
	}
	player->setTileMap(levels[currentLevel]->getMap());

	ball->setPosition(glm::vec2(ball->getPosition().x, 1 * levels[currentLevel]->getMap()->getTileSize()));
	ball->setTileMap(levels[currentLevel]->getMap());
}


void PlayGameState::keyPressed(int key)
{
	if (key == 27) // Escape code
	{
		Game::instance().popGameState(); //or better push so we dont loose the state??
	}
	else if (key == 'n')
	{
		if (currentLevel < LAST_LEVEL)
		{
			currentLevel += 1;
			if (levels.size() <= currentLevel)
			{
				Level* nextLevel = new Level();  //recordar liberar espacio delete()
				nextLevel->createLevel(currentLevel + 1);
				levels.push_back(nextLevel);	
			}
			player->setTileMap(levels[currentLevel]->getMap());
			ball->setTileMap(levels[currentLevel]->getMap());
		}
		
	}
	else if (key == 'b')
	{
		if (currentLevel > 0) {
			currentLevel -= 1;
			player->setTileMap(levels[currentLevel]->getMap());
			ball->setTileMap(levels[currentLevel]->getMap());
		}
	}

	else if (key == 'v')
	{
		ball->addVelocity(2);
	}
	else if (key == 'f')
	{
		ball->addVelocity(0.5);
	}
	
	keys[key] = true;
}

void PlayGameState::keyReleased(int key)
{
	keys[key] = false;
}

void PlayGameState::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void PlayGameState::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void PlayGameState::mouseMove(int x, int y)
{
}

void PlayGameState::mousePress(int button)
{
}

void PlayGameState::mouseRelease(int button)
{
}

void PlayGameState::initShaders()
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



