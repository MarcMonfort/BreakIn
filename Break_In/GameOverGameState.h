#ifndef _GAMEOVERGAMESTATE_INCLUDE
#define _GAMEOVERGAMESTATE_INCLUDE

#include "GameState.h"
#include "ShaderProgram.h"
#include "Sprite.h"
#include "SoundManager.h"


class GameOverGameState : public GameState
{

public:
	static GameOverGameState& instance()
	{
		static GameOverGameState G;
		return G;
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

	const SoundManager* soundManager;
	FMOD::Sound* music;
	FMOD::Channel* channel;
};

#endif // _GAME_INCLUDE
