#pragma once
struct SDL_Window;
namespace MyEngine
{
	class Minigin
	{
	public:
		void Initialize(const std::string& dataPath, const char* windowTitle, const int windowWidth, const int windowHeight, const float ppm, const std::string& ignoreScene);
		void LoadGame() const;
		void Cleanup();
		void Run();
		static const float SecPerFrame; //0.016 for 60 fps, 0.033 for 30 fps
	private:
		SDL_Window* m_pWindow{};
	};
}