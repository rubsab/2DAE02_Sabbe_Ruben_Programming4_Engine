#pragma once
#include "SceneObject.h"

namespace MyEngine
{
	class BaseComponent;
	class GameObject final : public SceneObject
	{
	public:
		void Update(const float deltaTime) override;
		void Render() const override;
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

		GameObject() = default;
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<BaseComponent*> m_Components;
	};
}
