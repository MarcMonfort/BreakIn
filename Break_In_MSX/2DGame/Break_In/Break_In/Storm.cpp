#include "Storm.h"
#include "Game.h"


#define SCREEN_X 32 //tiene que se igual al de PlayGameState.cpp
#define SCREEN_Y 48

void Storm::init(TileMap* tileMap, ShaderProgram& shaderProgram)
{

	spritesheet.loadFromFile("images/big_cloud.png", TEXTURE_PIXEL_FORMAT_RGBA);
	cloud1 = Sprite::createSprite(glm::ivec2(200, 150), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	posC1 = glm::vec2(-70, -50);
	//cloud1->setPosition(glm::vec2(40,-50));
	cloud1->setPosition(posC1);

	cloud2 = Sprite::createSprite(glm::ivec2(200, 150), glm::vec2(1, 1), &spritesheet, &shaderProgram);
	posC2 = glm::vec2(320, -50);
	cloud2->setPosition(posC2);

	//cloud2->setPosition(glm::vec2(220, -50));

	int velocity = rand() % 3 - 1;

	for (int i = 0; i < 10; ++i) {
		Drop* drop = new Drop(); //usar punteros?
		drop->init(glm::ivec2(SCREEN_X, SCREEN_Y), shaderProgram, velocity);
		drop->setTileMap(tileMap);
		drops.push_back(drop);
	}

	lightning = new Lightning();
	lightning->init(glm::ivec2(SCREEN_X, SCREEN_Y), shaderProgram);
	//lightning->setPosition(glm::vec2(50, 0));

	currentTime = 0;
	started = false;

	soundManager = Game::instance().getSoundManager();
	music_rain = soundManager->loadSound("sounds/heavy_rain.mp3", FMOD_LOOP_NORMAL | FMOD_CREATESTREAM);
	channel = soundManager->playSound(music_rain);
	channel->setVolume(0.2f);


}


void Storm::update(int deltaTime)
{
	currentTime += deltaTime;

	

	
	if (posC1.x < 30) {
		posC1.x += 1;
		posC2.x -= 1;

		cloud1->setPosition(posC1);
		cloud2->setPosition(posC2);
	}
	else {
		started = true;
	}

	if (started) {
		int velocity = rand() % 3 - 1;

		for (int i = 0; i < 10; ++i) {
			if (currentTime > 2000) {
				drops[i]->setStartVelocity(velocity);
			}
			drops[i]->update(deltaTime, glm::vec2(50, 50));
		}
		if (currentTime > 2000)
		{
			currentTime = 0;
		}

		lightning->update(deltaTime);

		cloud1->update(deltaTime);
		cloud2->update(deltaTime);
	}
}

void Storm::render()
{
	cloud1->render();
	cloud2->render();

	if (started)
	{
		for (int i = 0; i < 10; ++i) {
			drops[i]->render();
		}

		lightning->render();
	}

}


void Storm::setMusic(bool b) {
	if (b) {
		channel = soundManager->playSound(music_rain);
		channel->setVolume(0.5f);
	}
	else {
		channel->stop();
	}
}

void Storm::reset() {
	currentTime = 0;
	started = false;


	posC1 = glm::vec2(-70, -50);
	//cloud1->setPosition(glm::vec2(40,-50));
	cloud1->setPosition(posC1);

	posC2 = glm::vec2(320, -50);
	cloud2->setPosition(posC2);

	lightning->reset();
}