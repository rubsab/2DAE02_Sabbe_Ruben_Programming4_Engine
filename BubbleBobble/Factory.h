#pragma once
#include <string>
#include "DataHolder.h"
#include "Level.h"
#include "Engine\Managers\ResourceManager.h"
#include "Engine\Managers\SceneManager.h"
#include "Engine\Scene\Scene.h"
#include "Engine\Scene\GameObject.h"
#include "Engine\Components\RenderComponent.h"
using namespace MyEngine;

inline GameObject* CreateEnemy(const Enemy& enemy, const int windowHeight)
{
	const int cellDimension = windowHeight / 25;
	GameObject* enemyObj = new GameObject({ cellDimension / 2.0f + enemy.Col * cellDimension, windowHeight - (cellDimension / 2.0f + (enemy.Row + 2) * cellDimension) });
	enemyObj->AddComponent(new RenderComponent());

	switch (enemy.Type)
	{
	default:
	case(EnemyType::ZenChan):
		enemyObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture("sprites0.png"), true, 1, 8, 0.125f, cellDimension * 4, cellDimension * 2, { 0.5f, 0.5f }, -1, { 0.0f, 0.25f }, { 1.0f, 0.0625 });
		break;
	case(EnemyType::Mighta):
		enemyObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture("sprites0.png"), true, 1, 8, 0.125f, cellDimension * 4, cellDimension * 2, { 0.5f, 0.5f }, -1, { 0.0f, 0.9375f }, { 1.0f, 0.0625 });
		break;
	}

	return enemyObj;
}

inline void CreateLevel(const int levelNr, const int windowHeight)
{
	const Level& level = DataHolder::GetInstance()->GetLevel(levelNr);
	Scene* levelScene = new Scene("Level" + std::to_string(levelNr), true);
	Texture2D* blocksTex = ResourceManager::GetInstance()->LoadTexture("blocks.png");
	GameObject* levelObj = new GameObject();
	levelObj->AddComponent(new RenderComponent());
	levelScene->Add(levelObj);
	SceneManager::GetInstance()->AddScene(levelScene);
	std::vector<std::vector<bool>> hasCollider(25);

	int testCounter = 0;
	int cellDimension = windowHeight / 25;
	for (size_t cellIndex{}; cellIndex < 32; cellIndex++)
	{
		size_t highestLine = 0;
		bool hasWall = level.first[0][cellIndex];
		for (size_t lineIndex{}; lineIndex < 25; lineIndex++)
		{
			hasCollider[lineIndex].push_back(false);
			if (level.first[lineIndex][cellIndex])
				levelObj->GetComponent<RenderComponent>()->AddTexture(blocksTex, false, 0, 0, 0.0f, cellDimension, cellDimension, { 0.0f, 0.0f }, -1, { (levelNr % 10) * 0.1f, (levelNr / 10) * 0.1f }, { 0.1f, 0.1f }, { cellIndex, 25 - lineIndex });

			if (level.first[lineIndex][cellIndex] && !hasWall)
			{
				highestLine = lineIndex;
				hasWall = true;
			}
			if (hasWall && (!level.first[lineIndex][cellIndex] || lineIndex == 24))
			{
				if (lineIndex - highestLine > 1)
				{
					testCounter++;
					GameObject* collider = new GameObject();
					collider->AddComponent(new PhysicsComponent(PhysicsComponent::PhysicsType::Static, { cellDimension / 2.0f + cellIndex * cellDimension, windowHeight - ((highestLine + (lineIndex - highestLine) / 2.0f) * cellDimension) }, 0.0f, cellDimension / 2.0f, cellDimension * (lineIndex - highestLine) / 2.0f));
					levelScene->Add(collider, -1.0f);
					for (size_t counter{}; counter < lineIndex - highestLine; counter++)
					{
						hasCollider[highestLine + counter][cellIndex] = true;
					}
				}
				hasWall = false;
			}
		}
	}

	for (size_t lineIndex{}; lineIndex < 25; lineIndex++)
	{
		size_t highestLine = 0;
		bool hasWall = level.first[lineIndex][0] && !hasCollider[lineIndex][0];
		for (size_t cellIndex{}; cellIndex < 32; cellIndex++)
		{
			if (level.first[lineIndex][cellIndex] && !hasWall && !hasCollider[lineIndex][cellIndex])
			{
				highestLine = cellIndex;
				hasWall = true;
			}
			if (hasWall && (!level.first[lineIndex][cellIndex] || cellIndex == 31 || hasCollider[lineIndex][cellIndex]))
			{
				testCounter++;
				GameObject* collider = new GameObject();
				collider->AddComponent(new PhysicsComponent(PhysicsComponent::PhysicsType::Static, { ((highestLine + (cellIndex - highestLine) / 2.0f) * cellDimension), windowHeight - (cellDimension / 2.0f + lineIndex * cellDimension) }, 0.0f, cellDimension * (cellIndex - highestLine) / 2.0f, cellDimension / 2.0f));
				levelScene->Add(collider, -1.0f);
				hasWall = false;
			}
		}
	}

	for (const Enemy& enemy : level.second)
	{
		levelScene->Add(CreateEnemy(enemy, windowHeight));
	}
}