#include "AudioObject.h"
#include <cstddef>
#include <AL\alext.h>
#include <malloc.h>
#include <QDebug>
#include "visualobject.h"

void AudioObject::Play()
{
	ALsizei i;

	// clear any al errors
	alGetError();

	/* Rewind the source position and clear the buffer queue */
	alSourceRewind(p_Source);
	alSourcei(p_Source, AL_BUFFER, 0);

	/* Fill the buffer queue */
	for (i = 0; i < NUM_BUFFERS; i++)
	{
		/* Get some data to give it to the buffer */
		sf_count_t slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
		if (slen < 1) break;

		slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
		alBufferData(p_Buffers[i], p_Format, p_Membuf, (ALsizei)slen, p_Sfinfo.samplerate);
	}
	if (alGetError() != AL_NO_ERROR)
	{
		throw("Error buffering for playback");
	}

	/* Now queue and start playback! */
	alSourceQueueBuffers(p_Source, i, p_Buffers);
	alSourcePlay(p_Source);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("Error starting playback");
	}

}

void AudioObject::SetPosition(float x, float y, float z)
{
	alSource3f(p_Source, AL_POSITION, x, y, z);
}

void AudioObject::LoopSound(bool loop)
{
	alSourcei(p_Source, AL_LOOPING, loop);
}

void AudioObject::Pause()
{
	ALint state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	if(state == AL_PLAYING)
		alSourcePause(p_Source);
}

void AudioObject::Stop()
{
	//ALint state;
	//alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	//if (state == AL_PLAYING)
	//	alSourceStop(p_Source);
}

void AudioObject::Resume()
{
	ALint state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	if (state == AL_PAUSED)
		alSourcePlay(p_Source);
}

void AudioObject::SetVolume(float NewVolume)
{
	p_Gain = NewVolume;
	alSourcef(p_Source, AL_GAIN, p_Gain);
}

void AudioObject::SetAttenuation(float ReferenceDistance, float Range)
{
	p_ReferenceDistance = ReferenceDistance;
	p_MaxRange = Range;
	alSourcef(p_Source, AL_REFERENCE_DISTANCE, p_ReferenceDistance);
	alSourcef(p_Source, AL_MAX_DISTANCE, p_MaxRange);
}

void AudioObject::UpdateBufferStream()
{
	if (m_AttachTo)
	{
		SetPosition(m_AttachTo->mMatrix.getPosition().getX(),
			m_AttachTo->mMatrix.getPosition().getY(),
			m_AttachTo->mMatrix.getPosition().getZ());
	}

	ALint processed, state;

	// clear error 
	alGetError();
	/* Get relevant source info */
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	alGetSourcei(p_Source, AL_BUFFERS_PROCESSED, &processed);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error checking source state");
	}

	/* Unqueue and handle each processed buffer */
	while (processed > 0)
	{
		ALuint bufid;
		sf_count_t slen;

		alSourceUnqueueBuffers(p_Source, 1, &bufid);
		processed--;

		/* Read the next chunk of data, refill the buffer, and queue it
		 * back on the source */
		slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
		if (slen > 0)
		{
			slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufid, p_Format, p_Membuf, (ALsizei)slen,
				p_Sfinfo.samplerate);
			alSourceQueueBuffers(p_Source, 1, &bufid);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			throw("error buffering data");
		}
	}

	/* Make sure the source hasn't underrun */
	if (state != AL_PLAYING && state != AL_PAUSED)
	{
		ALint queued;

		/* If no buffers are queued, playback is finished */
		alGetSourcei(p_Source, AL_BUFFERS_QUEUED, &queued);
		if (queued == 0)
			return;



		alSourcePlay(p_Source);
		if (alGetError() != AL_NO_ERROR)
		{
			throw("error restarting playback");
		}
	}

}

ALint AudioObject::getSource()
{
	return p_Source;
}

AudioObject::AudioObject(const char* filename)
{
	Construct(filename);
}

AudioObject::AudioObject(VisualObject* AttachTo, const char* filename)
{
	Construct(filename);
	AttachAudioToObject(AttachTo);
}

void AudioObject::AttachAudioToObject(VisualObject* AttachTo)
{
	m_AttachTo = AttachTo;
}

void AudioObject::Construct(const char* filename)
{
	alGenSources(1, &p_Source);
	SetValues();

	alGenBuffers(NUM_BUFFERS, p_Buffers);

	std::size_t frame_size;

	p_SndFile = sf_open(filename, SFM_READ, &p_Sfinfo);
	if (!p_SndFile)
	{
		throw("could not open provided file -- check path");
	}

	/* Get the sound format, and figure out the OpenAL format */
	if (p_Sfinfo.channels == 1)
		p_Format = AL_FORMAT_MONO16;
	else if (p_Sfinfo.channels == 2)
		p_Format = AL_FORMAT_STEREO16;
	else if (p_Sfinfo.channels == 3)
	{
		if (sf_command(p_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			p_Format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (p_Sfinfo.channels == 4)
	{
		if (sf_command(p_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			p_Format = AL_FORMAT_BFORMAT3D_16;
	}
	if (!p_Format)
	{
		sf_close(p_SndFile);
		p_SndFile = NULL;
		throw("Unsupported channel count from file");
	}

	frame_size = ((size_t)BUFFER_SAMPLES * (size_t)p_Sfinfo.channels) * sizeof(short);
	p_Membuf = static_cast<short*>(malloc(frame_size));
}

AudioObject::~AudioObject()
{
	alDeleteSources(1, &p_Source);

	if (p_SndFile)
		sf_close(p_SndFile);

	p_SndFile = nullptr;

	free(p_Membuf);

	alDeleteBuffers(NUM_BUFFERS, p_Buffers);

}

void AudioObject::SetValues()
{
	alSourcef(p_Source, AL_PITCH, p_Pitch);
	alSourcef(p_Source, AL_GAIN, p_Gain);
	alSource3f(p_Source, AL_POSITION, p_Position[0], p_Position[1], p_Position[2]);
	alSource3f(p_Source, AL_VELOCITY, p_Velocity[0], p_Velocity[1], p_Velocity[2]);
	alSourcef(p_Source, AL_REFERENCE_DISTANCE, p_ReferenceDistance);
	alSourcef(p_Source, AL_MAX_DISTANCE, p_MaxRange);
	alSourcei(p_Source, AL_LOOPING, p_LoopSound);
}
