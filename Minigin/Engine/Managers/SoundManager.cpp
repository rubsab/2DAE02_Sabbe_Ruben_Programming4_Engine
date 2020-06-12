#include "MiniginPCH.h"
#include "SoundManager.h"
#include "../Helpers/Sound.h"
#include "SDL_mixer.h"
#include "../Helpers/Logger.h"

void MyEngine::SoundManager::Init(const std::string& data)
{
	m_DataPath = data;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		Logger::LogError("Core::Initialize( ), error when calling Mix_OpenAudio: " + std::string(Mix_GetError()));
		return;
	}
}

void MyEngine::SoundManager::Notify(const Event event, const int value, const GameObject*)
{
	for (SoundEffect* soundEffect : m_SoundEffects[event])
		soundEffect->Play(value);
	for (SoundStream* soundStream : m_SoundStreams[event])
		soundStream->Play(bool(value));
}

MyEngine::SoundManager::~SoundManager()
{
	for (const std::pair<const Event, std::vector<SoundEffect*>>& pair : m_SoundEffects)
	{
		for (SoundEffect* pSoundEffect : pair.second)
			Safe_Delete(pSoundEffect);
	}
	for (const std::pair<const Event, std::vector<SoundStream*>>& pair : m_SoundStreams)
	{
		for (SoundStream* pSoundStream : pair.second)
			Safe_Delete(pSoundStream);
	}
}

MyEngine::SoundEffect* MyEngine::SoundManager::LoadSoundEffect(const std::string& file, const Event event)
{
	m_SoundEffects[event].push_back(new MyEngine::SoundEffect(m_DataPath + file));
	return m_SoundEffects[event].back();
}

MyEngine::SoundStream* MyEngine::SoundManager::LoadSoundStream(const std::string& file, const Event event)
{
	m_SoundStreams[event].push_back(new MyEngine::SoundStream(m_DataPath + file));
	return m_SoundStreams[event].back();
}
