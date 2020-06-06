#pragma once
struct SDL_Window;
namespace MyEngine
{
	class Minigin
	{
	public:
		void Initialize(const std::string& dataPath);
		void LoadGame() const;
		void Cleanup();
		void Run();
	private:
		static const float SecPerFrame; //0.016 for 60 fps, 0.033 for 30 fps
		SDL_Window* m_Window{};
	};
}