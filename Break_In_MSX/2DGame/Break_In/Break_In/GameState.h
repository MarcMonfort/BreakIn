#ifndef _GAMESTATE_INCLUDE
#define _GAMESTATE_INCLUDE


class GameState
{

public:

	virtual void init() = 0;
	virtual void update(int deltaTime) = 0;
	virtual void render() = 0;

	// Input callback methods
	virtual void keyPressed(int key) = 0;
	virtual void keyReleased(int key) = 0;
	virtual void specialKeyPressed(int key) = 0;
	virtual void specialKeyReleased(int key) = 0;
	virtual void mouseMove(int x, int y) = 0;
	virtual void mousePress(int button) = 0;
	virtual void mouseRelease(int button) = 0;

	bool getKey(int key) const;
	bool getSpecialKey(int key) const;

protected:
	bool keys[256], specialKeys[256]; // Store key states so that 

};

#endif // _GAME_INCLUDE
