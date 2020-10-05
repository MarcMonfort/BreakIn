#ifndef _MENUGAMESTATE_INCLUDE
#define _MENUGAMESTATE_INCLUDE

#include "GameState.h"
#include "ShaderProgram.h"
#include "Sprite.h"




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


	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _GAME_INCLUDE
