#pragma once

namespace MyEngine
{
	class GameObject;
	class Scene
	{
	public:
		void Add(GameObject* pObject, float depth = 0.0f);

		void BaseUpdate(const float deltaTime);
		void BaseFixedUpdate(const float fixedDeltaTime);
		void BaseRender() const;

		Scene(const std::string& name, const bool removeOnDeactivate = false);
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		const std::string& GetName() const { return m_Name; }
		const bool GetShouldRemove() const { return m_RemoveOnDeactivate; }

	protected:

		virtual void Update(const float) {}
		virtual void FixedUpdate(const float) {}
		virtual void Render() const {}

	private: 
		friend class SceneManager;
		std::string m_Name;
		std::vector <std::pair<GameObject*, float>> m_Objects{};
		const bool m_RemoveOnDeactivate;

		static unsigned int m_IdCounter; 
	};

}
