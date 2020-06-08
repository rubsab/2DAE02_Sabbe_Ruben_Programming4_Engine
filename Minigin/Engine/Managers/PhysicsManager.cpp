#include "MiniginPCH.h"
#include "PhysicsManager.h"
#include "SDL.h"

MyEngine::PhysicsManager::~PhysicsManager()
{
	delete m_pWorld;
}

void MyEngine::PhysicsManager::Init(const b2Vec2& gravity, const int32 nrVelocityIterations, const int32 nrPositionIterations, const float ppm, const SDL_Color& debugColor)
{
	m_pWorld = new b2World(gravity);
	m_NrVelocityIterations = nrVelocityIterations;
	m_NrPositionIterations = nrPositionIterations;
	m_PixelsPerMeter = ppm;
	m_DebugDraw = false;
	m_DebugColor = debugColor;
}

void MyEngine::PhysicsManager::FixedUpdate(const float fixedDeltaTime)
{
	m_pWorld->Step(fixedDeltaTime, m_NrVelocityIterations, m_NrPositionIterations);
}

b2Body* MyEngine::PhysicsManager::CreateBody(const b2BodyDef& bodyDef)
{
	return m_pWorld->CreateBody(&bodyDef);
}
