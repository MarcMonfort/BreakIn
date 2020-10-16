#ifndef _PLAYGAMESTATE_INCLUDE
#define _PLAYGAMESTATE_INCLUDE


#include <glm/glm.hpp>
#include "GameState.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Level.h"


// PlayGameState contains all the entities of our game.
// It is responsible for updating and render them.


class PlayGameState : public GameState
{

public:

	static PlayGameState& instance()
	{
		static PlayGameState P;

		return P;
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

private:
	//TileMap* map;
	Player* player;
	Ball* ball;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	vector<Level*> levels;	// como conservar level?
	//Level* level;
	int currentLevel;


};


#endif // _SCENE_INCLUDE


