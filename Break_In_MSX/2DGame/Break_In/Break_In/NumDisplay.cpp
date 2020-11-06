#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "NumDisplay.h"


void NumDisplay::init(int nDigits, int y) {
	initShaders();
	createDigits(nDigits);
	setPosition(glm::vec2(0, y));
}

void NumDisplay::displayNum(int num) {
	int i = 0;
	while (num > 0) {
		int digit = num % 10;
		num = num / 10;
		digits[i]->changeAnimation(digit);
		i += 1;
	}
}

void NumDisplay::render()
{
	int nDigits = digits.size();
	for (int i = 0; i < nDigits; ++i) {
		digits[i]->render();
	}
}

void NumDisplay::createDigits(int nDigits)
{
	for (int i = 0; i < nDigits; ++i) {
		digits.push_back(createDigit());
	}
}

Sprite* NumDisplay::createDigit()
{
	spritesheet.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
	Sprite* digitSprite = Sprite::createSprite(glm::ivec2(14, 14), glm::vec2(16. / 128., 16. / 128.), &spritesheet, &texProgram);

	digitSprite->setNumberAnimations(10);
	for (int i = 0; i < 10; ++i) {
		digitSprite->addKeyframe(i, numPositions[i]);
	}
	digitSprite->changeAnimation(0);

	return digitSprite;
}

void NumDisplay::setPosition(glm::vec2 position) //es podria passar nomes la y
{
	int nDigits = digits.size();
	for (int i = 0; i < nDigits; ++i) {
		float x = 450 + 124 + 2 - 14 * (i + 1) - 2 * i; //mida camp de joc + mida imatge counters + offset - mida_digit*(i-1) - separacio_entre_digits*i
		digits[i]->setPosition(position + glm::vec2(x, 0.0));
	}
}

void NumDisplay::initShaders()
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