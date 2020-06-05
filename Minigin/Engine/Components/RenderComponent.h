#pragma once
#include "BaseComponent.h"
#include <vector>

namespace MyEngine
{
	class Texture2D;
	struct AnimatedTexture
	{
		Texture2D* Texture;
		int Rows, Columns, CurrentFrame;
		float FrameTime, CurrentTime;
		bool IsAnimated;
	};
	class RenderComponent final : public BaseComponent
	{
	public:
		//Inherited
		virtual void Update(const float deltaTime) override;
		virtual void FixedUpdate(const float fixedDeltaTime) override;
		virtual void Render() const override;

		~RenderComponent();

		RenderComponent(const size_t maxAmountTextures);
		void AddTexture(const std::string& filePath, bool isAnimated = false, const int rows = 1, const int columns = 1, const float frameTime = 1.0f);
	private:
		std::vector<AnimatedTexture> m_Textures;
		size_t m_AmountTextures = 0;
	};
}

