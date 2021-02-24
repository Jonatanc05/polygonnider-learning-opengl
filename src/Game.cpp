#include "Game.hpp"

Game* Game::_instance;

Game* Game::getInstance() {
	if (_instance == nullptr)
		return new Game();
	return _instance;
};

Game::Game() {
	playerCount = DEFAULT_PLAYER_COUNT;
	players = (Player**) malloc(DEFAULT_PLAYER_COUNT * sizeof(Player));
	for (short int i = 0; i < playerCount; i++)
		players[i] = new Player(0.5f, i * 0.5f);
	Game::_instance = this;
};

Game::~Game() {
	for (short int i = 0; i < playerCount; i++)
		delete players[i];
};

void Game::OnKeyAction(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_RELEASE) {
		switch (key) {
			case GLFW_KEY_W: _instance->players[0]->yV -=  VELOCITY; break;
			case GLFW_KEY_A: _instance->players[0]->xV -= -VELOCITY; break;
			case GLFW_KEY_S: _instance->players[0]->yV -= -VELOCITY; break;
			case GLFW_KEY_D: _instance->players[0]->xV -=  VELOCITY; break;
			case GLFW_KEY_UP:    _instance->players[1]->yV -=  VELOCITY; break;
			case GLFW_KEY_LEFT:  _instance->players[1]->xV -= -VELOCITY; break;
			case GLFW_KEY_DOWN:  _instance->players[1]->yV -= -VELOCITY; break;
			case GLFW_KEY_RIGHT: _instance->players[1]->xV -=  VELOCITY; break;
			default: break;
		}
	} else if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_W: _instance->players[0]->yV +=  VELOCITY; break;
			case GLFW_KEY_A: _instance->players[0]->xV += -VELOCITY; break;
			case GLFW_KEY_S: _instance->players[0]->yV += -VELOCITY; break;
			case GLFW_KEY_D: _instance->players[0]->xV +=  VELOCITY; break;
			case GLFW_KEY_UP:    _instance->players[1]->yV +=  VELOCITY; break;
			case GLFW_KEY_LEFT:  _instance->players[1]->xV += -VELOCITY; break;
			case GLFW_KEY_DOWN:  _instance->players[1]->yV += -VELOCITY; break;
			case GLFW_KEY_RIGHT: _instance->players[1]->xV +=  VELOCITY; break;
			default: break;
		}
	}
};

void Game::update() {
	for (short int i = 0; i < playerCount; i++)
		players[i]->update();
};

// Temp
float* Game::getVertexes(int* size) {
	*size = 6 * sizeof(float);
	float *v = (float*) malloc(*size);
	v[0] = players[0]->xP;
	v[1] = players[0]->yP;
	v[2] = players[1]->xP;
	v[3] = players[1]->yP;
	v[4] = 0;
	v[5] = 0;

	return v;
};

