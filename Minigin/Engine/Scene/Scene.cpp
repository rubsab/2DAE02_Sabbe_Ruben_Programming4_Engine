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
	{
		if (pair.first->ShouldRemoveOnSceneKill())
			Safe_Delete(pair.first);
	}
}

void MyEngine::Scene::Invoke(std::function<void()>func, float delay)
{
	m_Invokers.push_back({ func, delay });
}

void Scene::Add(GameObject* pObject, float depth)
{
	m_Objects.push_back({ pObject, depth });
	std::sort(m_Objects.begin(), m_Objects.end(), [](std::pair<GameObject*, float> a, std::pair<GameObject*, float> b) {return a.second > b.second; });
}

void MyEngine::Scene::Update(const float deltaTime)
{
	for (const std::pair<GameObject*, float>& pair : m_Objects)
		pair.first->Update(deltaTime);
}

void MyEngine::Scene::FixedUpdate(const float fixedDeltaTime)
{
	for (size_t invokeCounter{}; invokeCounter < m_Invokers.size(); invokeCounter++)
	{
		m_Invokers[invokeCounter].second -= fixedDeltaTime;
		if (m_Invokers[invokeCounter].second < 0.0f)
		{
			m_Invokers[invokeCounter].first();
			m_Invokers[invokeCounter] = m_Invokers.back();
			m_Invokers.pop_back();
			invokeCounter--;
		}
	}

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
}

void MyEngine::Scene::Render() const
{
	for (const std::pair<GameObject*, float>& pair : m_Objects)
		pair.first->Render();
}

