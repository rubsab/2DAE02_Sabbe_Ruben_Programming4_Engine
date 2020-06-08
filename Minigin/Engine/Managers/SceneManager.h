#pragma once
#include "../Helpers/Singleton.h"

namespace MyEngine
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		SceneManager() = default;
		Scene& CreateScene(const std::string& name);
		~SceneManager();

		void Update(const float deltaTime);
		void FixedUpdate(const float fixedDeltaTime);
		void Render();
	private:
		std::vector<Scene*> m_Scenes;
	};
}
