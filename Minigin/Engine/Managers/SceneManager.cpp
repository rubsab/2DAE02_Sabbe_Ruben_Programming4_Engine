#include "MiniginPCH.h"
#include "SceneManager.h"
#include "../Scene/Scene.h"
#include "../Helpers/Logger.h"
#include <algorithm>

void MyEngine::SceneManager::Update(const float deltaTime)
{
	if (m_ActiveScene >= 0)
		m_Scenes[m_ActiveScene]->BaseUpdate(deltaTime);
}

void MyEngine::SceneManager::FixedUpdate(const float fixedDeltaTime)
{
	if (m_ActiveScene >= 0)
		m_Scenes[m_ActiveScene]->BaseFixedUpdate(fixedDeltaTime);
	DeleteToDeletes();
}

void MyEngine::SceneManager::Render() const
{
	if (m_ActiveScene >= 0)
		m_Scenes[m_ActiveScene]->BaseRender();
}

void MyEngine::SceneManager::DeleteToDeletes()
{
	for (std::string& toDelete : m_ToDelete)
	{
		MyEngine::Scene* scene = RemoveScene(toDelete);
		Safe_Delete(scene);
	}
	m_ToDelete.clear();
}

void MyEngine::SceneManager::AddScene(Scene* pScene)
{
	if (std::find(m_Scenes.begin(), m_Scenes.end(), pScene) == m_Scenes.end())
	{
		m_Scenes.push_back(pScene);
		if (m_ActiveScene == -1)
			m_ActiveScene = 0;
		return;
	}
	Logger::LogWarning("Scene does already exist in the SceneManager");
}

MyEngine::Scene* MyEngine::SceneManager::RemoveScene(const std::string& name)
{
	const std::vector<Scene*>::iterator it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [name](Scene* scene) {return scene->GetName() == name; });
	if (it != m_Scenes.end())
	{
		if (it - m_Scenes.begin() == 0)
		{
			Logger::LogWarning("Scene with name '" + name + "' is currently active");
			return nullptr;
		}
		m_Scenes.erase(it);
		return *it;
	}
	Logger::LogWarning("Scene with name '" + name + "' does not exist");
	return nullptr;
}

void MyEngine::SceneManager::SetSceneActive(const std::string& name)
{
	const std::vector<Scene*>::iterator it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [name](Scene* scene) {return scene->GetName() == name; });
	if (it != m_Scenes.end())
	{
		if (m_Scenes[m_ActiveScene]->GetShouldRemove())
			m_ToDelete.push_back(m_Scenes[m_ActiveScene]->GetName());
		m_ActiveScene = int(it - m_Scenes.begin());
		return;
	}
	Logger::LogWarning("Scene with name '" + name + "' does not exist");
}

MyEngine::SceneManager::~SceneManager()
{
	for (Scene* pScene : m_Scenes)
	{
		Safe_Delete(pScene);
	}
}
