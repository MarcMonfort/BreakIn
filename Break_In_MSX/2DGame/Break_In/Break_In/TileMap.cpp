#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "TileMap.h"
#include <glm\gtc\matrix_transform.hpp>
#include "PlayGameState.h"
#include "Game.h"


#define SCREEN_X 32 //tiene que se igual al de PlayGameState.cpp
#define SCREEN_Y 48 //16

using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	
	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{

	soundManager = Game::instance().getSoundManager();
	music_wall_coin = soundManager->loadSound("sounds/wall_coin.mp3", FMOD_DEFAULT);
	music_basic_block = soundManager->loadSound("sounds/basic_block.mp3", FMOD_DEFAULT);
	music_bag = soundManager->loadSound("sounds/bag.mp3", FMOD_DEFAULT);
	drop_block1 = soundManager->loadSound("sounds/drop_down.mp3", FMOD_DEFAULT);
	drop_block2 = soundManager->loadSound("sounds/drop_block2.mp3", FMOD_DEFAULT);

	/*music = soundManager->loadSound("sounds/main_theme.mp3", FMOD_LOOP_NORMAL | FMOD_CREATESTREAM);
	channel = soundManager->playSound(music);
	channel->setVolume(1.0f);*/

	loadLevel(levelFile);
	a = minCoords;
	b = program;
	//prepareArrays(a, b);
	prepareArrays(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);

}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}


bool TileMap::loadLevel(const string &levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;
	
	fin.open(levelFile.c_str());
	if(!fin.is_open())
		return false;
	getline(fin, line);
	if(line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> backgroundFile;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	money_items = 0;
	
	map = new int[mapSize.x * mapSize.y];
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			fin.get(tile);
			if(tile == ' ')
				map[j*mapSize.x+i] = 0;
			else {
				map[j*mapSize.x+i] = tile - int('0');
				
				if (tile == 'I') {
					bRing = true;
					ringPosition = glm::vec2(i * tileSize, j * tileSize);
				}
				else if (tile == 'O' || tile == 'a') {
					money_items += 1;
				}
			}
		}
		fin.get(tile);
#ifndef _WIN32
		fin.get(tile);
#endif
	}
	fin.close();

	thereIsMoney = money_items != 0;
		
	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	int tile, nTiles = 0;
	glm::vec2 posTile, texCoordTile[2], halfTexel;
	vector<float> vertices;
	
	halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
	for(int j=0; j<mapSize.y; j++)
	{
		for(int i=0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			if(tile != 0)
			{
				// Non-empty tile
				nTiles++;
				posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
				//texCoordTile[0] = glm::vec2(float((tile-1)%2) / tilesheetSize.x, float((tile-1)/2) / tilesheetSize.y);
				texCoordTile[0] = glm::vec2(float((tile-1)%tilesheetSize.x) / tilesheetSize.x, float((tile-1)/tilesheetSize.y) / tilesheetSize.y); //NEW
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				//texCoordTile[0] += halfTexel;
				texCoordTile[1] -= halfTexel;  //��que es esto?
				// First triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				// Second triangle
				vertices.push_back(posTile.x); vertices.push_back(posTile.y);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[0].y);
				vertices.push_back(posTile.x + blockSize); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[1].x); vertices.push_back(texCoordTile[1].y);
				vertices.push_back(posTile.x); vertices.push_back(posTile.y + blockSize);
				vertices.push_back(texCoordTile[0].x); vertices.push_back(texCoordTile[1].y);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
}


string TileMap::getBackgroundFile()
{
	return backgroundFile;
}


bool TileMap::hasRing()
{
	return bRing;
}

glm::vec2 TileMap::getRingPosition()
{
	return ringPosition;
}

bool TileMap::alarmOn()
{
	return bAlarm;
}

