#pragma once
#include <AL\al.h>
#include <sndfile.h>

class VisualObject;

class MusicBuffer
{
public:
	void Play();
	void SetPosition(float x, float y, float z);
	void LoopSound(bool loop);
	void Pause();
	void Stop();
	void Resume();
	//void SetVolume()

	void UpdateBufferStream();

	ALint getSource();

	MusicBuffer(const char* filename);
	MusicBuffer(VisualObject* AttachTo, const char* filename);
	void AttachAudioToObject(VisualObject* AttachTo);
	void Construct(const char* filename);
	~MusicBuffer();

	void SetValues();

private:
	ALuint p_Source;
	static const int BUFFER_SAMPLES = 8192;
	static const int NUM_BUFFERS = 4;
	ALuint p_Buffers[NUM_BUFFERS];
	SNDFILE* p_SndFile;
	SF_INFO p_Sfinfo;
	short* p_Membuf;
	ALenum p_Format;

	float p_Pitch = 1.f;
	float p_Gain = 1.f;
	float p_Position[3] = { 0,0,0 };
	float p_Velocity[3] = { 0,0,0 };
	bool p_LoopSound = false;

	VisualObject* m_AttachTo = nullptr;

	MusicBuffer() = delete;
};
