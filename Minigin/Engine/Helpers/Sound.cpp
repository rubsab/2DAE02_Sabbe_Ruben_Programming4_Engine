#include "MiniginPCH.h"
#include "Sound.h"
#include "SDL_mixer.h"
#include "Logger.h"

MyEngine::SoundEffect::SoundEffect(const  std::string& path)
	:m_pMixChunk{ Mix_LoadWAV(path.c_str()) },
	m_Path(path)
{
	if (m_pMixChunk == nullptr)
		Logger::LogError("SoundEffect: Failed to load " + path + ",\nSDL_mixer Error: " + Mix_GetError());
}
MyEngine::SoundEffect::~SoundEffect()
{
	Mix_FreeChunk(m_pMixChunk);
	m_pMixChunk = nullptr;
}

bool MyEngine::SoundEffect::Play(int loops)
{
	if (m_pMixChunk != nullptr)
	{
		m_Channel = Mix_PlayChannel(-1, m_pMixChunk, loops);
		return m_Channel == -1 ? false : true;
	}
	else
	{
		return false;
	}
}

void MyEngine::SoundEffect::SetVolume(int value)
{
	if (m_pMixChunk != nullptr)
	{
		Mix_VolumeChunk(m_pMixChunk, value);
	}
}

int MyEngine::SoundEffect::GetVolume() const
{
	if (m_pMixChunk != nullptr)
	{
		return Mix_VolumeChunk(m_pMixChunk, -1);
	}
	else
	{
		return -1;
	}
}

void MyEngine::SoundEffect::Stop()
{
	Mix_HaltChannel(m_Channel);
}

void MyEngine::SoundEffect::Pause()
{
	Mix_Pause(m_Channel);
}
void MyEngine::SoundEffect::Resume()
{
	Mix_Resume(m_Channel);
}

MyEngine::SoundStream::SoundStream(const std::string& path)
	:m_pMixMusic{ Mix_LoadMUS(path.c_str()) },
	m_Path(path)
{
	if (m_pMixMusic == nullptr)
		Logger::LogError("SoundStream: Failed to load " + path + ",\nSDL_mixer Error: " + Mix_GetError());
}

MyEngine::SoundStream::~SoundStream()
{
	Mix_FreeMusic(m_pMixMusic);
	m_pMixMusic = nullptr;
}

bool MyEngine::SoundStream::Play(bool repeat) const
{
	if (m_pMixMusic != nullptr)
	{
		int result{ Mix_PlayMusic(m_pMixMusic, repeat ? -1 : 1) };
		return result == 0 ? true : false;
	}
	else
	{
		return false;
	}
}

void MyEngine::SoundStream::Stop()
{
	Mix_HaltMusic();
}
void MyEngine::SoundStream::Pause()
{
	Mix_PauseMusic();
}

void MyEngine::SoundStream::Resume()
{
	Mix_ResumeMusic();
}

int MyEngine::SoundStream::GetVolume()
{
	return Mix_VolumeMusic(-1);
}

bool MyEngine::SoundStream::IsPlaying()
{
	return Mix_PlayingMusic() == 0 ? false : true;
}

void MyEngine::SoundStream::SetVolume(int value)
{
	Mix_VolumeMusic(value);
}
