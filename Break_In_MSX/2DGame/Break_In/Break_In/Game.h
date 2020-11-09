#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE

#include <stack>


#include "GameState.h"
#include "MenuGameState.h"
#include "SoundManager.h"


#define SCREEN_WIDTH 640  //640
#define SCREEN_HEIGHT 480  //480


// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

public:
	Game() {}
	
	
	static Game &instance()
	{
		static Game G;
	
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();

	GameState* getGameState();
	void popGameState();
	void pushGameState(GameState* state);

	void setBplay(bool b);
	
	
	const SoundManager* getSoundManager() const;

	int getBestBreakIn();
	void setBestBreakIn(int best);
	
	// Input callback methods
	/*void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);*/
	
	/*bool getKey(int key) const;
	bool getSpecialKey(int key) const;*/

private:
	void initShaders();

	Sprite* upc_fib_logo;
	Texture spritesheet;

	ShaderProgram texProgram;

	int bestBreakIn = 0;

	bool bPlay;                       // Continue to play game?

	stack <GameState*> states;

	SoundManager soundManager;
};


#endif // _GAME_INCLUDE


