#include "MiniginPCH.h"
#include "TransformComponent.h"
#include "../Graphics/Renderer.h"

void MyEngine::TransformComponent::Update(const float deltaTime)
{
	UNREFERENCED_PARAMETER(deltaTime);
}

void MyEngine::TransformComponent::FixedUpdate(const float fixedDeltaTime)
{
	UNREFERENCED_PARAMETER(fixedDeltaTime);
}

void MyEngine::TransformComponent::Render() const
{

}

MyEngine::TransformComponent::TransformComponent(const glm::fvec2 pos, const float angle)
	:m_Position{ pos },
	m_Angle{ angle }
{
}