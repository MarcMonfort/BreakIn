#ifndef _NUMDISPLAY_INCLUDE
#define _NUMDISPLAY_INCLUDE

#include <vector>
#include "Texture.h"
#include "Sprite.h"
#include "ShaderProgram.h"

class NumDisplay
{

public:
	void init(int nDigits, int y);
	void displayNum(int num);
	void render();	

private:
	void initShaders();
	void createDigits(int nDigits);
	Sprite* createDigit();
	void setPosition(glm::vec2 position);

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
};

#endif // _NUMDISPLAY_INCLUDE