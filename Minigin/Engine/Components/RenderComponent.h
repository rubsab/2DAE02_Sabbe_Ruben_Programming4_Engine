#pragma once
#include "BaseComponent.h"
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
		void AddTexture(const std::string& filePath, const int rows, const int columns, const float frameTime, const int activeState = -1);
		void AddTexture(const std::string& filePath, const int rows, const int columns, const float frameTime, const int drawWidth, const int drawHeight, const int activeState = -1);
		void AddTexture(const std::string& filePath, const int activeState = -1);
		void AddTexture(const std::string& filePath, const int drawWidth, const int drawHeight, const int activeState = -1);
	private:
		void AddTexture(const std::string& filePath, const int rows, const int columns, const float frameTime, const int drawWidth, const int drawHeight, const int activeState, bool isAnimated);
		void DimensionsSet(size_t index, SDL_Rect& dstRect) const;
		std::vector<AnimatedTexture> m_Textures;
		size_t m_AmountTextures = 0;
	};
}

