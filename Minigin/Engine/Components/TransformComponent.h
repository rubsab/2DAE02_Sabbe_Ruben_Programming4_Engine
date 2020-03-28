#pragma once
#include "BaseComponent.h"
#include "../Helpers/Structs.h"

namespace MyEngine
{
	class TransformComponent final : public BaseComponent
	{
	public:
		//Inherited
		virtual void Update(const float deltaTime) override;
		virtual void Render() const override;

		TransformComponent(const Vector2 pos = { 0.0f, 0.0f });

		//Getters & Setters
		const Vector2& GetPosition() const;
		void SetPosition(float x, float y);
	private:
		Vector2 m_Position;
	};
}
