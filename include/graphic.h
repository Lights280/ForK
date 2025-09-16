#pragma once
#include "define.h"

class Graphic
{
private:
    struct GraphicObject
    {
        GLuint vao = 0;
        GLuint vbo = 0;
    };
    GLFWwindow *window;
    std::vector<GraphicObject> graphicObjects;
    GLint program;
    GLint flag;
    int keySet[GLFW_KEY_LAST] = {0};

public:
    Graphic();
    ~Graphic();
    void buildProgram(void);
    uint32_t generate(const int maxQuantity, const int type);
    void bindData(const float *data, const int quantity, const int start, const uint32_t id);
    void draw(const uint32_t id, const int type, const int start, const int quantity);
    void clear(void);
    int processKey(int key);
    // lambda macros
    const std::function<void(const int, const int, const int)> drawLines = [this](const uint32_t id, const int start, const int quantity)
    { draw(id, GL_LINES, start, 2 * quantity); },
                                                               drawTriangles = [this](const uint32_t id, const int start, const int quantity)
    { draw(id, GL_TRIANGLES, start, 3 * quantity); },
                                                               drawRectangle6 = [this](const uint32_t id, const int start, const int quantity)
    { draw(id, GL_TRIANGLES, start, 6 * quantity); },
                                                               drawRectangle4 = [this](const uint32_t id, const int start, const int quantity)
    {
        for (int i = 0; i < quantity; ++i)
        {
            draw(id, GL_TRIANGLES, start + i * 4, 3);
            draw(id, GL_TRIANGLES, start + i * 4 + 1, 3);
        }
    };
    const std::function<GLFWwindow *(void)> getWindow = [this]()
    { return window; };
    const std::function<void(void)> useProgram = [this]()
    { glUseProgram(program); };
};