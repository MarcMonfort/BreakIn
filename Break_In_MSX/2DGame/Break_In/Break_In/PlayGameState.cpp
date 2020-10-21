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
	levels[currentLevel]->setTransition(0);

	spritesheet.loadFromFile("images/counters.png", TEXTURE_PIXEL_FORMAT_RGBA);
	counters = Sprite::createSprite(glm::ivec2(2*62, 2*192), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	counters->setPosition(glm::vec2(450, 48));  //Nombre arbitrari (a ojo), potser caldria calcular-lo

	player = new Player();
	player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * levels[currentLevel]->getMap()->getTileSize(), INIT_PLAYER_Y_TILES * levels[currentLevel]->getMap()->getTileSize()));
	player->setTileMap(levels[currentLevel]->getMap());

	ball = new Ball();
	ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	ball->setPosition(glm::vec2(INIT_BALL_X_TILES * levels[currentLevel]->getMap()->getTileSize(), INIT_BALL_Y_TILES * levels[currentLevel]->getMap()->getTileSize()));
	ball->setTileMap(levels[currentLevel]->getMap());

	int y = 0;

	money = 0;
	y += 65;
	moneyDisplay = new NumDisplay();
	moneyDisplay->init(7, y); //nombre de digits, coordenada y

	points = 0;
	y += 80;
	pointsDisplay = new NumDisplay();
	pointsDisplay->init(7, y);

	lives = 0;
	y += 96;
	livesDisplay = new NumDisplay();
	livesDisplay->init(2, y);

	bank = 0;
	y += 64;
	bankDisplay = new NumDisplay();
	bankDisplay->init(2, y);

	room = 0;
	y += 112;
	roomDisplay = new NumDisplay();
	roomDisplay->init(2, y);

}

void PlayGameState::update(int deltaTime)
{
	currentTime += deltaTime;
	levels[currentLevel]->update(deltaTime);

	if (upDownTime > 0) {
		levels[previousLevel]->update(deltaTime);
	}

	if (upDownTime > 0) {
		upDownTime -= deltaTime;
	}

	player->update(deltaTime);
	glm::vec2 posPlayer = player->getPosition();
	ball->update(deltaTime, posPlayer);

	moneyDisplay->displayNum(money);
	pointsDisplay->displayNum(points);
	livesDisplay->displayNum(lives);
	bankDisplay->displayNum(bank);
	roomDisplay->displayNum(room);
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

	if (upDownTime > 0) {
		levels[previousLevel]->render();
	}
	levels[currentLevel]->render();

	counters->render();

	player->render(); //creo que es mejor que este render lo haga el Level
	ball->render();		//con una funcion setPlayer(Player* player)

	moneyDisplay->render();
	pointsDisplay->render();
	livesDisplay->render();
	bankDisplay->render();
	roomDisplay->render();
}

void PlayGameState::deleteLevels() {
	for (int i = 0; i < levels.size(); ++i) {
		delete levels[i];
	}
	levels.clear();
}

void PlayGameState::nextLevel()
{
	levels[currentLevel]->setTransition(3);
	previousLevel = currentLevel;
	upDownTime = 200;
	currentLevel += 1;
	if (levels.size() <= currentLevel)
	{
		Level* nextLevel = new Level();  //recordar liberar espacio delete()
		nextLevel->createLevel(currentLevel + 1);
		levels.push_back(nextLevel);
	}
	levels[currentLevel]->setTransition(0);
	player->setTileMap(levels[currentLevel]->getMap());
	ball->setPosition(glm::vec2(ball->getPosition().x, INIT_BALL_Y_TILES * levels[currentLevel]->getMap()->getTileSize()));
	ball->setTileMap(levels[currentLevel]->getMap());
}

void PlayGameState::lastLevel()
{
	if (currentLevel > 0) {
		levels[currentLevel]->setTransition(2);
		previousLevel = currentLevel;
		upDownTime = 200;
		currentLevel -= 1;
		levels[currentLevel]->setTransition(1);
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
		deleteLevels();
		Game::instance().popGameState(); //or better push so we dont loose the state??
	}
	else if (key == 'n')
	{
		if (currentLevel < LAST_LEVEL)
		{
			levels[currentLevel]->setTransition(3);
			previousLevel = currentLevel;
			upDownTime = 200;
			currentLevel += 1;
			if (levels.size() <= currentLevel)
			{
				Level* nextLevel = new Level();  //recordar liberar espacio delete()
				nextLevel->createLevel(currentLevel + 1);
				levels.push_back(nextLevel);	
			}
			levels[currentLevel]->setTransition(0);
			player->setTileMap(levels[currentLevel]->getMap());
			ball->setTileMap(levels[currentLevel]->getMap());
		}
		
	}
	else if (key == 'b')
	{
		if (currentLevel > 0) {
			levels[currentLevel]->setTransition(2);
			previousLevel = currentLevel;
			upDownTime = 200;
			currentLevel -= 1;
			levels[currentLevel]->setTransition(1);
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
	else if (key == 'q')
	{
		levels[currentLevel]->setTransition(1);
	}
	else if (key == 'w')
	{
		levels[currentLevel]->setTransition(3);
	}
	else if (key == 'd')
	{
		deleteLevels();
	}
	else if (key == 'm')
	{
		money += 1;
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



