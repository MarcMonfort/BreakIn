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

void Level::createLevel(int numLevel, int numMap)
{
	initShaders();
	string pathLevel = "levels/level" + to_string(numLevel) + to_string(numMap) +  ".txt";
	map = TileMap::createTileMap(pathLevel, glm::vec2(SCREEN_X, SCREEN_Y), texProgram);

	spritesheet.loadFromFile(map->getBackgroundFile(), TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::ivec2(22*16, 23*16), glm::vec2(11.f, 11.f), &spritesheet, &texProgram);
	background->setPosition(glm::vec2(SCREEN_X+16, SCREEN_Y+16));

	if (map->hasRing())
	{
		bRing = true;
		ring = new Ring();
		ring->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		ring->setPosition(map->getRingPosition());

		guard = new Guard();
		guard->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		guard->setPosition(glm::vec2(SCREEN_X-8, SCREEN_Y + 312));
	}

	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	soundManager = Game::instance().getSoundManager();
	music_alarm = soundManager->loadSound("sounds/alarm.mp3", FMOD_LOOP_NORMAL | FMOD_CREATESTREAM);
	musicOn = false;

	//drop = new Drop(); //usar punteros?
	//drop->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	//drop->setPosition(glm::vec2(INIT_BALL_X_TILES * map->getTileSize(), INIT_BALL_Y_TILES * map->getTileSize()));
	//drop->setTileMap(map);

	for (int i = 0; i < 10; ++i) {
		Drop* drop = new Drop(); //usar punteros?
		drop->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
		int posX = (rand() % 21)+1;
		drop->setPosition(glm::vec2(posX * map->getTileSize(), 50));
		drop->setTileMap(map);
		drops.push_back(drop);
	}

	lightning = new Lightning();
	lightning->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	lightning->setPosition(glm::vec2(50, 16));

	if (map->hasCloud())
	{
		bCloud = false;
		bStorm = false;
		cloudCounter = 0;
		cloud = new Cloud();
		cloud->init();

		int minCounterLimit = 15000; //15s
		int maxCounterLimit = 60000; //60s
		std::random_device rand_dev;
		std::mt19937 generator(rand_dev());
		std::uniform_int_distribution<int> distr(minCounterLimit, maxCounterLimit);
		cloudCounterLimit = distr(generator);
	}
}

void Level::update(int deltaTime)
{
	currentTime += deltaTime;
	if (transTime > 0)
		transTime -= deltaTime;

	if (!bAlarm && bRing && map->alarmOn())
	{
		
		bAlarm = true;
		resetGuard();
		setMusic(true);
	}

	if (bAlarm) {
		ring->update(deltaTime);
		guard->update(deltaTime);

	}

	if (map->hasCloud())
	{
		if (bCloud) {
			cloud->update(deltaTime);
		}
		else {
			cloudCounter += deltaTime;
			if (cloudCounter > cloudCounterLimit) {
				bCloud = true;
				cloudCounter = 0;
			}
		}
	}


	//drop->update(deltaTime, glm::vec2(50, 50));
	for (int i = 0; i < 10; ++i) {
		drops[i]->update(deltaTime, glm::vec2(50, 50));
	}

	lightning->update(deltaTime);
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

	if (bRing) {
		ring->setPosition(glm::vec2(map->getRingPosition().x, map->getRingPosition().y + transY));
		ring->render();

		if (bAlarm) {
			glm::vec2 aux = guard->getPosition();
			guard->setPosition(glm::vec2(guard->getPosition().x, guard->getPosition().y + transY));
			guard->render();
			guard->setPosition(aux);
		}
	}

	//drop->render();
	for (int i = 0; i < 10; ++i) {
		drops[i]->render();
	}

	lightning->render();

	if (map->hasCloud() && bCloud) {
		if (transY == 0)
			cloud->render();
	}
}

void Level::resetGuard()
{
	if (bAlarm) {
		guard->setPosition(glm::vec2(SCREEN_X - 8, SCREEN_Y + 312));
		guard->reset();
	}
}


void Level::setMusic(bool music)
{
	if (bAlarm) {
		if (music) {
			channel = soundManager->playSound(music_alarm);
		}
		else {
			channel->stop();
		}
	}
}

void Level::setAlarm(bool alarm)
{
	bAlarm = alarm;
	map->setAlarm(alarm);
	if (alarm == false) {
		//setMusic(false);
		channel->stop();
	}
}

void Level::setTransition(int transition)
{
	this->transition = transition;
	transTime = 200;
}

void Level::deleteALL()
{
	if (bRing) {
		delete ring;
		delete guard;
		if (bAlarm) {
			channel->stop();
		}
	}
	
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

void Level::cloud_taken() {
	bCloud = false;
	bStorm = true;
	cloud->reinitCloud();
}