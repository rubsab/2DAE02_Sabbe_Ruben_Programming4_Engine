#pragma once
#include "BaseComponent.h"

class b2Body;

namespace MyEngine
{
	class PhysicsComponent final : public BaseComponent
	{
	public:
		enum class PhysicsType
		{
			Static,
			Kinematic,
			Dynamic
		};

		PhysicsComponent(const PhysicsType physicsType, const float posX = 0.0f, const float posY = 0.0f, const float angle = 0.0f, const float halfWidth = 0.5f, const float halfHeight = 0.5f, const float density = 1.0f, const float friction = 0.3f, const float restitution = 0.5f, unsigned short categoryBits = 0x0001, unsigned short maskBits = 0x0000);
		void Update(const float deltaTime) override;
		void FixedUpdate(const float fixedDeltaTime) override;
		void Render() const override;

		bool IsOverlapping(const PhysicsComponent* other) const;
	private:
		friend class TransformComponent;
		b2Body* m_pBody;
	};
}