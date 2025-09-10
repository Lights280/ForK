#include "glad/glad.h"
#include "alc.h"
#include "al.h"
#include "glfw3.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include <queue>
#include <cmath>
#include <cstring>

const GLchar *shader1 = new char[123]{"#version 330 core\n"
                                      "layout (location = 0) in vec3 aPos;\n"
                                      "void main()\n"
                                      "{\n"
                                      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                      "}\0"};

static constexpr int KEY[4] = {GLFW_KEY_D, GLFW_KEY_F, GLFW_KEY_J, GLFW_KEY_K};

static constexpr float game_vertices[] = {
    // 顶点。
    -0.2f, -1.0f, 0.0f, // 第一条线起点
    -0.2f, 1.0f, 0.0f,  // 第一条线终点
    0.2f, -1.0f, 0.0f,  // 第二条线起点
    0.2f, 1.0f, 0.0f,   // 第二条线终点
    0.6f, -1.0f, 0.0f,  // 第三条线起点
    0.6f, 1.0f, 0.0f,   // 第三条线终点
    1.0f, -1.0f, 0.0f,  // 第四条线起点
    1.0f, 1.0f, 0.0f    // 第四条线终点
};

struct note
{
    int offset; // offset (milliseconds)
    int type;
};

struct Shader
{
    char *program;
    int length;
    int shaderId;
};

struct Point3d
{
    float x;
    float y;
    float z;
};

inline constexpr void frameSizeCallback(GLFWwindow *window, int width, int height) noexcept
{
    glViewport(0, 0, width, height);
}

inline constexpr void error(const char *message) noexcept
{
    std::cerr << "[Error]:" << message << std::endl;
}

inline constexpr void info(const char *message) noexcept
{
    std::cerr << "[Info]:" << message << std::endl;
}

inline Point3d mkPt(float x, float y, float z) noexcept
{
    Point3d point;
    point.x = x, point.y = y, point.z = z;
    return point;
}

class Program
{
private:
    int program;
    int count;
    int status;
    int flush; // status has been changed but not changes is applied.
    int temp[10];
    int flag;
    std::thread *handle;
    std::chrono::system_clock::time_point startTick, tick, frametick;
    std::queue<struct note> notes[4];
    GLFWwindow *window;

    void pmenu(void);
    void pchoose(void);
    void pgame(void);
    void bgame(void);

protected:
    void drawLine(Point3d &start, Point3d &end);

public:
    Program();
    ~Program();
    void run(void);

protected:
    void process(void);
    void brush(void);
};

int main(int argc, char *argv[])
{
    Program _program;
    _program.run();
    return 0;
}

void Program::process(void)
{
    while (status && !glfwWindowShouldClose(window))
    {
        while (flush == 1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        tick = std::chrono::system_clock::now();
        switch (status)
        {
        case 1:
            pmenu();
            break;
        case 2:
            pchoose();
            break;
        case 3:
            pgame();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    info("Control Process exiting...");
    return;
}

void Program::pmenu(void)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && temp[5] == 0)
    {
        --status, flush = 1, temp[5] = 1;
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && temp[4] == 0)
    {
        ++status, flush = 1, temp[4] = 1;
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE && temp[5] == 1)
        temp[5] = 0;
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE && temp[4] == 1)
        temp[4] = 0;
    return;
}
/*
int Program::readShader(const char *filename)
{
    int success;
    char infoLog[512];
    std::ifstream file;
    file.open(filename, std::ios::in);
    if (!file.is_open())
        return 1;
    file.seekg(0, std::ios::end);
    shader[count].length = file.tellg();
    file.seekg(0, std::ios::beg);
    shader[count].program = new char[shader[count].length];
    if (shader[count].program == nullptr)
    {
        file.close();
        return 1;
    }
    file.read(shader[count].program, shader[count].length);
    file.close();
    shader[count].shaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shader[count].shaderId, 1, &(shader[count].program), nullptr);
    glCompileShader(shader[count].shaderId);
    glGetShaderiv(shader[0].shaderId, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        info(infoLog);
        return 1;
    }
    glAttachShader(program, shader[count].shaderId);
    glDeleteShader(shader[count].shaderId);
    ++count;
    return 0;
}
*/
void Program::pchoose(void)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && temp[5] == 0)
    {
        --status, flush = 1, temp[5] = 1;
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && temp[4] == 0)
    {
        ++status, flush = 1, temp[4] = 1;
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE && temp[5] == 1)
        temp[5] = 0;
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE && temp[4] == 1)
        temp[4] = 0;
    return;
}

void Program::pgame(void)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && temp[5] == 0)
    {
        --status, flush = 1, temp[5] = 1;
        // to do something here...
        // this block is to set something that is quit music...
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE && temp[5] == 1)
        temp[5] = 0;
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE && temp[4] == 1)
        temp[4] = 0;
    for (int i = 0; i < 4; ++i)
    {
        if (glfwGetKey(window, KEY[i]) == GLFW_PRESS && temp[i] == 0)
            temp[i] = 1;
        if (glfwGetKey(window, KEY[i]) == GLFW_RELEASE)
            temp[i] = 0;
        if (notes[i].empty() || temp[i] == 0)
            continue;
        auto tOff = std::abs(notes[i].front().offset - std::chrono::duration_cast<std::chrono::milliseconds>(tick - startTick).count());
        if (tOff <= 80)
        {
            // todo here...
            notes[i].pop();
            continue;
        }
        if (tOff > 80 && tOff <= 200)
        {
            notes[i].pop();
            continue;
        }
        if (tOff > 200 && tOff <= 300)
        {
            notes[i].pop();
            continue;
        }
        if (tOff > 300)
        {
            continue;
        }
    }
    return;
}

void Program::bgame(void)
{
    static GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(game_vertices), game_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glUseProgram(program);
    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 8); // 绘制4条线，共8个顶点
    // TODO ...
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    return;
}

Program::Program(void)
{
    status = flag = 0;
    count = 1;
    handle = nullptr;
    window = nullptr;
    if (!glfwInit())
    {
        error("Failed to initialize GLFW!");
        return;
    }
    flag |= 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

Program::~Program()
{
    delete handle;
    status = 0;
    if (flag & 2)
        glfwDestroyWindow(window);
    if (flag & 1)
        glfwTerminate();
}

void Program::run(void)
{
    window = glfwCreateWindow(900, 600, "ForK - A single four key music game!", nullptr, nullptr);
    if (!window)
    {
        error("Failed to create GLFW window!");
        return;
    }
    info("Creating the main window...");
    flag |= 2;
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameSizeCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        error("Failed to initialize GLAD.");
        return;
    }
    flag |= 4, status = 1;
    program = glCreateProgram();
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
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        error(infoLog);
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        error(infoLog);
    }
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glLinkProgram(program);
    handle = new std::thread(process, this);
    brush();
    handle->join();
    glDeleteProgram(program);
    return;
}

void Program::brush(void)
{
    glUseProgram(program);
    static constexpr std::chrono::milliseconds GAP = std::chrono::milliseconds(10);
    frametick = std::chrono::system_clock::now();
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        if (!status)
            break;
        switch (status)
        {
        case 1:
            if (flush == 1)
                info("Status 1");
            break;
        case 2:
            if (flush == 1)
                info("Status 2");
            break;
        case 3:
            if (flush == 1)
                info("Status 3");
            bgame();
            break;
        }
        flush = 0;
        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_until(frametick + GAP);
        frametick = std::chrono::system_clock::now();
    }
    return;
}