#pragma once
#include "BaseComponent.h"
namespace MyEngine
{
	class FPSComponent final : public BaseComponent
	{
	public:
		//Inherited
		void Update(const float deltaTime) override;
		void FixedUpdate(const float fixedDeltaTime) override;
		void Render() const override;

	private:
		float m_DeltaTime;
		int m_FPS;
	};
}
