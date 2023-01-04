#pragma once
#include <AL\al.h>
class SoundSource
{
public:
    SoundSource();
    ~SoundSource();

    void Play(const ALuint buffer_to_play);
    void Stop();
    void Pause();
    void Resume();

    void SetPosition(const float& x, const float& y, const float& z);

private:
    ALuint p_Source;
    float p_Pitch = 1.f;
    float p_Gain = 1.f;
    float p_Position[3] = { 3,0,0 };
    float p_Velocity[3] = { 0,0,0 };
    bool p_LoopSound = false;
    ALuint p_Buffer = 0;
};