#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	//glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	states.push(&MenuGameState::instance());
	states.top()->init();
	//scene.init();
}

bool Game::update(int deltaTime)
{
	//scene.update(deltaTime);

	states.top()->update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//scene.render();
	states.top()->render();
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


/*void Game::keyPressed(int key)
{
	if(key == 27) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}*/

/*bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}*/