void TileMap::setAlarm(bool alarm)
{
	bAlarm = alarm;
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.


bool TileMap::checkCollision(const glm::ivec2& pos)
{
	int x, y;

	x = pos.x / tileSize;
	y = pos.y / tileSize;
	if (map[y * mapSize.x + x] != 0)
		return true;

	return false;
}

bool TileMap::collisionPoint(const glm::ivec2& pos, int type)
{
	int x, y;

	x = pos.x / tileSize;
	y = pos.y / tileSize;
	if (map[y * mapSize.x + x] != 0)
		return treatCollision(y * mapSize.x + x, type);

	return false;
}


//bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size)
//{
//	int x, y0, y1;
//	
//	x = pos.x / tileSize;
//	y0 = pos.y / tileSize;
//	y1 = (pos.y + size.y - 1) / tileSize;
//	for(int y=y0; y<=y1; y++)
//	{
//		if (map[y * mapSize.x + x] != 0)
//		{
//			return treatCollision(y * mapSize.x + x);
//		}
//	}
//	
//	return false;
//}
//
//bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size)
//{
//	int x, y0, y1;
//	
//	x = (pos.x + size.x - 1) / tileSize;
//	y0 = pos.y / tileSize;
//	y1 = (pos.y + size.y - 1) / tileSize;
//	for(int y=y0; y<=y1; y++)
//	{
//		if (map[y * mapSize.x + x] != 0)
//		{
//			return treatCollision(y * mapSize.x + x);
//		}
//	}
//	
//	return false;
//}
//
//bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size)
//{
//	int x0, x1, y;
//
//	x0 = pos.x / tileSize;
//	x1 = (pos.x + size.x - 1) / tileSize;
//	y = pos.y / tileSize;
//	for (int x = x0; x <= x1; x++)
//	{
//		//if (map[y * mapSize.x + x] == 'A' - int('0'))
//		if (map[y * mapSize.x + x] != 0)
//		{
//			return treatCollision(y * mapSize.x + x);
//		}
//	}
//
//	return false;
//}
//
//bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size)
//{
//	int x0, x1, y;
//	
//	x0 = pos.x / tileSize;
//	x1 = (pos.x + size.x - 1) / tileSize;
//	y = (pos.y + size.y - 1) / tileSize;
//	for(int x=x0; x<=x1; x++)
//	{
//		if (map[y * mapSize.x + x] != 0)
//		{
//			return treatCollision(y * mapSize.x + x);
//		}
//	}
//	
//	return false;
//}

int TileMap::checkBlock(int block)
{
	if (block <= 14)
		return basic;
	else if (block <= 16)
		return strong;
	else if (block <= 19)
		return wall;
	else if (block == 20)
		return lose;
	else if (block <= 22)
		return arrow;
	else if (block == 23 || block == 24 || block == 39 || block == 40)
		return key;
	else if (block == 34 || block == 25)
	//else if (block == 25 || block == 26 || block == 41 || block == 42)
		return ring;
	else if (block == 31 || block == 32 || block == 47 || block == 48)
		return bag;
	else if (block == 49 || block == 50 || block == 65 || block == 66)
		return coin;
}

bool TileMap::treatCollision(int pos, int type)
{
	int block = checkBlock(map[pos]);
	
	if (block == basic)
	{
		if (map[pos] % 2 == 0)
			map[pos - 1] = 0;
		else
			map[pos + 1] = 0;
		map[pos] = 0;
		//channel->stop();
		if (type == 0)
			channel = soundManager->playSound(music_basic_block);
		else {
			channel = soundManager->playSound(drop_block2);
			channel->setVolume(0.3f);
		}

		//channel->setVolume(1.0f);
		PlayGameState::instance().addPoints(100);
	}
	else if (block == wall) {
		//channel->stop();
		if (type == 0)
			channel = soundManager->playSound(music_wall_coin);
		else {
			channel = soundManager->playSound(drop_block2);
			channel->setVolume(0.3f);
		}

		//channel->setVolume(1.0f);
	}
	else if (block == strong)
	{
		if (map[pos] % 2 == 0) {
			map[pos] = 14;
			map[pos - 1] = 13;
		}
		else {
			map[pos] = 13;
			map[pos + 1] = 14;
		}
		//channel->stop();
		if (type == 0)
			channel = soundManager->playSound(music_basic_block);
		else {
			channel = soundManager->playSound(drop_block2);
			channel->setVolume(0.3f);
		}

		//channel->setVolume(1.0f);
		PlayGameState::instance().addPoints(100);
	}
	else if (block == key)
	{
		if (map[pos] == 23) {
			map[pos+1] = 0;
			map[pos+mapSize.x] = 0;
			map[pos+mapSize.x+1] = 0;

		}
		else if (map[pos] == 24) {
			map[pos - 1] = 0;
			map[pos + mapSize.x] = 0;
			map[pos + mapSize.x - 1] = 0;
		}
		else if (map[pos] == 39) {
			map[pos + 1] = 0;
			map[pos - mapSize.x] = 0;
			map[pos - mapSize.x + 1] = 0;
		}
		else if (map[pos] == 40) {
			map[pos - 1] = 0;
			map[pos - mapSize.x] = 0;
			map[pos - mapSize.x - 1] = 0;
		}
		map[pos] = 0;
		map[8] = 21;
		map[9] = 22;
		map[10] = 21;
		map[11] = 22;
		map[12] = 21;
		map[13] = 22;
		map[14] = 21;
		map[15] = 22;
		map[pos] = 0;

		if (type == 0)
			channel = soundManager->playSound(music_wall_coin);
		else {
			channel = soundManager->playSound(drop_block2);
			channel->setVolume(0.3f);
		}

		//channel->setVolume(1.0f);
	}
	else if (block == arrow)
	{
		if (type == 0) {
			PlayGameState::instance().nextMap();
			return false;
		}
	}
	else if (block == lose)
	{
		if (type == 0) {
			if (!PlayGameState::instance().getGodMode())
			{
				PlayGameState::instance().lastMap();
				return false;
			}
		}
	}
	else if (block == bag)
	{
		if (map[pos] == 31) {
			map[pos + 1] = 0;
			map[pos + mapSize.x] = 0;
			map[pos + mapSize.x + 1] = 0;
		}
		else if (map[pos] == 32) {
			map[pos - 1] = 0;
			map[pos + mapSize.x] = 0;
			map[pos + mapSize.x - 1] = 0;
		}
		else if (map[pos] == 47) {
			map[pos + 1] = 0;
			map[pos - mapSize.x] = 0;
			map[pos - mapSize.x + 1] = 0;
		}
		else if (map[pos] == 48) {
			map[pos - 1] = 0;
			map[pos - mapSize.x] = 0;
			map[pos - mapSize.x - 1] = 0;
		}
		map[pos] = 0;
		if (type == 0)	
			channel = soundManager->playSound(music_bag);
		else {
			channel = soundManager->playSound(drop_block2);
			channel->setVolume(0.3f);
		}

		PlayGameState::instance().addMoney(200);
		money_items -= 1;
	}
	else if (block == coin)
	{
		if (map[pos] == 49) {
			map[pos + 1] = 0;
			map[pos + mapSize.x] = 0;
			map[pos + mapSize.x + 1] = 0;

		}
		else if (map[pos] == 50) {
			map[pos - 1] = 0;
			map[pos + mapSize.x] = 0;
			map[pos + mapSize.x - 1] = 0;
		}
		else if (map[pos] == 65) {
			map[pos + 1] = 0;
			map[pos - mapSize.x] = 0;
			map[pos - mapSize.x + 1] = 0;
		}
		else if (map[pos] == 66) {
			map[pos - 1] = 0;
			map[pos - mapSize.x] = 0;
			map[pos - mapSize.x - 1] = 0;
		}
		map[pos] = 0;
		if (type == 0)
			channel = soundManager->playSound(music_wall_coin);
		else {
			channel = soundManager->playSound(drop_block2);
			channel->setVolume(0.3f);
		}

		PlayGameState::instance().addMoney(100);
		money_items -= 1;
	}
	else if (block == ring)
	{
		if (type == 0)
			bAlarm = true;
		/*if (map[pos] == 25) {
			map[pos + 1] = 33;
			map[pos + mapSize.x] = 43;
			map[pos + mapSize.x + 1] = 33;
			map[pos] = 27;
		}
		else if (map[pos] == 26) {
			map[pos - 1] = 27;
			map[pos + mapSize.x] = 33;
			map[pos + mapSize.x - 1] = 43;
			map[pos] = 33;
		}
		else if (map[pos] == 41) {
			map[pos + 1] = 33;
			map[pos - mapSize.x] = 27;
			map[pos - mapSize.x + 1] = 33;
			map[pos] = 43;
		}
		else if (map[pos] == 42) {
			map[pos - 1] = 43;
			map[pos - mapSize.x] = 33;
			map[pos - mapSize.x - 1] = 27;
			map[pos] = 33;
		}*/
	}
	prepareArrays(a, b);
	return true;

}

bool TileMap::noMoneyLeft() {
	return (thereIsMoney && money_items == 0);
}
