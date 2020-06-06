#pragma once
struct Mix_Chunk;
typedef struct _Mix_Music Mix_Music;

namespace MyEngine
{
	class SoundEffect
	{
	public:
		explicit SoundEffect(const std::string& path);
		~SoundEffect();
		SoundEffect(const SoundEffect& other) = delete;
		SoundEffect& operator=(const SoundEffect& rhs) = delete;
		SoundEffect(SoundEffect&& other) = delete;
		SoundEffect& operator=(SoundEffect&& rhs) = delete;

		bool Play(int loops);
		void SetVolume(int value);
		int GetVolume() const;
		void Stop();
		void Pause();
		void Resume();

	private:
		friend class SoundManager;
		Mix_Chunk* m_pMixChunk;
		int m_Channel;
		const std::string m_Path;
	};

	class SoundStream
	{
	public:
		explicit SoundStream(const std::string& path);
		~SoundStream();
		SoundStream(const SoundStream& other) = delete;
		SoundStream& operator=(const SoundStream& rhs) = delete;
		SoundStream(SoundStream&& other) = delete;
		SoundStream& operator=(SoundStream&& other) = delete;

		bool Play(bool repeat) const;
		static void Stop();
		static void Pause();
		static void Resume();
		static void SetVolume(int value);
		static int GetVolume();
		static bool IsPlaying();

	private:
		friend class SoundManager;
		Mix_Music* m_pMixMusic;
		const std::string m_Path;
	};
}
