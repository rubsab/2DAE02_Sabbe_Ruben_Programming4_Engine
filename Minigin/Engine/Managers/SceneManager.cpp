#include "MiniginPCH.h"
#include "SceneManager.h"
#include "../Scene/Scene.h"

void MyEngine::SceneManager::Update(const float deltaTime)
{
	for(Scene* pScene : m_Scenes)
	{
		pScene->Update(deltaTime);
	}
}

void MyEngine::SceneManager::Render()
{
	for (const Scene* pScene : m_Scenes)
	{
		pScene->Render();
	}
}

MyEngine::Scene& MyEngine::SceneManager::CreateScene(const std::string& name)
{
	Scene* scene = new Scene(name);
	m_Scenes.push_back(scene);
	return *scene;
}

MyEngine::SceneManager::~SceneManager()
{
	for (Scene* pScene : m_Scenes)
	{
		delete pScene;
		pScene = nullptr;
	}
}
