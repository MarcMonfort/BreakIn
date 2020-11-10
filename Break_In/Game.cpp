#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	initShaders();
	spritesheet.loadFromFile("images/upc_fib_logo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	upc_fib_logo = Sprite::createSprite(glm::ivec2(96, 24), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	upc_fib_logo->setPosition(glm::vec2(15, 445));

	bPlay = true;
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	states.push(&MenuGameState::instance());
	states.top()->init();

	bestBreakIn = 0;
}

bool Game::update(int deltaTime)
{
	states.top()->update(deltaTime);
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	states.top()->render();
	upc_fib_logo->render();
}

GameState* Game::getGameState()
{
	return states.top();
}

void Game::popGameState()
{
	states.pop();
}

void Game::pushGameState(GameState* state)
{
	states.push(state);
}

void Game::setBplay(bool b)
{
	bPlay = b;
}

const SoundManager* Game::getSoundManager() const {
	return &soundManager;
}

void Game::initShaders()
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

int Game::getBestBreakIn()
{
	return bestBreakIn;
}

void Game::setBestBreakIn(int best)
{
	if (best > bestBreakIn)
		bestBreakIn = best;
}
