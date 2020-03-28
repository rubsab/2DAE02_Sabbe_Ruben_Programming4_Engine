#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "../Scene/GameObject.h"
#include "TextComponent.h"

void MyEngine::FPSComponent::Update(const float deltaTime)
{
	m_DeltaTime += deltaTime;
	m_FPS++;
	if (m_DeltaTime >= 1.0f)
	{
		m_DeltaTime -= 1.0f;
		m_pGameObject->GetComponent<TextComponent>()->SetText(std::to_string(m_FPS));
		m_FPS = 0;
	}
}

void MyEngine::FPSComponent::Render() const
{
}
