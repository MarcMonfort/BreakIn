#include "Animation.h"
#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Game.h"
#include "PlayGameState.h"
#include <GL/glew.h>
#include <GL/glut.h>



#define SCREEN_X 32 //tiene que se igual al de PlayGameState.cpp
#define SCREEN_Y 48

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 25

void Animation::init()
{
	//glClearColor(1.f, 0.0f, 0.0f, 1.0f);

	initShaders();

	spritesheet.loadFromFile("images/animationBackground.png", TEXTURE_PIXEL_FORMAT_RGBA);
	background = Sprite::createSprite(glm::ivec2(24 * 16, 24 * 16), glm::vec2(1.f, 1.f), &spritesheet, &texProgram);
	background->setPosition(glm::vec2(SCREEN_X, SCREEN_Y ));

	thief = new Thief();
	thief->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
	thief->setPosition(glm::vec2(60,258) );


	projection = glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
	currentTime = 0.0f;

	soundManager = Game::instance().getSoundManager();
	music_player = soundManager->loadSound("sounds/thief.mp3", FMOD_DEFAULT);
	

}

void Animation::restart()
{
	thief->setPosition(glm::vec2(60, 258));
	channel = soundManager->playSound(music_player);
}

void Animation::stopMusic()
{
	channel->stop();
}

void Animation::update(int deltaTime)
{
	currentTime += deltaTime;
	thief->update(deltaTime);

	if (thief->getPosition().x >= 325) {
		PlayGameState::instance().stopAnimation();
	}


}

void Animation::render()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glm::mat4 modelview;
	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
	background->render();

	thief->render();
}

void Animation::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}