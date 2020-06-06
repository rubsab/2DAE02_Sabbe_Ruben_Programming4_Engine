#pragma once
#include "BaseComponent.h"
#include <glm/common.hpp>

namespace MyEngine
{
	class TransformComponent final : public BaseComponent
	{
	public:
		//Inherited
		virtual void Update(const float deltaTime) override;
		virtual void FixedUpdate(const float fixedDeltaTime) override;
		virtual void Render() const override;

		TransformComponent(const glm::fvec2 pos = { 0.0f, 0.0f });

		//Getters & Setters
		const glm::fvec2& GetPosition() const;
		void SetPosition(float x, float y);
		void SetPosition(const glm::fvec2& pos);
	private:
		glm::fvec2 m_Position;
	};
}
