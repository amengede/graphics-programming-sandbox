#pragma once
#include "config.h"
#include "shader.h"
#include "quad_model.h"

struct FrameSize {
	unsigned int width, height;
};

class Engine {
public:
	Engine(int width, int height);
	~Engine();

	void render();
	void create_color_buffer();
	void draw_screen();
	void pset(int x, int y, const glm::vec3 &color);
	void vertical_line(int x, int y1, int y2, const glm::vec3& color);
	void clear_screen(const glm::vec3& color);
	void clear_screen_avx2(const glm::vec3& color);
	uint32_t crunch_color(const glm::vec3& color);

	unsigned int shader, width, height;
	unsigned int colorBuffer;
	std::vector<uint32_t> colorBufferMemory;
	QuadModel* screenMesh;

};