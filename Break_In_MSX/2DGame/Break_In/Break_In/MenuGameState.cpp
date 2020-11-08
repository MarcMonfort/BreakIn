#include "MenuGameState.h"
#include <iostream>
#include <glm\gtc\matrix_transform.hpp>
#include "Game.h"
#include "PlayGameState.h"
#include "InstructionsGameState.h"
#include "CreditsGameState.h"



void MenuGameState::init()
{
	//glClearColor(1.f, 0.0f, 0.0f, 1.0f);

	initShaders();

	spritesheet.loadFromFile("images/menuBackground_v9.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::ivec2(SCREEN_WIDTH, SCREEN_HEIGHT), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	background->setPosition(glm::vec2(0, 0));

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	soundManager = Game::instance().getSoundManager();
	music = soundManager->loadSound("sounds/main_theme.mp3", FMOD_LOOP_NORMAL | FMOD_CREATESTREAM);
	channel = soundManager->playSound(music);
	channel->setVolume(1.0f);

	bestBreakInDisplay = new NumDisplay();
	bestBreakInDisplay->init(7, 510, 369, 1); //num. digits, coord x, coord y, tipus
}

void MenuGameState::update(int deltaTime)
{ 
	bestBreakInDisplay->displayNum(Game::instance().getBestBreakIn());
}

void MenuGameState::render()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	background->render();
	bestBreakInDisplay->render();
}


void MenuGameState::keyPressed(int key)
{
	if (key == 27) // Escape code
	{
		Game::instance().setBplay(false);
	}
	else if (key == 32) // Space code
	{
		channel->stop();
		PlayGameState::instance().init();
		Game::instance().popGameState(); //or better push so we dont loose the state??
		Game::instance().pushGameState(&PlayGameState::instance());
	}
	else if (key == 'i') {
		InstructionsGameState::instance().init();
		Game::instance().popGameState(); //or better push so we dont loose the state??
		Game::instance().pushGameState(&InstructionsGameState::instance());
	}
	else if (key == 'c') {
		CreditsGameState::instance().init();
		Game::instance().popGameState(); //or better push so we dont loose the state??
		Game::instance().pushGameState(&CreditsGameState::instance());
	}

	keys[key] = true;
}

void MenuGameState::keyReleased(int key)
{
	keys[key] = false;
}

void MenuGameState::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void MenuGameState::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void MenuGameState::mouseMove(int x, int y)
{
}

void MenuGameState::mousePress(int button)
{
}

void MenuGameState::mouseRelease(int button)
{
}



void MenuGameState::initShaders()
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
