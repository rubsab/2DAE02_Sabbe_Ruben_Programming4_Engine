#pragma once
#include "../Helpers/Singleton.h"
#include "Box2D.h"
#include "SDL_pixels.h"

namespace MyEngine
{
	class Box2DDebugDrawer;
	class PhysicsManager final : public Singleton<PhysicsManager>
	{
	public:
		PhysicsManager() = default;
		~PhysicsManager();
		void Init(const b2Vec2& gravity, const int32 nrVelocityIterations, const int32 nrPositionIterations, const float ppm, const SDL_Color& debugColor);
		void FixedUpdate(const float fixedDeltaTime);
		b2Body* CreateBody(const b2BodyDef& bodyDef);
		const float GetPixelsPerMeter() const { return m_PixelsPerMeter; }
		void SetPixelsPerMeter(const float ppm) { m_PixelsPerMeter = ppm; }
		const bool IsDebugDrawingActive()const { return m_DebugDraw; }
		void EnableDebugDrawing(const bool debugDraw) { m_DebugDraw = debugDraw; }
		const SDL_Color& GetDebugColor() const { return m_DebugColor; }
		void SetDebugColor(const SDL_Color& debugColor) { m_DebugColor = debugColor; }
	private:
		b2World* m_pWorld;
		int32 m_NrVelocityIterations, m_NrPositionIterations;
		float m_PixelsPerMeter;
		bool m_DebugDraw;
		SDL_Color m_DebugColor;
	};
}

