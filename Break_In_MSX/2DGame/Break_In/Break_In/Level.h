#ifndef _LEVEL_INCLUDE
#define _LEVEL_INCLUDE


#include <glm/glm.hpp>
#include "GameState.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Drop.h"
#include "Lightning.h"
#include "Ring.h"
#include "Guard.h"
#include "SoundManager.h"
#include "Cloud.h"
#include "Storm.h"


class Level
{

public:
	void createLevel(int numLevel, int numMap);
	void update(int deltaTime);
	void render();

	TileMap* getMap();


	enum transitions
	{
		upCenter,
		downCenter,
		centerUp,
		centerDown
	};

	void setTransition(int transition);
	void resetGuard();
	void setAlarm(bool alarm);
	void setMusic(bool music);

	void deleteALL();

	void cloud_taken();
	void createStorm();


private:
	void initShaders();

private:
	TileMap* map;
	//Player* player;
	//Ball* ball;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int numLevel;

	float transTime;
	int transition;

	Sprite* background;
	Texture spritesheet;

	Ring* ring;

	bool bRing = false;
	bool bAlarm = false;

	Guard* guard;

	const SoundManager* soundManager;
	FMOD::Sound* music_alarm;
	FMOD::Channel* channel;

	bool musicOn;

	Drop* drop;
	vector<Drop*> drops;

	Lightning* lightning;

	Cloud* cloud;

	bool bCloud = false;
	bool bStorm = false;
	Storm* storm;


	int cloudCounter = 0;
	int cloudCounterLimit;
};


#endif
