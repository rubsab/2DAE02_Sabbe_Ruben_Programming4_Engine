#pragma once
#include "../Helpers/Singleton.h"

namespace MyEngine
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		SceneManager() = default;
		void AddScene(Scene* pScene);
		Scene* RemoveScene(const std::string& name);
		void SetSceneActive(const std::string& name);
		void SetSceneActiveDelayed(const std::string& name, float delay);
		Scene* GetActiveScene() const { return m_Scenes[m_ActiveScene]; };
		~SceneManager();

		void Update(const float deltaTime);
		void FixedUpdate(const float fixedDeltaTime);
		void Render() const;
	private:
		std::vector<Scene*> m_Scenes;
		int m_ActiveScene{ -1 };
		std::vector<std::string> m_ToDelete;

		void DeleteToDeletes();
		float m_DelayTimer;
		std::string m_DelayedName;
	};
}
