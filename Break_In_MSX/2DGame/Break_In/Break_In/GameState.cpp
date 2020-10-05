#include "GameState.h"


bool GameState::getKey(int key) const
{
	return keys[key];
}

bool GameState::getSpecialKey(int key) const
{
	return specialKeys[key];
}