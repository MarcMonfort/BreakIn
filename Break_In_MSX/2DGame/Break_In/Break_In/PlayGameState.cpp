#include "PlayGameState.h"

#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include <GL\freeglut_std.h>
#include "GameOverGameState.h"


#define SCREEN_X 32
#define SCREEN_Y 48

#define INIT_PLAYER_X_TILES 11
#define INIT_PLAYER_Y_TILES 20.5

#define NUM_MAPS 3
#define NUM_LEVELS 3

#define INIT_BALL_X_TILES 11
#define INIT_BALL_Y_TILES 21.5



void PlayGameState::init()
{
	initShaders();

	currentMap = 0; // = 0!!
	previousMap = 0;
	currentLevel = 1; //se podria enviar por parametro en init()

	Level* first = new Level();
	first->createLevel(currentLevel, currentMap+1);
	levels.push_back(first);
	levels[currentMap]->setTransition(0);

	spritesheet.loadFromFile("images/counters.png", TEXTURE_PIXEL_FORMAT_RGBA);
	counters = Sprite::createSprite(glm::ivec2(384, 384), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	counters->setPosition(glm::vec2(450 - 33, 48 + 20));  //Nombre arbitrari (a ojo), potser caldria calcular-lo

	godMode_spritesheet.loadFromFile("images/godmode.png", TEXTURE_PIXEL_FORMAT_RGBA);
	godMode_sprite = Sprite::createSprite(glm::ivec2(128, 16), glm::vec2(1.f, 1.f), &godMode_spritesheet, &texProgram);
	godMode_sprite->setPosition(glm::vec2(160, 445));


	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * levels[currentMap]->getMap()->getTileSize(), INIT_PLAYER_Y_TILES * levels[currentMap]->getMap()->getTileSize()));
	player->setTileMap(levels[currentMap]->getMap());

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_BALL_X_TILES * levels[currentMap]->getMap()->getTileSize(), INIT_BALL_Y_TILES * levels[currentMap]->getMap()->getTileSize()));
	ball->setTileMap(levels[currentMap]->getMap());

	int x = 450 + 124 + 2; //mida camp de joc + mida imatge counters + offset
	int y = 0;

	money = 0;
	y += 65 + 20;
	moneyDisplay = new NumDisplay();
	moneyDisplay->init(7, x, y, 0); //nombre de digits, coordenada x, coordenada y, tipus

	points = 0;
	y += 78;
	pointsDisplay = new NumDisplay();
	pointsDisplay->init(7, x, y, 0);

	lives = 4;
	y += 78;
	livesDisplay = new NumDisplay();
	livesDisplay->init(2, x, y, 0);

	bank = 1;
	y += 78;
	bankDisplay = new NumDisplay();
	bankDisplay->init(2, x, y, 0);

	room = 1;
	y += 78;
	roomDisplay = new NumDisplay();
	roomDisplay->init(2, x, y, 0);

	animation = new Animation();
	animation->init();

	victory = new Victory();
	victory->init();

	started = false;
	countStarted = 0;

	isDead = false;
	bAnim = false;
	bVict = false;
	ALL_DEAD = false;
	godMode = false;

	resetKeys();
}

