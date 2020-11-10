#ifndef _LIGHTING_INCLUDE
#define _LIGHTING_INCLUDE

#include "Sprite.h"
#include "TileMap.h"


class Lightning
{
public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition();

	void reset();

	void stopMusic();

	void setTileMap(TileMap* tileMap);

private:
	glm::ivec2 tileMapDispl, posLightning;
	Texture spritesheet;
	Sprite* sprite;

	Texture spritesheet2;
	Sprite* electric_ball;

	int counter_strike = 0;
	int counter_ball = 0;
	int counter_restart = 0;

	bool eBall;
	bool isStrike;

	const SoundManager* soundManager;
	FMOD::Sound* music_shotgun;
	FMOD::Sound* music_eBall;
	FMOD::Channel* channel;

	TileMap* map;
};

#endif
