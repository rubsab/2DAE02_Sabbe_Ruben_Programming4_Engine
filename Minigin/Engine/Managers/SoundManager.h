#pragma once
#include <vector>
#include <map>
#include "../Helpers/Observer.h"

namespace MyEngine
{
	class SoundEffect;
	class SoundStream;

	class SoundManager final : public Observer<SoundManager>
	{
	public:
		SoundManager() = default;
		void Init(const std::string& data);
		void Notify(const Event event, const int value = 0, const GameObject* object = nullptr) override;

		~SoundManager();

		SoundEffect* LoadSoundEffect(const std::string& file, const Event event);
		SoundStream* LoadSoundStream(const std::string& file, const Event event);
	private:
		std::map<const Event, std::vector<SoundEffect*>> m_SoundEffects;
		std::map<const Event, std::vector<SoundStream*>> m_SoundStreams;
		std::string m_DataPath;
	};
}
