#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"


Scene::Scene()
{
	quad = NULL;
}

Scene::~Scene()
{
	if(quad != NULL)
		delete quad;
	for(int i=0; i<3; i++)
		if(texQuad[i] != NULL)
			delete texQuad[i];
}


void Scene::init()
{
	glm::vec2 geom[2] = {glm::vec2(0.f, 0.f), glm::vec2(128.f, 128.f)};
	glm::vec2 texCoords[2] = {glm::vec2(0.f, 0.f), glm::vec2(1.f, 1.f)};

	initShaders();
	quad = Quad::createQuad(0.f, 0.f, 128.f, 128.f, simpleProgram);
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(0.5f, 0.5f);
	texQuad[0] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);		//mario
	texCoords[0] = glm::vec2(0.5f, 0.5f); texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad[1] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);		//estrella
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(5.f, 1.25f);
	texQuad[2] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);		//rock
	texCoords[0] = glm::vec2(0.f, 0.5f); texCoords[1] = glm::vec2(0.5f, 1.f);
	texQuad[3] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);		//bolet
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	texQuad[4] = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);		//nuvol

	// Load textures
	texs[0].loadFromFile("images/varied.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[0].setMagFilter(GL_NEAREST);
	texs[1].loadFromFile("images/rocks.jpg", TEXTURE_PIXEL_FORMAT_RGB);
	texs[1].setMagFilter(GL_NEAREST);
	texs[2].loadFromFile("images/cloud2.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[2].setMagFilter(GL_NEAREST);
	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);
	currentTime = 0.0f;
	
	// Select which font you want to use
	if((!text[0].init("fonts/OpenSans-Regular.ttf")) || (!text[1].init("fonts/OpenSans-Bold.ttf")))
	//if(!text.init("fonts/OpenSans-Bold.ttf"))
	//if(!text.init("fonts/DroidSerif.ttf"))
		cout << "Could not load font!!!" << endl;
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
}

void Scene::render()
{
	glm::mat4 modelview;

	simpleProgram.use();
	simpleProgram.setUniformMatrix4f("projection", projection);
	simpleProgram.setUniform4f("color", 0.2f, 0.2f, 0.8f, 1.0f);

	//cel
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(320.f, 240.f, 0.f));	//mover al centro
	//modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	//modelview = glm::rotate(modelview, -currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::scale(modelview, glm::vec3(5.f, 3.75f, 0.0f));				//escalar a toda la pantalla
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	simpleProgram.setUniformMatrix4f("modelview", modelview);
	quad->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	//nuvol
	//modelview = glm::translate(glm::mat4(1.0f), glm::vec3(sin(currentTime / 5000.f) * 290.f, 0.f, 0.f));
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3((int)(currentTime / 10) % (760 + 100), 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, 100.f, 0.f));
	//modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	//modelview = glm::scale(modelview, glm::vec3(scaleValue, scaleValue, 0.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[4]->render(texs[2]);
	//nuvol2
	//modelview = glm::translate(glm::mat4(1.0f), glm::vec3(sin(currentTime / 5000.f) * 290.f, 0.f, 0.f));
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3((int)(currentTime / 10) % (640 + 64 + 500), 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(-500.f, 170.f, 0.f));
	//modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	//modelview = glm::scale(modelview, glm::vec3(scaleValue, scaleValue, 0.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[4]->render(texs[2]);

	//mario
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, -abs(cos(currentTime / 600.f) * 80.f), 0.f));
	modelview = glm::translate(modelview, glm::vec3(cos(currentTime / 1200.f) * 80.f, 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(320.f, 320 - 25.f, 0.f));
	//modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	//modelview = glm::rotate(modelview, currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::scale(modelview, glm::vec3(0.5f, 0.5f, 0.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[0]->render(texs[0]); //com a parametre li enviem quina textura (imatge) agafa el quad

	//estrella
	float scaleValue = 1.f - 0.75f * (sin(currentTime / 1000.f) + 1.f) / 2.f;
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, cos(currentTime / 1000.f) * 40.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(sin(currentTime / 700.f) * 290.f, 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(320.f, 160.f, 0.f));
	//modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	modelview = glm::rotate(modelview, currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::scale(modelview, glm::vec3(scaleValue, scaleValue, 0.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[1]->render(texs[0]);

	//rock
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(320.f, 320.f + 80.f, 0.f));	//mover a la parte de debajo
	//modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	//modelview = glm::rotate(modelview, -currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::scale(modelview, glm::vec3(5.f, 1.25f, 0.0f));	//escalar a 1/3 de la pantalla (horizontal)
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	simpleProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[2]->render(texs[1]);

	//bolet
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(sin(currentTime / 1000.f) * 288.f, 0.f, 0.f));
	modelview = glm::translate(modelview, glm::vec3(320.f, 320 - 14.f, 0.f));
	//modelview = glm::translate(modelview, glm::vec3(64.f, 64.f, 0.f));
	//modelview = glm::rotate(modelview, -currentTime / 1000.f, glm::vec3(0.0f, 0.0f, 1.0f));
	modelview = glm::scale(modelview, glm::vec3(0.5f, 0.5f, 0.0f));
	modelview = glm::translate(modelview, glm::vec3(-64.f, -64.f, 0.f));
	texProgram.setUniformMatrix4f("modelview", modelview);
	texQuad[3]->render(texs[0]);

	
	
	text[0].render("Videogames!!!", glm::vec2(10, CAMERA_HEIGHT-20), 32, glm::vec4(1, 1, 1, 1));

	if (abs(sin(currentTime / 1000)) >= .99f && !flagRebot) {
		flagRebot = true;
		++rebots;	//falta precision???
	}
	if (abs(sin(currentTime / 1000)) < .99f && flagRebot) {
		flagRebot = false;
	}

	text[0].render("Rebots: " + std::to_string(rebots), glm::vec2(10, 40), 32, glm::vec4(1, 1, 1, 1));	//se puede usar text multiples veces
	if (showText) {
		text[1].render("Bolet", glm::vec2((sin(currentTime / 1000.f) * 288.f) + 280.f, 280.f), 32, glm::vec4(1, 1, 1, 1));
	}

}

void Scene::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleProgram.init();
	simpleProgram.addShader(vShader);
	simpleProgram.addShader(fShader);
	simpleProgram.link();
	if(!simpleProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleProgram.log() << endl << endl;
	}
	simpleProgram.bindFragmentOutput("outColor");

	vShader.free();
	fShader.free();
	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if(!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if(!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if(!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
}

