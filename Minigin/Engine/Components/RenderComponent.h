#pragma once
#include "BaseComponent.h"
#include <glm/common.hpp>
#include <vector>

struct SDL_Rect;

namespace MyEngine
{
	class Texture2D;
	struct AnimatedTexture
	{
		Texture2D* Texture;
		float FrameTime, CurrentTime;
		int Rows, Columns, CurrentFrame, State, DrawWidth, DrawHeight;
		bool IsAnimated;
		glm::fvec2 Pivot;
	};
	class RenderComponent final : public BaseComponent
	{
	public:
		//Inherited
		void Update(const float deltaTime) override;
		void FixedUpdate(const float fixedDeltaTime) override;
		void Render() const override;

		RenderComponent(const size_t maxAmountTextures = 1);
		void AddTexture(const std::string& filePath, bool isAnimated = false, const int rows = 0, const int columns = 0, const float frameTime = 0.0f, const int drawWidth = 0.0f, const int drawHeight = 0.0f, const glm::fvec2& pivot = { 0.5f, 0.5f }, const int activeState = -1);
	private:
		void DimensionsSet(size_t index, SDL_Rect& dstRect) const;
		std::vector<AnimatedTexture> m_Textures;
		size_t m_AmountTextures = 0;
	};
}

