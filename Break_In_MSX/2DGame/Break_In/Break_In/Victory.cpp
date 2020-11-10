#include "Victory.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "PlayGameState.h"
#include <GL/glew.h>
#include <GL/glut.h>


#define SCREEN_X 32 //tiene que se igual al de PlayGameState.cpp
#define SCREEN_Y 48

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

void Victory::init()
{
	//glClearColor(1.f, 0.0f, 0.0f, 1.0f);

	initShaders();

	spritesheet.loadFromFile("images/victoryBackground.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::ivec2(24 * 16, 24 * 16), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	background->setPosition(glm::vec2(SCREEN_X, SCREEN_Y));

	thief = new Thief();
	thief->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	thief->setPosition(glm::vec2(60, 258));

	spritesheet2.loadFromFile("images/car.png", TEXTURE_PIXEL_FORMAT_RGBA);
	car = Sprite::createSprite(glm::ivec2(745/4, 281/4), glm::vec2(1.f, 1.f), &spritesheet2, &texProgram);
	car->setPosition(glm::vec2(225, 300));

	spritesheet3.loadFromFile("images/victory.png", TEXTURE_PIXEL_FORMAT_RGBA);
	victory = Sprite::createSprite(glm::ivec2(236, 34), glm::vec2(236.0/256.0, 34.0/256.0), &spritesheet3, &texProgram);
	victory->setPosition(glm::vec2(110, 400));

	victory->setNumberAnimations(1);
	victory->setAnimationSpeed(0, 1);
	victory->addKeyframe(0, glm::vec2(1.0/256.0, 1.0/256.0));
	victory->addKeyframe(0, glm::vec2(0.0, 0.5));
	victory->changeAnimation(0);

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	soundManager = Game::instance().getSoundManager();
	music_player = soundManager->loadSound("sounds/thief.mp3", FMOD_DEFAULT);
	coins = soundManager->loadSound("sounds/falling_coins_cut.mp3", FMOD_LOOP_NORMAL);
	car_door = soundManager->loadSound("sounds/car_door.mp3", FMOD_DEFAULT);
	car_engine = soundManager->loadSound("sounds/car_engine.mp3", FMOD_DEFAULT);

	counter = 0;

	started = false;
}

void Victory::restart()
{
	thief->setPosition(glm::vec2(60, 258));
	channel = soundManager->playSound(music_player);
	channel3 = soundManager->playSound(coins);
	channel3->setVolume(0.6);
	started = true;
}

void Victory::stopMusic()
{
	channel->stop();
	channel2->stop();
	channel3->stop();
	channel4->stop();
}

void Victory::update(int deltaTime)
{
	if (!started) {
		restart();
	}
	currentTime += deltaTime;

	victory->update(deltaTime);
	
	glm::vec2 carPos = car->getPosition();
	glm::vec2 thiefPos = thief->getPosition();

	if (carPos.x >= 500) {
		if (counter > 8000) {
			stopMusic();
			started = false;
			PlayGameState::instance().winGame();
		}
		if (counter > 5000)
			channel3->stop();
		counter += deltaTime;
	}
	else if (thiefPos.x >= 230) {

		if (!bDoor) {
			channel4 = soundManager->playSound(car_door);
			channel4->setVolume(1.5);
			bDoor = true;
		}
		
		if (counter > 3500) {
			thief->setPosition(thiefPos + glm::vec2(2, 0));
			car->setPosition(carPos + glm::vec2(2, 0));
		}
		else if (counter > 2500) {
			if (!bEngine) {
				channel2 = soundManager->playSound(car_engine);
				channel2->setVolume(0.5);
				bEngine = true;
			}
			if (bCar)
				car->setPosition(carPos + glm::vec2(-3, 0));
			else
				car->setPosition(carPos + glm::vec2(3, 0));
			bCar = !bCar;
		}
		counter += deltaTime;
	}
	else {
		thief->update(deltaTime);
	}
}

void Victory::render()
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
	victory->render();
	thief->render();
	car->render();
}

void Victory::initShaders()
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