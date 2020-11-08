#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "NumDisplay.h"


void NumDisplay::init(int nDigits, int x, int y, int type) {
	initShaders();
	createDigits(nDigits, type);
	numDigits = nDigits;
	setPosition(glm::vec2(x, y), type);
}

void NumDisplay::displayNum(int num) {
	/*int i = 0;
	while (num > 0) {
		int digit = num % 10;
		num = num / 10;
		digits[i]->changeAnimation(digit);
		i += 1;
	}*/

	for (int i = 0; i < numDigits; ++i) {
		int digit = num % 10;
		num = num / 10;
		digits[i]->changeAnimation(digit);
	}
}

void NumDisplay::render()
{
	int nDigits = digits.size();
	for (int i = 0; i < nDigits; ++i) {
		digits[i]->render();
	}
}

void NumDisplay::createDigits(int nDigits, int type)
{
	for (int i = 0; i < nDigits; ++i) {
		digits.push_back(createDigit(type));
	}
}

Sprite* NumDisplay::createDigit(int type)
{
	glm::vec2 quadSize;
	glm::vec2 sizeInSpritesheet;

	if (type == 0) {
		spritesheet.loadFromFile("images/numbers.png", TEXTURE_PIXEL_FORMAT_RGBA);
		quadSize = glm::vec2(14, 14);
		sizeInSpritesheet = glm::vec2(16. / 128., 16. / 128.);
	}
	else {
		spritesheet.loadFromFile("images/numbers2.png", TEXTURE_PIXEL_FORMAT_RGBA);
		quadSize = glm::vec2(16, 16);
		sizeInSpritesheet = glm::vec2(16. / 128., 16. / 128.);
	}

	Sprite* digitSprite = Sprite::createSprite(quadSize, sizeInSpritesheet, &spritesheet, &texProgram);
	digitSprite->setNumberAnimations(10);
	for (int i = 0; i < 10; ++i) {
		if (type == 0)
			digitSprite->addKeyframe(i, numPositions[i]);
		else
			digitSprite->addKeyframe(i, numPositions2[i]);
	}
	digitSprite->changeAnimation(0);

	return digitSprite;
}

void NumDisplay::setPosition(glm::vec2 position, int type)
{
	int nDigits = digits.size();
	for (int i = 0; i < nDigits; ++i) {
		float dx;
		if (type == 0)
			dx = -14 * (i + 1) - 2 * i; //- mida_digit*(i-1) - separacio_entre_digits*i
		else
			dx = -16 * (i + 1) - 2 * i; //- mida_digit*(i-1) - separacio_entre_digits*i
		digits[i]->setPosition(position + glm::vec2(dx, 0.0));
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