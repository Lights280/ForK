#include "program.h"


void Program::pmain(void)
{
    if (graphic.processKey(GLFW_KEY_ESCAPE) == 1)
        audio.stop(audioId[0]), changeStatus(-1);
    if (graphic.processKey(GLFW_KEY_ENTER) == 1)
        audio.stop(audioId[0]), changeStatus(1);
}

void Program::pmenu(void)
{
    if (graphic.processKey(GLFW_KEY_ESCAPE) == 1)
        changeStatus(-1);
    if (graphic.processKey(GLFW_KEY_ENTER) == 1)
        changeStatus(1);
    return;
}

void Program::pgame(void)
{
    GLuint sts = 0b0000;
    if (graphic.processKey(GLFW_KEY_ESCAPE) == 1)
    {
        changeStatus(-1);
        std::printf("1\n");
        for (int i = 0; i < 4; ++i)
            notes[i].clear();
        return;
    }
    for (int i = 0; i < 4; ++i)

    {
        key[i] = graphic.processKey(KEY[i]);
        if (notes[i].empty())
        {
            sts |= 1 << i;
            continue;
        }
        if (notes[i].front().offset - std::chrono::duration_cast<std::chrono::milliseconds>(nowTick - startTick).count() < -160)
        {
            info("Miss!");
            notes[i].pop_front();
            continue;
        }
        if (key[i] == 0)
            continue;
        auto tOff = std::abs(notes[i].front().offset - std::chrono::duration_cast<std::chrono::milliseconds>(nowTick - startTick).count());
        if (key[i] == 1)
        {
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
                continue;
        }
    }
    if (sts == 0b1111)
    {
        changeStatus(1);
        std::printf("2\n");
        for (int i = 0; i < 4; ++i)
            notes[i].clear();
    }
    return;
}

void Program::pend()
{
    if (graphic.processKey(GLFW_KEY_ESCAPE) == 1)
        changeStatus(-1);
    if (graphic.processKey(GLFW_KEY_ENTER) == 1)
        changeStatus(-2);
    return;
}

Program::Program()
{
    status = 0;
    audioId.resize(10);
    audioId[0] = audio.generate("bgm.pcm");
    threadProcess = nullptr;
}

void Program::run(void)
{
    flush = status = 1;
    graphic.buildProgram();
    threadProcess = new std::thread(process, this);
    brush();
    threadProcess->join();
    return;
}

Program::~Program()
{
    delete threadProcess;
    threadProcess = nullptr;
}

void Program::process(void)
{
    while (status && !glfwWindowShouldClose(graphic.getWindow()))
    {
        while (flush == 1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        nowTick = std::chrono::system_clock::now();
        switch (status)
        {
        case 1:
            pmain();
            break;
        case 2:
            pmenu();
            break;
        case 3:
            pgame();
            break;
        case 4:
            pend();
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    info("Control Process exiting...");
    return;
}

void Program::brush(void)
{
    graphic.useProgram();
    graphicId.resize(10);
    graphicId[0] = graphic.generate(1024, GL_STATIC_DRAW);
    graphicId[1] = graphic.generate(1024, GL_DYNAMIC_DRAW);
    graphic.bindData(game_vertices, sizeof(game_vertices) / VSIZE, 0, graphicId[0]);
    glLineWidth(2.15f);
    static constexpr std::chrono::milliseconds GAP = std::chrono::milliseconds(10);
    frameTick = std::chrono::system_clock::now();
    while (!glfwWindowShouldClose(graphic.getWindow()))
    {
        if (!status)
            break;
        switch (status)
        {
        case 1:
            if (flush == 1)
            {
                info("Status 1");
                audio.play(audioId[0]);
            }
            break;
        case 2:
            if (flush == 1)
            {
                info("Status 2");
            }
            break;
        case 3:
            if (flush == 1)
            {
                startTick = std::chrono::system_clock::now();
                info("Status 3");
                for (int i = 0; i < 20; ++i)
                    notes[i % 4].push_back(note(i * 750 + 2500, 1));
            }
            bgame();
            break;
        case 4:
            if (flush == 1)
            {
                info("Status 4");
            }
            bend();
            break;
        }
        flush = 0;
        graphic.clear();
        std::this_thread::sleep_until(frameTick + GAP);
        frameTick = std::chrono::system_clock::now();
    }
    info("Brush Process exiting...");
    return;
}

void Program::bgame(void)
{
    graphic.drawLines(graphicId[0], 0, 6);
    for (int i = 0; i < 4; ++i)
        if (key[i])
            graphic.drawRectangle4(graphicId[0], 12 + (i << 2), 1);
    for (int i = 0; i < 4; ++i)
    {
        std::vector<float> key;
        static auto _push = [&key](const float &x, const float &y)
        { key.push_back(x), key.push_back(y), key.push_back(0.0f); };
        for (const auto &_nt : notes[i])
        {
            auto delta = _nt.offset - std::chrono::duration_cast<std::chrono::milliseconds>(nowTick - startTick).count();
            if (delta > 1.8 / SPEED || delta < -0.4 / SPEED)
                continue;
            float x1 = -0.2f + 0.3f * i, x2 = 0.1f + 0.3f * i;
            float y1 = -0.7f + (delta * SPEED), y2 = -0.8f + (delta * SPEED);
            _push(x1, y1), _push(x1, y2), _push(x2, y1), _push(x2, y2);
        }
        graphic.bindData(key.data(), key.size() / 3, 0, graphicId[1]);
        graphic.drawRectangle4(graphicId[1], 0, key.size() / (3 * 4));
    }
    return;
}

void Program::bend(void)
{
    return;
}