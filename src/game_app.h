#pragma once
#include "config.h"
#include "engine.h"

enum class returnCode {
	CONTINUE, QUIT
};

class GameApp {
public:
	GameApp(int width, int height);
	void run();
	~GameApp();
private:
	GLFWwindow* makeWindow();
	returnCode processInput();
	void calculateFrameRate();

	GLFWwindow* window;
	int width, height;
	Engine* renderer;

	double lastTime, currentTime;
	int numFrames;
	float frameTime;
};