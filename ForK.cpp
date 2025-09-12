#include "glad/glad.h"
#include "alc.h"
#include "al.h"
#include "glfw3.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include <vector>
#include <deque>
#include <cmath>
#include <cstring>

const GLchar *shader1 = new char[123]{"#version 330 core\n"
                                      "layout (location = 0) in vec3 aPos;\n"
                                      "void main()\n"
                                      "{\n"
                                      "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                      "}\0"};
constexpr float speed = 0.003f;
constexpr GLuint VSIZE = sizeof(float) * 3;
static constexpr int KEY[4] = {GLFW_KEY_D, GLFW_KEY_F, GLFW_KEY_J, GLFW_KEY_K};
static constexpr float game_vertices[] = {
    // stage 3 vertex group
    // Line 1
    -0.2f,
    -1.0f,
    0.0f,
    -0.2f,
    1.0f,
    0.0f,
    // Line 2
    0.1f,
    -1.0f,
    0.0f,
    0.1f,
    1.0f,
    0.0f,
    // Line 3
    0.4f,
    -1.0f,
    0.0f,
    0.4f,
    1.0f,
    0.0f,
    // Line 4
    0.7f,
    -1.0f,
    0.0f,
    0.7f,
    1.0f,
    0.0f,
    // Line 5
    1.0f,
    -1.0f,
    0.0f,
    1.0f,
    1.0f,
    0.0f,
    // Line 6
    -0.2f,
    -0.8f,
    0.0f,
    1.0f,
    -0.8f,
    0.0f,
    // Quad 1
    -0.2f,
    -0.8f,
    0.0f,
    -0.2f,
    -1.0f,
    0.0f,
    0.1f,
    -0.8f,
    0.0f,
    0.1f,
    -1.0f,
    0.0f,
    // Quad 2
    0.1f,
    -0.8f,
    0.0f,
    0.1f,
    -1.0f,
    0.0f,
    0.4f,
    -0.8f,
    0.0f,
    0.4f,
    -1.0f,
    0.0f,
    // Quad 3
    0.4f,
    -0.8f,
    0.0f,
    0.4f,
    -1.0f,
    0.0f,
    0.7f,
    -0.8f,
    0.0f,
    0.7f,
    -1.0f,
    0.0f,
    // Quad 4
    0.7f,
    -0.8f,
    0.0f,
    0.7f,
    -1.0f,
    0.0f,
    1.0f,
    -0.8f,
    0.0f,
    1.0f,
    -1.0f,
    0.0f,
};

struct note
{
    int offset; // offset (milliseconds)
    int type;
    note(const int &off, const int &tp) noexcept
    {
        offset = off, type = tp;
    }
};

struct Shader
{
    char *program;
    int length;
    int shaderId;
};

auto frameSizeCallback = [](GLFWwindow *, int width, int height)
{ glViewport(0, 0, width, height); };

inline constexpr void error(const char *message) noexcept
{
    std::cerr << "[Error]:" << message << std::endl;
}

inline constexpr void info(const char *message) noexcept
{
    std::cerr << "[Info]:" << message << std::endl;
}

class Program
{
private:
    int program;
    int count;
    int status;
    int flush;
    int temp[10];
    int flag;
    int allocation3;
    GLuint VAO[2], VBO[2];
    std::thread *handle;
    std::chrono::system_clock::time_point startTick, tick, frameTick;
    std::deque<note> notes[4];
    GLFWwindow *window;

    void pmenu(void);
    void pchoose(void);
    void pgame(void);
    void bgame(void);

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
        if (!notes[i].empty() && notes[i].front().offset - std::chrono::duration_cast<std::chrono::milliseconds>(tick - startTick).count() < -160)
        {
            info("Miss!");
            notes[i].pop_front();
            continue;
        }
        if (notes[i].empty() || temp[i] == 0)
            continue;
        auto tOff = std::abs(notes[i].front().offset - std::chrono::duration_cast<std::chrono::milliseconds>(tick - startTick).count());
        if (tOff <= 80)
        {
            info("Perfect!");
            notes[i].pop_front();
            continue;
        }
        if (tOff > 80 && tOff <= 160)
        {
            info("Good!");
            notes[i].pop_front();
            continue;
        }
        if (tOff > 160 && tOff <= 300)
        {
            info("Bad.");
            notes[i].pop_front();
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
    allocation3 = sizeof(game_vertices);
    glUseProgram(program);
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glDrawArrays(GL_LINES, 0, 12);
    for (int i = 0; i < 4; ++i)
    {
        if (temp[i] == 1)
        {
            glDrawArrays(GL_TRIANGLES, 12 + (i << 2), 3);
            glDrawArrays(GL_TRIANGLES, 13 + (i << 2), 3);
        }
        glBindVertexArray(VAO[1]);
        glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
        std::vector<float> key;
        auto _push = [&key](const float &x, const float &y)
        { key.push_back(x), key.push_back(y), key.push_back(0.0f); };
        for (const auto &_nt : notes[i])
        {
            auto delta = _nt.offset - std::chrono::duration_cast<std::chrono::milliseconds>(tick - startTick).count();
            if (delta > 1.8 / speed || delta < -0.4 / speed)
                continue;
            float x1 = -0.2f + 0.3f * i, x2 = 0.1f + 0.3f * i;
            float y1 = -0.725f + (delta * speed), y2 = -0.8f + (delta * speed);
            _push(x1, y1);
            _push(x1, y2);
            _push(x2, y1);
            _push(x1, y2);
            _push(x2, y1);
            _push(x2, y2);
        }
        glBufferSubData(GL_ARRAY_BUFFER, 0, key.size() * sizeof(float), key.data());
        glDrawArrays(GL_TRIANGLES, 0, key.size());
    }
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    return;
}

Program::Program(void)
{
    status = flag = allocation3 = 0, count = 1;
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
    window = glfwCreateWindow(1600, 900, "ForK - A single four key music game!", nullptr, nullptr);
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
        std::cerr << "[ERROR]Shader program linking failed: " << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glLinkProgram(program);
    glLineWidth(3.0f);
    // test
    for (int i = 0; i < 20; ++i)
    {
        notes[0].push_back(note(i * 1000 + 2000, 1));
    }
    handle = new std::thread(process, this);
    brush();
    handle->join();
    glDeleteProgram(program);
    return;
}

void Program::brush(void)
{
    glUseProgram(program);
    glGenVertexArrays(2, VAO);
    glGenBuffers(1, &VBO[0]);
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(game_vertices), game_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
    glGenBuffers(1, &VBO[1]);
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, VSIZE * 4096, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    static constexpr std::chrono::milliseconds GAP = std::chrono::milliseconds(10);
    frameTick = std::chrono::system_clock::now();
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
            {
                startTick = std::chrono::system_clock::now();
                info("Status 3");
            }
            bgame();
            break;
        }
        flush = 0;
        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_until(frameTick + GAP);
        frameTick = std::chrono::system_clock::now();
    }
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(1, &VBO[0]);
    glDeleteBuffers(1, &VBO[1]);
    return;
}