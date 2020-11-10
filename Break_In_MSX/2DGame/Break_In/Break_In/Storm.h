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

private:
	Drop* drop;
	vector<Drop*> drops;

	Lightning* lightning;

	int currentTime;
};

#endif
