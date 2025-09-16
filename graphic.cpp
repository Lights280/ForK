#include "graphic.h"

const char *vertexShaderSource = "#version 330 core\n"
                                 "layout (location = 0) in vec3 aPos;\n"
                                 "void main()\n"
                                 "{\n"
                                 "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                 "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "   FragColor = vec4(0.8f, 0.2f, 0.2f, 1.0f);\n"
                                   "}\0";

constexpr auto frameSizeCallback = [](GLFWwindow *, int width, int height)
{ glViewport(0, 0, width, height); };

Graphic::Graphic()
{
    if (!glfwInit())
    {
        error("Failed to initialize GLFW!");
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(1600, 900, "ForK - A single four key music game!", nullptr, nullptr);
    if (!window)
    {
        error("Failed to create GLFW window!");
        return;
    }
    info("Creating the main window...");
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameSizeCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        error("Failed to initialize GLAD.");
        return;
    }
}

Graphic::~Graphic()
{
    for (auto &object : graphicObjects)
    {
        glDeleteBuffers(1, &object.vbo);
        glDeleteVertexArrays(1, &object.vao);
    }
    if (program)
        glDeleteProgram(program);
    if (window)
        glfwDestroyWindow(window);
    glfwTerminate();
}

void Graphic::buildProgram(void)
{
    program = glCreateProgram();
    const auto mkShader = [this](const char *shaderSRC, const int type) -> std::pair<int, int>
    {
        unsigned int shader = glCreateShader(type);
        glShaderSource(shader, 1, &shaderSRC, nullptr);
        glCompileShader(shader);
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            error(infoLog);
            return std::pair(0, 0);
        }
        glAttachShader(program, shader);
        return std::pair(1, shader);
    };
    std::pair<int, int> vShader = mkShader(vertexShaderSource, GL_VERTEX_SHADER),
                        fShader = mkShader(fragmentShaderSource, GL_FRAGMENT_SHADER);
    if (!vShader.first || !fShader.first)
        return;
    glLinkProgram(program);
    glDeleteShader(vShader.second);
    glDeleteShader(vShader.second);
    return;
}

uint32_t Graphic::generate(const int maxQuantity, const int type)
{
    glUseProgram(program);
    uint32_t id = graphicObjects.size();
    GraphicObject newVertex;
    glGenVertexArrays(2, &newVertex.vao);
    glGenBuffers(1, &newVertex.vbo);
    glBindVertexArray(newVertex.vao);
    glBindBuffer(GL_ARRAY_BUFFER, newVertex.vbo);
    glBufferData(GL_ARRAY_BUFFER, VSIZE * maxQuantity, nullptr, type);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VSIZE, (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    graphicObjects.push_back(newVertex);
    return id;
}

void Graphic::bindData(const float *data, const int quantity, const int start, const uint32_t id)
{
    glUseProgram(program);
    glBindVertexArray(graphicObjects[id].vao);
    glBindBuffer(GL_ARRAY_BUFFER, graphicObjects[id].vbo);
    glBufferSubData(GL_ARRAY_BUFFER, start * VSIZE, quantity * VSIZE, data);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return;
}

void Graphic::draw(const uint32_t id, const int type, const int start, const int quantity)
{
    glBindVertexArray(graphicObjects[id].vao);
    glBindBuffer(GL_ARRAY_BUFFER, graphicObjects[id].vbo);
    glDrawArrays(type, start, quantity);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    return;
}

void Graphic::clear(void)
{
    glfwSwapBuffers(window);
    glfwPollEvents();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    return;
}

int Graphic::processKey(int key)
{
    if (glfwGetKey(window, key) == GLFW_RELEASE)
        return keySet[key] = 0;
    else
    {
        if (keySet[key] == 0)
            return keySet[key] = 1;
        else
            return 2;
    }
}
