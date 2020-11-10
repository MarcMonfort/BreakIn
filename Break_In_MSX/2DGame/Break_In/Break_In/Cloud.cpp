#include "Cloud.h"
#include <cmath>
#include <random>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"
#include "PlayGameState.h"
#include <glm/gtc/matrix_transform.hpp>


void Cloud::init()
{
	initShaders();
	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);

	spritesheet.loadFromFile("images/cloud_buff.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);

	waitTime = 2000;	//Temps que el Cloud es queda aturat quan arriba a goTo
	cloudVel = 2;		//Velocitat del Cloud

	reinitCloud();
}

void Cloud::reinitCloud()
{
	// Limits cloud: X in [50, 365], Y in [225, 382]
	xmin = 50;
	xmax = 365;
	ymin = 225;
	ymax = 382;

	std::random_device rand_dev;
	std::mt19937 generator(rand_dev());
	std::uniform_int_distribution<int> initX(0, 1);
	std::uniform_int_distribution<int> distrX(xmin, xmax);
	std::uniform_int_distribution<int> distrY(ymin, ymax);

	// Sempre comença a fora de la pantalla (x fora dels límits) a la dreta
	// o a l'esquerra aleatòriament, i a una y aleatòria dins dels límits
	bool bx = initX(generator);
	if (bx)
		posCloud.x = -40;
	else
		posCloud.x = 430;
	posCloud.y = distrY(generator);

	//goTo s'inicialitza a un punt aleatori dels límits del cloud a distància 30 en alguna direcció de posCloud
	int i = 0;
	do { goTo = glm::vec2(distrX(generator), distrY(generator)); ++i; }
	while (abs(goTo.x - posCloud.x) < 30 && abs(goTo.x - posCloud.x) < 30 && i < 10);

	counter = 0;
	sprite->setPosition(posCloud);
}

void Cloud::update(int deltaTime)
{
	if (posCloud == goTo) {
		counter += deltaTime;
		if (counter > waitTime) {
			std::random_device rand_dev;
			std::mt19937 generator(rand_dev());
			std::uniform_int_distribution<int> distrX(xmin, xmax);
			std::uniform_int_distribution<int> distrY(ymin, ymax);

			int i = 0;
			do { goTo = glm::vec2(distrX(generator), distrY(generator)); ++i; }
			while (abs(goTo.x - posCloud.x) < 30 && abs(goTo.x - posCloud.x) < 30 && i < 10);

			counter = 0;
		}
	}
	else {
		if (abs(posCloud.x - goTo.x) < cloudVel)
			posCloud.x = goTo.x;
		if (abs(posCloud.y - goTo.y) < cloudVel)
			posCloud.y = goTo.y;

		if (goTo.x > posCloud.x)
			posCloud.x += cloudVel;
		else if (goTo.x < posCloud.x)
			posCloud.x -= cloudVel;

		if (goTo.y > posCloud.y)
			posCloud.y += cloudVel;
		else if (goTo.y < posCloud.y)
			posCloud.y -= cloudVel;
	}

	posPlayer = PlayGameState::instance().getPlayerPosition();

	int Cxmin = posCloud.x;
	int Cxmax = posCloud.x + 32;
	int Cymin = posCloud.y;
	int Cymax = posCloud.y + 32;

	//SCREEN_X = 32 i SCREEN_Y = 48
	int Pxmin = posPlayer.x + 32;
	int Pxmax = posPlayer.x + 38 + 32;
	int Pymin = posPlayer.y + 48;
	int Pymax = posPlayer.y + 52 + 48;

	if ((Cxmin < Pxmax && Pxmin < Cxmax) && (Cymin < Pymax && Pymin < Cymax)) {
		PlayGameState::instance().cloud_taken();
	}

	sprite->setPosition(posCloud);
}

void Cloud::render()
{
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	sprite->render();
}

void Cloud::initShaders()
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
