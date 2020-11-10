#ifndef _NUMDISPLAY_INCLUDE
#define _NUMDISPLAY_INCLUDE

#include <vector>
#include "Texture.h"
#include "Sprite.h"
#include "ShaderProgram.h"


class NumDisplay
{

public:
	void init(int nDigits, int x, int y, int type);
	void displayNum(int num);
	void render();	

private:
	void initShaders();
	void createDigits(int nDigits, int type);
	Sprite* createDigit(int type);
	void setPosition(glm::vec2 position, int type);

	vector<Sprite*> digits;
	int numDigits;

	Texture spritesheet;
	ShaderProgram texProgram;

	glm::vec2 numPositions[10] = {
		glm::vec2(16 * 0 / 128.f, 16 * 0 / 128.f),
		glm::vec2(16 * 1 / 128.f, 16 * 0 / 128.f),
		glm::vec2(16 * 2 / 128.f, 16 * 0 / 128.f),
		glm::vec2(16 * 3 / 128.f, 16 * 0 / 128.f),
		glm::vec2(16 * 4 / 128.f, 16 * 0 / 128.f),
		glm::vec2(16 * 5 / 128.f, 16 * 0 / 128.f),
		glm::vec2(16 * 6 / 128.f, 16 * 0 / 128.f),
		glm::vec2(16 * 7 / 128.f, 16 * 0 / 128.f),
		glm::vec2(16 * 0 / 128.f, 16 * 1 / 128.f),
		glm::vec2(16 * 1 / 128.f, 16 * 1 / 128.f)
	};

	glm::vec2 numPositions2[10] = {
		glm::vec2(((16 + 1) * 0 + 1) / 128.f, (16 * 0 + 1) / 128.f),
		glm::vec2(((16 + 1) * 1 + 1) / 128.f, (16 * 0 + 1) / 128.f),
		glm::vec2(((16 + 1) * 2 + 1) / 128.f, (16 * 0 + 1) / 128.f),
		glm::vec2(((16 + 1) * 3 + 1) / 128.f, (16 * 0 + 1) / 128.f),
		glm::vec2(((16 + 1) * 4 + 1) / 128.f, (16 * 0 + 1) / 128.f),
		glm::vec2(((16 + 1) * 5 + 1) / 128.f, (16 * 0 + 1) / 128.f),
		glm::vec2(((16 + 1) * 6 + 1) / 128.f, (16 * 0 + 1) / 128.f),
		glm::vec2(((16 + 1) * 0 + 1) / 128.f, (16 * 1 + 1) / 128.f),
		glm::vec2(((16 + 1) * 1 + 1) / 128.f, (16 * 1 + 1) / 128.f),
		glm::vec2(((16 + 1) * 2 + 1) / 128.f, (16 * 1 + 1) / 128.f)
	};
};

#endif
