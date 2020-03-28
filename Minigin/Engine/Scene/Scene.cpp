#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "../Managers/InputManager.h"

using namespace MyEngine;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene()
{
	for (GameObject* pGameObj : m_Objects)
	{
		delete pGameObj;
		pGameObj = nullptr;
	}
}

void Scene::Add(GameObject* pObject)
{
	m_Objects.push_back(pObject);
}

void Scene::Update(const float deltaTime)
{
	for(GameObject* pGameObj : m_Objects)
	{
		InputManager::GetInstance()->ProcessInput(pGameObj);
		pGameObj->Update(deltaTime);
	}
}

void Scene::Render() const
{
	for (const GameObject* pGameObj : m_Objects)
	{
		pGameObj->Render();
	}
}

