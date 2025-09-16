#pragma once
#include "define.h"

class Audio
{
private:
    struct AudioObject
    {
        ALuint buffer = 0;
        ALuint source = 0;
    };
    ALCdevice *device = nullptr;
    ALCcontext *context = nullptr;
    std::vector<AudioObject> audioObjects;

public:
    Audio();
    ~Audio();
    uint32_t generate(const char *filePath);
    void play(uint32_t id);
    void stop(uint32_t id);
};