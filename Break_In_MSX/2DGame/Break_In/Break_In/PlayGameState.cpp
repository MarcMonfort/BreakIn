#include "PlayGameState.h"

#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <GL\freeglut_std.h>


#define SCREEN_X 32
#define SCREEN_Y 16

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

#define LAST_LEVEL 2





void PlayGameState::init()
{
	currentLevel = 0; // = 0!!
	Level* first = new Level();
	first->createLevel(currentLevel+1);
	levels.push_back(first);
}

void PlayGameState::update(int deltaTime)
{
	currentTime += deltaTime;
	levels[currentLevel]->update(deltaTime);
}

void PlayGameState::render()
{
	levels[currentLevel]->render();
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
		}
		
	}
	else if (key == 'b')
	{
		if (currentLevel > 0) {
			currentLevel -= 1;
		}
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



