#ifndef _VICTORY_INCLUDE
#define _VICTORY_INCLUDE


#include <glm/glm.hpp>
#include "GameState.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Thief.h"
#include "SoundManager.h"


class Victory
{

public:
	void init();
	void update(int deltaTime);
	void render();
	void restart();

	void stopMusic();


private:
	void initShaders();

	Sprite* background;
	Texture spritesheet;

	Sprite* car;
	Texture spritesheet2;

	Sprite* victory;
	Texture spritesheet3;

	bool started = false;

	int counter;
	bool bCar = false;
	bool bDoor = false;
	bool bEngine = false;

	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

	Thief* thief;

	const SoundManager* soundManager;
	FMOD::Sound* music_player;
	FMOD::Sound* coins;
	FMOD::Sound* car_door;
	FMOD::Sound* car_engine;
	FMOD::Channel* channel;
	FMOD::Channel* channel2;
	FMOD::Channel* channel3;
	FMOD::Channel* channel4;
};

#endif
