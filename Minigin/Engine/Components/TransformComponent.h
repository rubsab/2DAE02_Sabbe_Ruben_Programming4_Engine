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

		TransformComponent(const glm::fvec2 pos = { 0.0f, 0.0f }, const float angle = 0.0f);

		//Getters & Setters
		const glm::fvec2& GetPosition() const {	return m_Position; }
		void SetPosition(float x, float y) { m_Position = { x, y }; }
		void SetPosition(const glm::fvec2& pos) { m_Position = pos; }

		const float& GetRotation() const { return m_Angle; }
		void SetAngle(float angle) { m_Angle = angle; }
	private:
		glm::fvec2 m_Position;
		float m_Angle;
	};
}
