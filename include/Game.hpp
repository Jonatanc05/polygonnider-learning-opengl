#ifndef GAME_H
#define GAME_H

#include <GLFW/glfw3.h>
#include <iostream>
#include "Player.hpp"

#define DEFAULT_PLAYER_COUNT 2
#define VELOCITY 0.001f

class Game {

public:
	unsigned int playerCount;
	Player **players;

	static Game* getInstance();

	static void OnKeyAction(GLFWwindow *window, int key, int scancode, int action, int mods);

	void update();

	float* getVertexes(int* size);


private:
	Game();
	~Game();

	static Game* _instance;

};

#endif
