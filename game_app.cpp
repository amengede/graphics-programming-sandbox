#include "game_app.h"

GameApp::GameApp(int width, int height) {
	this->width = width;
	this->height = height;

	lastTime = glfwGetTime();
	numFrames = 0;
	frameTime = 16.0f;

	window = makeWindow();
	// eg. hiding the mouse
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	renderer = new Engine(width, height);

}

GLFWwindow* GameApp::makeWindow() {
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

	GLFWwindow* window = glfwCreateWindow(width, height, "This is working I hope", NULL, NULL);

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, width, height);

	return window;
}

returnCode GameApp::processInput() {

	//eg. getting amd setting the mouse position
	double mouse_x, mouse_y;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);
	//glfwSetCursorPos(window, static_cast<double>(width / 2), static_cast<double>(height / 2));

	// eg. checking if the esc key is down
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		return returnCode::QUIT;
	}
	return returnCode::CONTINUE;
}

void GameApp::run() {

	returnCode nextAction = returnCode::CONTINUE;

	while (nextAction == returnCode::CONTINUE) {

		//mouse position/keypress code can be handled here
		nextAction = processInput();
		glfwPollEvents();

		//update logic can be handled here

		//draw (no arguments right now, as no real data exists for drawing)
		renderer->render();

		calculateFrameRate();

	}
}

GameApp::~GameApp() {
	//free memory
	delete renderer;
	glfwTerminate();
}

void GameApp::calculateFrameRate() {

	currentTime = glfwGetTime();
	double delta = currentTime - lastTime;

	if (delta >= 1) {
		int framerate{ std::max(1, int(numFrames / delta)) };
		std::stringstream title;
		title << "Running at " << framerate << " fps.";
		glfwSetWindowTitle(window, title.str().c_str());
		lastTime = currentTime;
		numFrames = -1;
		frameTime = float(1000.0 / framerate);
	}

	++numFrames;
}