void PlayGameState::update(int deltaTime)
{
	currentTime += deltaTime;

	
	
	if (levels[currentMap]->getMap()->noMoneyLeft()) {
		if (currentLevel < NUM_LEVELS)
			nextLevel();
		else
			bAnim = true;
			bVict = true;
	}

	if (!bAnim)
	{
		if (!started) {
			countStarted += deltaTime;
			if (countStarted > 3000) {
				started = true;
			}
		}

		levels[currentMap]->update(deltaTime);

		if (upDownTime > 0) {
			levels[previousMap]->update(deltaTime);
			upDownTime -= deltaTime;
		}

		player->update(deltaTime, ball->getPosition());

		if (!isDead) {
			glm::vec2 posPlayer = player->getPosition();
			ball->update(deltaTime, posPlayer);
		}
	}
	else 
	{
		if (!bVict)
			animation->update(deltaTime);
		else
			victory->update(deltaTime);

		if (points >= 9) {
			money += 9;
			points -= 9;
		}
		else if (points > 0) {
			endPointMoneyTransition();
		}
	}

	moneyDisplay->displayNum(money);
	pointsDisplay->displayNum(points);
	livesDisplay->displayNum(lives);
	bankDisplay->displayNum(bank);
	roomDisplay->displayNum(room);

	if (godMode)
	{
		godMode_sprite->update(deltaTime);
	}

	if (GAME_END) {
		deleteAll();
		MenuGameState::instance().init();
		Game::instance().popGameState();
		Game::instance().pushGameState(&MenuGameState::instance());
		Game::instance().setBestBreakIn(money);
	}
	else if (ALL_DEAD) {
		deleteAll();
		GameOverGameState::instance().init();
		Game::instance().popGameState();
		Game::instance().pushGameState(&GameOverGameState::instance());
		Game::instance().setBestBreakIn(money);
	}
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

	if (!bAnim)
	{
		if (upDownTime > 0) {
			levels[previousMap]->render();
		}
		levels[currentMap]->render();


		player->render(); //creo que es mejor que este render lo haga el Level
		ball->render();		//con una funcion setPlayer(Player* player)
	}
	else {
		if (!bVict)
			animation->render();
		else
			victory->render();
	}

	counters->render();
	moneyDisplay->render();
	pointsDisplay->render();
	livesDisplay->render();
	bankDisplay->render();
	roomDisplay->render();

	if (godMode) {
		godMode_sprite->render();
	}
}

void PlayGameState::deleteLevels() {
	for (int i = 0; i < levels.size(); ++i) {
		levels[i]->deleteALL();
		delete levels[i];
	}
	levels.clear();
}

void PlayGameState::nextMap()
{
	levels[currentMap]->setTransition(3);
	levels[currentMap]->resetGuard();
	levels[currentMap]->setMusic(false);
	previousMap = currentMap;
	upDownTime = 200;
	currentMap += 1;
	room += 1;
	if (levels.size() <= currentMap)
	{
		Level* newMap = new Level();  //recordar liberar espacio delete()
		newMap->createLevel(currentLevel, currentMap + 1);
		levels.push_back(newMap);
	}
	levels[currentMap]->setTransition(0);
	levels[currentMap]->setMusic(true);

	player->setTileMap(levels[currentMap]->getMap());
	ball->setPosition(glm::vec2(ball->getPosition().x, INIT_BALL_Y_TILES * levels[currentMap]->getMap()->getTileSize()));

	glm::vec2 velocity = ball->getVelocity();
	velocity.y = -abs(velocity.y);
	ball->setVelocity(velocity);
	ball->setTileMap(levels[currentMap]->getMap());
}

void PlayGameState::lastMap()
{
	if (currentMap > 0) {
		levels[currentMap]->setTransition(2);
		levels[currentMap]->resetGuard();
		levels[currentMap]->setMusic(false);
		previousMap = currentMap;
		upDownTime = 200;
		currentMap -= 1;
		levels[currentMap]->setTransition(1);
		levels[currentMap]->setMusic(true);

		player->setTileMap(levels[currentMap]->getMap());
		ball->setTileMap(levels[currentMap]->getMap());

		room -= 1;
		player->setTileMap(levels[currentMap]->getMap());
		ball->setPosition(glm::vec2(ball->getPosition().x, 1.5 * levels[currentMap]->getMap()->getTileSize()));

		glm::vec2 velocity = ball->getVelocity();
		velocity.y = abs(velocity.y);
		ball->setVelocity(velocity);
		ball->setTileMap(levels[currentMap]->getMap());
	}
	else {
		lost_life();
	}
}


void PlayGameState::nextLevel() {
	bAnim = true;
	lives += 1;
	animation->restart();
	setLevel(currentLevel+1);
}

void PlayGameState::stopAnimation() {
	bAnim = false;
	animation->stopMusic();
	endPointMoneyTransition();
}

void PlayGameState::lost_life() {
	if (lives > 0) {
		if (!godMode)
			lives -= 1;
		player->dead();
		isDead = true;
	}
	else {
		ALL_DEAD = true;
	}
}

