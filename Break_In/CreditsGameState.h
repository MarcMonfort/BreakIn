#ifndef _CREDITSGAMESTATE_INCLUDE
#define _CREDITSGAMESTATE_INCLUDE

#include "GameState.h"
#include "ShaderProgram.h"
#include "Sprite.h"
#include "SoundManager.h"


class CreditsGameState : public GameState
{

public:
	static CreditsGameState& instance()
	{
		static CreditsGameState M;
		return M;
	}

	void init();
	void update(int deltaTime);
	void render();

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

private:
	void initShaders();

	Sprite* background;
	Texture spritesheet;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
};

#endif
