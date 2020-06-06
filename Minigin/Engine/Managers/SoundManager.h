#pragma once
#include "../Helpers/Singleton.h"
#include <vector>

namespace MyEngine
{
	class SoundEffect;
	class SoundStream;

	class SoundManager final : public Singleton<SoundManager>
	{
	public:
		SoundManager() = default;
		void Init(const std::string& data);
		~SoundManager();

		SoundEffect* LoadSoundEffect(const std::string& file);
		SoundStream* LoadSoundStream(const std::string& file);
	private:
		std::vector<SoundEffect*> m_SoundEffects;
		std::vector<SoundStream*> m_SoundStreams;
		std::string m_DataPath;
	};
}
