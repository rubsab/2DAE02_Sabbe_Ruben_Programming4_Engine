#pragma once
#include "BaseComponent.h"
namespace MyEngine
{
	class FPSComponent final : public BaseComponent
	{
	public:
		//Inherited
		virtual void Update(const float deltaTime) override;
		virtual void Render() const override;

	private:
		float m_DeltaTime;
		int m_FPS;
	};
}
