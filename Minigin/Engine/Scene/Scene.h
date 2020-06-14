#pragma once
#include <functional>
#include <map>

namespace MyEngine
{
	class GameObject;
	class Scene final
	{
	public:
		void Add(GameObject* pObject, float depth = 0.0f);
		void Invoke(std::function<void()> func, float delay);

		Scene(const std::string& name, const bool removeOnDeactivate = false);
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		const std::string& GetName() const { return m_Name; }
		const bool GetShouldRemove() const { return m_RemoveOnDeactivate; }

		void Update(const float);
		void FixedUpdate(const float);
		void Render() const;

	private: 
		friend class SceneManager;
		std::string m_Name;
		std::vector <std::pair<GameObject*, float>> m_Objects{};
		const bool m_RemoveOnDeactivate;
		std::vector<std::pair<std::function<void()>, float>> m_Invokers;

		static unsigned int m_IdCounter; 
	};

}
