#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "../Managers/InputManager.h"
#include <algorithm>

using namespace MyEngine;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name, const bool removeOnDeactivate) : m_Name(name), m_RemoveOnDeactivate(removeOnDeactivate) {}

Scene::~Scene()
{
	for (std::pair<GameObject*, float>& pair : m_Objects)
		Safe_Delete(pair.first);
}

void Scene::Add(GameObject* pObject, float depth)
{
	m_Objects.push_back({ pObject, depth });
	std::sort(m_Objects.begin(), m_Objects.end(), [](std::pair<GameObject*, float> a, std::pair<GameObject*, float> b) {return a.second > b.second; });
}

void MyEngine::Scene::BaseUpdate(const float deltaTime)
{
	for (const std::pair<GameObject*, float>& pair : m_Objects)
		pair.first->Update(deltaTime);
	Update(deltaTime);
}

void MyEngine::Scene::BaseFixedUpdate(const float fixedDeltaTime)
{
	for (size_t objectCounter{}; objectCounter < m_Objects.size(); objectCounter++)
	{
		if (m_Objects[objectCounter].first->ShouldDespawn())
		{
			Safe_Delete(m_Objects[objectCounter].first);
			m_Objects[objectCounter] = m_Objects.back();
			m_Objects.pop_back();
			objectCounter--;
			continue;
		}
		m_Objects[objectCounter].first->FixedUpdate(fixedDeltaTime);
	}
	FixedUpdate(fixedDeltaTime);
}

void MyEngine::Scene::BaseRender() const
{
	for (const std::pair<GameObject*, float>& pair : m_Objects)
		pair.first->Render();
	Render();
}

