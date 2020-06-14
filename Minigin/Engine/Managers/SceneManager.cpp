#include "MiniginPCH.h"
#include "SceneManager.h"
#include "../Scene/Scene.h"
#include "../Helpers/Logger.h"
#include <algorithm>
#include "InputManager.h"

void MyEngine::SceneManager::Update(const float deltaTime)
{
	if (m_ActiveScene >= 0)
		m_Scenes[m_ActiveScene]->Update(deltaTime);
}

void MyEngine::SceneManager::FixedUpdate(const float fixedDeltaTime)
{
	if (m_ActiveScene >= 0)
		m_Scenes[m_ActiveScene]->FixedUpdate(fixedDeltaTime);
	DeleteToDeletes();
	if (!m_DelayedName.empty())
	{
		m_DelayTimer -= fixedDeltaTime;
		if (m_DelayTimer <= 0.0f)
		{
			SetSceneActive(m_DelayedName);
			m_DelayedName = "";
		}
	}
}

void MyEngine::SceneManager::Render() const
{
	if (m_ActiveScene >= 0)
		m_Scenes[m_ActiveScene]->Render();
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
		if (it - m_Scenes.begin() == m_ActiveScene)
		{
			Logger::LogWarning("Scene with name '" + name + "' is currently active");
			return nullptr;
		}
		else if (it - m_Scenes.begin() < m_ActiveScene)
			m_ActiveScene--;
		MyEngine::InputManager::GetInstance()->RemoveCommandsByIdentifierName((*it)->GetName());
		Scene* scene = *it;
		m_Scenes.erase(it);
		return scene;
	}
	//Logger::LogWarning("Scene with name '" + name + "' does not exist");
	return nullptr;
}

void MyEngine::SceneManager::SetSceneActive(const std::string& name)
{
	const std::vector<Scene*>::iterator it = std::find_if(m_Scenes.begin(), m_Scenes.end(), [name](Scene* scene) {return scene->GetName() == name; });
	if (it != m_Scenes.end())
	{
		if (m_Scenes[m_ActiveScene]->GetShouldRemove() && std::find(m_ToDelete.begin(), m_ToDelete.end(), m_Scenes[m_ActiveScene]->GetName()) == m_ToDelete.end())
			m_ToDelete.push_back(m_Scenes[m_ActiveScene]->GetName());
		m_ActiveScene = int(it - m_Scenes.begin());
		return;
	}
	Logger::LogWarning("Scene with name '" + name + "' does not exist");
}

void MyEngine::SceneManager::SetSceneActiveDelayed(const std::string& name, float delay)
{
	m_DelayedName = name;
	m_DelayTimer = delay;
}

MyEngine::SceneManager::~SceneManager()
{
	for (Scene* pScene : m_Scenes)
	{
		Safe_Delete(pScene);
	}
}
