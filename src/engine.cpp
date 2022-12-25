#include "engine.h"

Engine::Engine(int width, int height) {

    shader = util::load_shader("shaders/vertex.txt", "shaders/fragment.txt");
    glUseProgram(shader);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    this->width = width;
    this->height = height;
    screenMesh = new QuadModel;

    create_color_buffer();

}

Engine::~Engine() {
    delete screenMesh;
    glDeleteTextures(1, &colorBuffer);
    glDeleteProgram(shader);
}

void Engine::create_color_buffer() {

    unsigned int tempWidth = width;
    unsigned int tempHeight = height;

    glGenTextures(1, &colorBuffer);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);

    glTextureParameteri(colorBuffer, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(colorBuffer, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(colorBuffer, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(colorBuffer, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    colorBufferMemory.reserve(width * height);
    for (int x = 0; x < tempWidth; ++x) {
        for (int y = 0; y < tempHeight; ++y) {
            colorBufferMemory.push_back(0);
        }
    }

}

void Engine::render() {

    glm::vec3 green = glm::vec3(32.0f / 255.0f, 192.0f / 255.0f, 32.0f / 255.0f);

    clear_screen(green);
    //clear_screen_avx2(green);

    draw_screen();
}

void Engine::clear_screen(const glm::vec3& color) {

    uint32_t flattened_color = crunch_color(color);

    for (int pixel = 0; pixel < width * height; ++pixel) {
        colorBufferMemory[pixel] = flattened_color;
    }
}

void Engine::clear_screen_avx2(const glm::vec3& color) {

    uint32_t flattened_color = crunch_color(color);

    __m256i colorSIMD = _mm256_set1_epi32(flattened_color);
    int blockCount = static_cast<int>(width * height / 8);
    __m256i* blocks = (__m256i*) colorBufferMemory.data();

    //SIMD as much as possible
    for (int block = 0; block < blockCount; ++block) {
        blocks[block] = colorSIMD;
    }

    //set any remaining pixels individually
    for (int pixel = blockCount * 8; pixel < width * height; ++pixel) {
        colorBufferMemory[pixel] = flattened_color;
    }
}

void Engine::vertical_line(int x, int y1, int y2, const glm::vec3& color){

    uint32_t flattened_color = crunch_color(color);

    for (int y = y1; y <= y2; ++y) {
        colorBufferMemory[y + height * x] = flattened_color;
    }
}

void Engine::pset(int x, int y, const glm::vec3& color) {

    colorBufferMemory[y + height * x] = crunch_color(color);
}

uint32_t Engine::crunch_color(const glm::vec3& color) {

    uint8_t r = std::max(0, std::min(255, (int)(255 * color.x)));
    uint8_t g = std::max(0, std::min(255, (int)(255 * color.y)));
    uint8_t b = std::max(0, std::min(255, (int)(255 * color.z)));
    return (r << 24) + (g << 8) + (b << 16);
}

void Engine::draw_screen() {

    glUseProgram(shader);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorBuffer);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, height, width, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorBufferMemory.data());

    glBindVertexArray(screenMesh->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glFlush();

}