void PlayGameState::setIsDead(bool dead) {
	this->isDead = dead;
}

void PlayGameState::deleteAll() {
	deleteLevels();
	delete ball;
	delete player;
	delete moneyDisplay;
	delete pointsDisplay;
	delete livesDisplay;
	delete bankDisplay;
	delete roomDisplay;
	delete animation;
	delete victory;
}

void PlayGameState::setLevel(int level) {
	if (currentLevel != level) {
		levels[currentMap]->setMusic(false);
		deleteLevels();
		currentLevel = level;
		currentMap = 0;
		previousMap = 0;
		room = 1;
		bank = level;

		Level* first = new Level();
		first->createLevel(currentLevel, currentMap + 1);
		levels.push_back(first);
		levels[currentMap]->setTransition(0);

		player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * levels[currentMap]->getMap()->getTileSize(), INIT_PLAYER_Y_TILES * levels[currentMap]->getMap()->getTileSize()));
		player->setTileMap(levels[currentMap]->getMap());
		ball->setPosition(glm::vec2(INIT_BALL_X_TILES * levels[currentMap]->getMap()->getTileSize(), INIT_BALL_Y_TILES * levels[currentMap]->getMap()->getTileSize()));
		ball->setTileMap(levels[currentMap]->getMap());

		started = false;
		countStarted = 0;

	}
}

glm::vec2 PlayGameState::getPlayerPosition() {
	return player->getPosition();
}

void PlayGameState::setAlarm(bool alarm) {
	levels[currentMap]->setAlarm(alarm);
}

void PlayGameState::keyPressed(int key)
{
	if (key == 27) // Escape code
	{
		if (bVict)
			winGame();
		else if (bAnim)
			stopAnimation();
		else
		{
			endPointMoneyTransition();
			Game::instance().setBestBreakIn(money);

			deleteAll();
			MenuGameState::instance().init();
			Game::instance().popGameState();
			Game::instance().pushGameState(&MenuGameState::instance());
		}			
	}
	else if (key == 'n' || key == 'N')
	{
		if (bVict)
			winGame();
		else if (bAnim)
			stopAnimation();
		else
		{
			if (currentMap < NUM_MAPS - 1)
				nextMap();
			else if (currentLevel < NUM_LEVELS)
				nextLevel();
			else {
				bAnim = true;
				bVict = true;
				victory->restart();
			}
		}
	}
	else if (key == 'b' || key == 'B')
	{
		if (currentMap > 0)
			lastMap();
		else if (currentLevel > 1)
			setLevel(currentLevel-1);

		if (bAnim) {
			stopAnimation();
			if (bVict) {
				bVict = false;
				victory->stopMusic();
			}
		}
	}

	else if (key == 'v')
	{
		ball->addVelocity(1.25);
	}
	else if (key == 'f')
	{
		ball->addVelocity(0.8);
	}
	else if (key == '1' || key == '2' || key == '3')
	{
		setLevel(key-'0');
	}
	else if (key == ' ')
	{
		if (bVict)
			winGame();
		else if (bAnim)
			stopAnimation();
		else
			started = true;
	}

	else if (key == 'd')
	{
		player->addVelocity(0.8);
	}

	else if (key == 'c')
	{
		player->addVelocity(1.25);
	}

	else if (key == 'g' || key == 'G')
	{
		godMode = !godMode;
	}
	
	keys[key] = true;
}

bool PlayGameState::getGodMode()
{
	return godMode;
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

void PlayGameState::addPoints(int addedPoints)
{
	points += addedPoints;
}

void PlayGameState::addMoney(int addedMoney)
{
	money += addedMoney;
}

bool PlayGameState::getStarted() {
	return started;
}

void PlayGameState::setStarted(bool b) {
	started = b;
	if (!started)
		countStarted = 0;
}

void PlayGameState::endPointMoneyTransition() {
	money += points;
	points = 0;
}

void PlayGameState::winGame() {

	bAnim = false;
	bVict = false;
	victory->stopMusic();
	endPointMoneyTransition();
	GAME_END = true;
}
