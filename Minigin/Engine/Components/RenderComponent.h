#pragma once
#include "BaseComponent.h"
#include <vector>

namespace MyEngine
{
	class Texture2D;
	class RenderComponent final : public BaseComponent
	{
	public:
		//Inherited
		virtual void Update(const float deltaTime) override;
		virtual void Render() const override;

		~RenderComponent();

		RenderComponent(const size_t maxAmountTextures);
		void AddTexture(Texture2D* texture);
	private:
		std::vector<Texture2D*> m_Textures;
		size_t m_AmountTextures = 0;
	};
}

