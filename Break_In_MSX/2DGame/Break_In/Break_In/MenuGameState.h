#ifndef _MENUGAMESTATE_INCLUDE
#define _MENUGAMESTATE_INCLUDE

#include "GameState.h"
#include "ShaderProgram.h"
#include "Sprite.h"
#include "SoundManager.h"
#include "NumDisplay.h"



class MenuGameState : public GameState
{

public:
	static MenuGameState& instance()
	{
		static MenuGameState M;

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

	NumDisplay* bestBreakInDisplay;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	const SoundManager* soundManager;
	FMOD::Sound* music;
	FMOD::Channel* channel;

};


#endif // _GAME_INCLUDE
