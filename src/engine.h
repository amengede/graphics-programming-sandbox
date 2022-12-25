#pragma once
#include "config.h"
#include "shader.h"
#include "quad_model.h"

/**
	Graphics engine class, drawing functions are defined here.
*/
class Engine {
public:
	Engine(int width, int height);
	~Engine();
	
	/**
		Tells the engine to render a frame. As the project grows,
		this function should accept as an argument some description of
		the environment to be rendered.
	*/
	void render();

private:

	/**
		Creates the color buffer (an array of numbers representing pixel colors).
	*/
	void create_color_buffer();

	/**
		Displays the contents of the color buffer to the screen.
		This should be called at the end of the render function, after
		the color buffer has been written to for the current frame.
	*/
	void draw_screen();

	/**
		Set an individual pixel in the color buffer.

		\param x the x coordinate on the screen, between 0 (left) and width - 1 (right)
		\param y the y coordinate on the screen, between 0 (top) and height - 1 (bottom)
		\param color a glm::vec3 storing the color data in (r,g,b) form, with float values between 0 and 1
	*/
	void pset(int x, int y, const glm::vec3 &color);

	/**
		Draw a vertical line (the color buffer is currently set up so that vertical lines
		are cache friendly)

		\param x coordinate on the screen, between 0 (left) and width - 1 (right)
		\param y1 the y coordinate for the line top, between 0 (top) and height - 1 (bottom)
		\param y2 the y coordinate for the line bottom, between 0 (top) and height - 1 (bottom)
		\param color a glm::vec3 storing the color data in (r,g,b) form, with float values between 0 and 1
	*/
	void vertical_line(int x, int y1, int y2, const glm::vec3& color);

	/**
		Clear the whole color buffer to a given color.

		\param color a glm::vec3 storing the color data in (r,g,b) form, with float values between 0 and 1
	*/
	void clear_screen(const glm::vec3& color);

	/**
		Clear the whole color buffer to a given color. This implementation uses AVX2 SIMD operations to set
		8 pixels at a time.

		\param color a glm::vec3 storing the color data in (r,g,b) form, with float values between 0 and 1
	*/
	void clear_screen_avx2(const glm::vec3& color);

	/**
		Perform bitshifts etc to get a color vector into a single number.

		\param color a glm::vec3 storing the color data in (r,g,b) form, with float values between 0 and 1
		\returns a 32 bit integer representing the given RGB input
	*/
	uint32_t crunch_color(const glm::vec3& color);

	unsigned int shader, width, height;
	unsigned int colorBuffer;
	std::vector<uint32_t> colorBufferMemory;
	QuadModel* screenMesh;

};