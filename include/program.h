#pragma once
#include "audio.h"
#include "graphic.h"

struct note
{
    int offset;
    int type;
    note(const int &off, const int &tp) noexcept
    {
        offset = off, type = tp;
    }
};

class Program
{
private:
    int status;
    int flush;
    int key[10];
    Graphic graphic;
    Audio audio;
    std::vector<int> audioId;
    std::vector<int> graphicId;
    std::thread *threadProcess;
    std::deque<note> notes[4];
    std::chrono::system_clock::time_point startTick, frameTick, nowTick;
    const std::function<void(int)> changeStatus = [this](int adder)
    { status += adder, flush = 1; };
    void pmain(void);
    void pmenu(void);
    void pgame(void);
    void pend(void);
    void bmain(void);
    void bmenu(void);
    void bgame(void);
    void bend(void);

public:
    Program();
    ~Program();
    void run(void);

protected:
    void process();
    void brush();
};