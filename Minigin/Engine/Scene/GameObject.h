#pragma once
#include <glm/common.hpp>

namespace MyEngine
{
	class BaseComponent;
	class GameObject final
	{
	public:
		void Update(const float deltaTime);
		void FixedUpdate(const float fixedDeltaTime);
		void Render() const;
		void AddComponent(BaseComponent* pComponent);
		template <class T>
		T* GetComponent()
		{
			const type_info& ti = typeid(T);
			for (BaseComponent* pComp : m_Components)
			{
				if (pComp && typeid(*pComp) == ti)
					return static_cast<T*>(pComp);
			}
			return nullptr;
		}
		void SetState(int state) { m_State = state; }
		int GetState() const { return m_State; }

		void SetActive(bool isActive) { m_IsActive = isActive; }
		bool IsActive() const { return m_IsActive; }

		void SetShouldDespawn(bool shouldDespawn) { m_ShouldDespawn = shouldDespawn; }
		bool ShouldDespawn() const { return m_ShouldDespawn; }

		void SetShouldRemoveOnSceneKill(bool shouldRemoveOnSceneKill) { m_ShouldRemoveOnSceneKill = shouldRemoveOnSceneKill; }
		bool ShouldRemoveOnSceneKill() const { return m_ShouldRemoveOnSceneKill; }

		GameObject(const glm::fvec2& pos = { 0.0f, 0.0f }, const float angle = 0.0f, bool shouldRemoveOnSceneKill = true);
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<BaseComponent*> m_Components;
		int m_State;
		bool m_IsActive, m_ShouldDespawn, m_ShouldRemoveOnSceneKill;
	};
}
