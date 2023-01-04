#pragma once
#include <AL\alc.h>

class SoundDevice
{
public:
    static SoundDevice* get();

    void GetLocation(float& x, float& y, float& z);
    void SetLocation(const float& x, const float& y, const float& z);


private:
    SoundDevice();
    ~SoundDevice();

    ALCdevice* p_ALCDevice;
    ALCcontext* p_ALCContext;

};