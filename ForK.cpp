#include "glad/glad.h"
#include "alc.h"
#include "al.h"
#include "glfw3.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <queue>
#include <cmath>

static constexpr int KEY[4] = {GLFW_KEY_D, GLFW_KEY_F, GLFW_KEY_J, GLFW_KEY_K};

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

struct note
{
    int offset; // offset (milliseconds)
    int type;
};

class Program
{
private:
    int status;
    int flush; // status has been changed but not changes is applied.
    int temp[4];
    int flag;
    std::thread *handle;
    std::chrono::system_clock::time_point startTick, tick, frametick;
    std::queue<struct note> notes[4];
    GLFWwindow *window;

    void pmenu(void);
    void pchoose(void);
    void pgame(void);
    void bgame(void);

public:
    Program();
    ~Program();
    void run();

protected:
    void process();
    void brush(void);
};

int main(int argc, char *argv[])
{
    Program program;
    program.run();
    return 0;
}

void Program::process(void)
{
    while (status && !glfwWindowShouldClose(window))
    {
        if (flush == 1)
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
    return;
}

void Program::pmenu(void)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        status = 0;
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        status = 2;
        flush = 1;
    }
    return;
}

void Program::pchoose(void)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        status = 2;
        flush = 1;
        return;
    }
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        status = 3;
        flush = 1;
        return;
    }
}

void Program::pgame(void)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        status = 2;
        flush = 1;
        // to do here...
        // this way is for quit.
        return;
    }
    for (int i = 0; i < 4; ++i)
    {
        if (glfwGetKey(window, KEY[i]) == GLFW_PRESS && temp[i] == 0)
        {
            temp[i] = 1;
        }
        if (glfwGetKey(window, KEY[i]) == GLFW_RELEASE)
        {
            temp[i] = 0;
        }
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
    return;
}

Program::Program()
{
    status = flag = 0;
    handle = nullptr;
    window = nullptr;
    if (!glfwInit())
    {
        error("Failed to initialize GLFW!");
        return;
    }
    flag |= 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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
    handle = new std::thread(process, this);
    brush();
    handle->join();
    return;
}

void Program::brush(void)
{
    static constexpr std::chrono::milliseconds GAP = std::chrono::milliseconds(10);
    frametick = std::chrono::system_clock::now();
    while (!glfwWindowShouldClose(window))
    {
        if (!status)
            break;
        switch (status)
        {
        case 1:
            if (flush == 1)
            {
                info("Status 1");
                flush = 0;
            }
            break;
        case 2:
            if (flush == 1)
            {
                info("Status 2");
                glColor3f(0.0, 0.0, 0.0);
                flush = 0;
            }
            break;
        case 3:
            if (flush == 1)
            {
                info("Status 3");
                glColor3f(1.0, 1.0, 1.0);
                glLineWidth(2.0f);
                glBegin(GL_LINE);
                // line 1
                glVertex2d(-0.2, -1.0);
                glVertex2d(-0.2, 1.0);
                // line 2
                glVertex2d(0.2, -1.0);
                glVertex2d(0.2, 1.0);
                // line 3
                glVertex2d(0.6, -1.0);
                glVertex2d(0.6, 1.0);
                // line 3
                glVertex2d(1.0, -1.0);
                glVertex2d(1.0, 1.0);
                glEnd();
                flush = 0;
            }
            bgame();
            break;
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
        std::this_thread::sleep_until(frametick + GAP);
        frametick = std::chrono::system_clock::now();
    }
    return;
}