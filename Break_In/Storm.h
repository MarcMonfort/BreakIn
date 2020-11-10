#ifndef _STORM_INCLUDE
#define _STORM_INCLUDE

#include "Drop.h"
#include "Lightning.h"
#include "SoundManager.h"


class Storm
{


public:
	void init(TileMap* tileMap, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setMusic(bool b);
	void reset();

private:
	Drop* drop;
	vector<Drop*> drops;

	Lightning* lightning;

	int currentTime;


	Texture spritesheet;
	Sprite* cloud1;
	Sprite* cloud2;

	glm::vec2 posC1;
	glm::vec2 posC2;

	bool started;


	const SoundManager* soundManager;
	FMOD::Sound* music_rain;
	FMOD::Channel* channel;
};

#endif
