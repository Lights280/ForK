#include "audio.h"


Audio::Audio()
{
    device = alcOpenDevice(nullptr);
    if (!device)
    {
        error("Failed to open OpenAL device");
        return;
    }
    context = alcCreateContext(device, nullptr);
    if (!context || !alcMakeContextCurrent(context))
    {
        error("Failed to create OpenAL context");
        alcCloseDevice(device);
        return;
    }
}

Audio::~Audio()
{
    for (auto &audio : audioObjects)
    {
        alDeleteSources(1, &audio.source);
        alDeleteBuffers(1, &audio.buffer);
    }
    alcDestroyContext(context);
    alcCloseDevice(device);
}

uint32_t Audio::generate(const char *filePath)
{
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file)
    {
        error("Failed to open file.");
        return -1;
    }
    size_t fileSize = static_cast<size_t>(file.tellg());
    file.seekg(0, std::ios::beg);
    std::vector<char> pcmData(fileSize);
    if (!file.read(pcmData.data(), fileSize))
    {
        error("Failed to read PCM data...");
        return -1;
    }
    AudioObject newAudio;
    alGenBuffers(1, &newAudio.buffer);
    alBufferData(newAudio.buffer, AL_FORMAT_STEREO16, pcmData.data(), static_cast<ALsizei>(pcmData.size()), 44100);
    alGenSources(1, &newAudio.source);
    alSourcei(newAudio.source, AL_BUFFER, newAudio.buffer);
    alSourcef(newAudio.source, AL_PITCH, 1.0f);
    alSourcef(newAudio.source, AL_GAIN, 1.0f);
    alSource3f(newAudio.source, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSource3f(newAudio.source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alSourcei(newAudio.source, AL_LOOPING, AL_FALSE);
    audioObjects.push_back(std::move(newAudio));
    return audioObjects.size() - 1;
}

void Audio::play(uint32_t id)
{
    if (id >= audioObjects.size())
    {
        error("Invalid audio ID.");
        return;
    }
    ALint state;
    alGetSourcei(audioObjects[id].source, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING)
        alSourcePlay(audioObjects[id].source);
    return;
}

void Audio::stop(uint32_t id)
{
    if (id >= audioObjects.size())
    {
        error("Invalid audio ID.");
        return;
    }
    alSourceStop(audioObjects[id].source);
    return;
}
