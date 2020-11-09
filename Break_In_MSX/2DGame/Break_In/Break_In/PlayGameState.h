#ifndef _PLAYGAMESTATE_INCLUDE
#define _PLAYGAMESTATE_INCLUDE


#include <glm/glm.hpp>
#include "GameState.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Sprite.h"
#include "Player.h"
#include "Level.h"
#include "NumDisplay.h"
#include "Animation.h"
#include "Victory.h"


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

	void nextMap();
	void lastMap();

	void nextLevel();
	void stopAnimation();

	bool getStarted();
	void setStarted(bool b);

	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void specialKeyPressed(int key);
	void specialKeyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	void addPoints(int addedPoints);
	void addMoney(int addedMoney);

	glm::vec2 getPlayerPosition();

	void lost_life();
	void setIsDead(bool dead);

	void setAlarm(bool alarm);

	bool getGodMode();

	void endPointMoneyTransition();

	void winGame();


private:
	void initShaders();
	void deleteLevels();
	void deleteAll();
	void setLevel(int level);

private:
	//TileMap* map;
	Player* player;
	Ball* ball;

	Texture spritesheet;
	Sprite* counters;

	int money;
	NumDisplay* moneyDisplay;
	
	int points;
	NumDisplay* pointsDisplay;

	int lives;
	NumDisplay* livesDisplay;

	int bank;
	NumDisplay* bankDisplay;

	int room;
	NumDisplay* roomDisplay;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	vector<Level*> levels;	// como conservar level?
	//Level* level;
	int currentMap;
	int previousMap;
	int currentLevel;

	float upDownTime = 0;

	Animation* animation;
	bool bAnim = false;

	Victory* victory;
	bool bVict = false;

	bool started = false;
	int countStarted;

	bool isDead = false;

	bool ALL_DEAD = false;

	bool godMode = false;

	Texture godMode_spritesheet;
	Sprite* godMode_sprite;
};


#endif // _SCENE_INCLUDE


