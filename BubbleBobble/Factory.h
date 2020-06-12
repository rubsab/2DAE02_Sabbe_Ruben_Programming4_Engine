#pragma once
#include <string>
#include "DataHolder.h"
#include "Level.h"
#include "Engine\Managers\ResourceManager.h"
#include "Engine\Managers\SceneManager.h"
#include "Engine\Scene\Scene.h"
#include "Engine\Scene\GameObject.h"
#include "Engine\Components\RenderComponent.h"
#include "MyComponents.h"
#include "Engine\Managers\InputManager.h"
using namespace MyEngine;

inline MyEngine::GameObject* CreateEnemy(const Enemy& enemy, const int windowHeight, MyEngine::GameObject* pPlayer)
{
	const int cellDimension = windowHeight / 25;
	MyEngine::GameObject* enemyObj = new MyEngine::GameObject();
	enemyObj->AddComponent(new RenderComponent());
	enemyObj->GetComponent<RenderComponent>()->AddTexture("Enemy.png", true, false, 1, 4, 0.25f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 0, { 0.0f, int(enemy.Type) / 8.0f }, { 1.0f, 0.125 });
	enemyObj->GetComponent<RenderComponent>()->AddTexture("Enemy.png", true, false, 1, 4, 0.25f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 1, { 0.0f, int(enemy.Type) / 8.0f }, { 1.0f, 0.125 }, { 0.0f, 0.0f }, true);
	enemyObj->SetState(0);
	enemyObj->AddComponent(new PhysicsComponent(PhysicsComponent::PhysicsType::Kinematic, { cellDimension / 2.0f + enemy.Col * cellDimension, windowHeight - (cellDimension / 2.0f + (enemy.Row + 2) * cellDimension) }, 0.0f, float(cellDimension), float(cellDimension), 1.0f, 0.3f, 0.0f));
	enemyObj->AddComponent(new EnemyBehaviourComponent(pPlayer, int(enemy.Type), enemy.Delay));
	DataHolder::GetInstance()->AddEnemy(enemyObj);

	return enemyObj;
}

inline void CreateBubble(MyEngine::GameObject* player, Scene* scene, int playerNr, int windowHeight, int levelNr)
{
	const int cellDimension = windowHeight / 25;
	MyEngine::GameObject* bubbleObj = new MyEngine::GameObject();
	scene->Add(bubbleObj, -2.5f);
	bubbleObj->AddComponent(new RenderComponent());
	bubbleObj->GetComponent<RenderComponent>()->AddTexture("bubbles.png", true, false, 1, 4, 0.25f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 0, { 0.0f, 0.1f * playerNr }, { 1.0f, 0.1f });
	bubbleObj->SetState(0);
	bubbleObj->AddComponent(new PhysicsComponent(PhysicsComponent::PhysicsType::Kinematic, player->GetComponent<TransformComponent>()->GetPosition(), 0.0f, float(cellDimension), float(cellDimension), 1.0f, 0.3f, 0.0f));
	bubbleObj->AddComponent(new BubbleBehaviourComponent(DataHolder::GetInstance()->GetLevel(levelNr), bool(player->GetState() % 2), player, scene));
	player->GetComponent<PlayerMovementComponent>()->Shoot();
}

inline MyEngine::GameObject* CreatePlayer(const int playerNr, int levelNr, const int windowHeight, Scene* scene)
{
	const int cellDimension = windowHeight / 25;
	MyEngine::GameObject* playerObj = new MyEngine::GameObject();
	playerObj->AddComponent(new RenderComponent());
	std::string file = (playerNr == 1 ? "heroGreen.png" : "heroBlue.png");
	playerObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture(file), true, false, 1, 4, 0.25f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 0, { 0.0f, 0.0f }, { 1.0f, 0.3333f });
	playerObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture(file), true, false, 1, 4, 0.25f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 1, { 0.0f, 0.0f }, { 1.0f, 0.3333f }, { 0.0f, 0.0f }, true);
	playerObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture(file), false, false, 0, 0, 0.0f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 2, { 0.0f, 0.0f }, { 0.25f, 0.3333f });
	playerObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture(file), false, false, 0, 0, 0.0f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 3, { 0.0f, 0.0f }, { 0.25f, 0.3333f }, { 0.0f, 0.0f }, true);
	playerObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture(file), true, false, 1, 2, 0.1f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 4, { 0.0f, 2.0f / 3.0f }, { 0.5f, 1.0f / 3.0f });
	playerObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture(file), true, false, 1, 2, 0.1f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 5, { 0.0f, 2.0f / 3.0f }, { 0.5f, 1.0f / 3.0f }, { 0.0f, 0.0f }, true);
	playerObj->AddComponent(new PhysicsComponent(PhysicsComponent::PhysicsType::Kinematic, { cellDimension * 3.0f, cellDimension * 20.0f }, 0.0f, float(cellDimension), float(cellDimension), 1.0f, 0.3f, 0.0f));
	playerObj->AddComponent(new PlayerMovementComponent(DataHolder::GetInstance()->GetLevel(levelNr)));
	playerObj->SetState(2);

	switch (playerNr)
	{
	case(1):
		InputManager::GetInstance()->AddCommand({ {'D', Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_RIGHT, Hardware::Controller} }, new Command{ [playerObj]() {playerObj->SetState(0); }, ButtonState::Down }, playerNr - 1);
		InputManager::GetInstance()->AddCommand({ {'A', Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_LEFT, Hardware::Controller} }, new Command{ [playerObj]() {playerObj->SetState(1); }, ButtonState::Down }, playerNr - 1);
		InputManager::GetInstance()->AddCommand({ {'D', Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_RIGHT, Hardware::Controller} }, new Command{ [playerObj]() {playerObj->SetState(2); }, ButtonState::Released }, playerNr - 1);
		InputManager::GetInstance()->AddCommand({ {'A', Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_LEFT, Hardware::Controller} }, new Command{ [playerObj]() {playerObj->SetState(3); }, ButtonState::Released }, playerNr - 1);
		InputManager::GetInstance()->AddCommand({ {'W', Hardware::KeyBoard}, {XINPUT_GAMEPAD_A, Hardware::Controller} }, new Command{ [playerObj]() {playerObj->GetComponent<PlayerMovementComponent>()->Jump(); }, ButtonState::Pressed }, playerNr - 1);
		InputManager::GetInstance()->AddCommand({ {VK_SPACE, Hardware::KeyBoard}, {XINPUT_GAMEPAD_X, Hardware::Controller} }, new Command{ [scene, playerObj, windowHeight, levelNr]() {CreateBubble(playerObj, scene, 0, windowHeight, levelNr); }, ButtonState::Pressed }, playerNr - 1);
		break;
	case(2):
		InputManager::GetInstance()->AddCommand({ {VK_LEFT, Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_RIGHT, Hardware::Controller} }, new Command{ [playerObj]() {playerObj->SetState(0); }, ButtonState::Down }, playerNr - 1);
		InputManager::GetInstance()->AddCommand({ {VK_RIGHT, Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_LEFT, Hardware::Controller} }, new Command{ [playerObj]() {playerObj->SetState(1); }, ButtonState::Down }, playerNr - 1);
		InputManager::GetInstance()->AddCommand({ {VK_LEFT, Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_RIGHT, Hardware::Controller} }, new Command{ [playerObj]() {playerObj->SetState(2); }, ButtonState::Released }, playerNr - 1);
		InputManager::GetInstance()->AddCommand({ {VK_RIGHT, Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_LEFT, Hardware::Controller} }, new Command{ [playerObj]() {playerObj->SetState(3); }, ButtonState::Released }, playerNr - 1);
		InputManager::GetInstance()->AddCommand({ {VK_UP, Hardware::KeyBoard}, {XINPUT_GAMEPAD_A, Hardware::Controller} }, new Command{ [playerObj]() {playerObj->GetComponent<PlayerMovementComponent>()->Jump(); }, ButtonState::Pressed }, playerNr - 1);
		InputManager::GetInstance()->AddCommand({ {VK_NUMPAD0, Hardware::KeyBoard}, {XINPUT_GAMEPAD_X, Hardware::Controller} }, new Command{ [scene, playerObj, windowHeight, levelNr]() {CreateBubble(playerObj, scene, 1, windowHeight, levelNr); }, ButtonState::Pressed }, playerNr - 1);
		break;
	default:
		break;
	}
	scene->Add(playerObj, -2.0f);

	return playerObj;
}

inline MyEngine::GameObject* CreateFruitDrop(GameObject* pPlayer, const Level& currentLevel, int enemyType, const glm::fvec2& pos)
{
	int cellDimension = currentLevel.WindowHeight / 25;
	MyEngine::GameObject* fruitDrop = new MyEngine::GameObject();
	fruitDrop->AddComponent(new RenderComponent());
	fruitDrop->GetComponent<RenderComponent>()->AddTexture("drops.png", false, false, 0, 0, 0.0f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, -1, { 0.0f, 0.125f * enemyType }, { 1.0f, 0.125f });
	fruitDrop->AddComponent(new PhysicsComponent(PhysicsComponent::PhysicsType::Kinematic, pos, 0.0f, float(cellDimension), float(cellDimension), 1.0f, 0.3f, 0.0f));
	fruitDrop->AddComponent(new FruitDropComponent(pPlayer, currentLevel));
	return fruitDrop;
}

inline void CreateLevel(const int levelNr, const int windowHeight)
{
	const Level& level = DataHolder::GetInstance()->GetLevel(levelNr);
	Scene* levelScene = new Scene("Level" + std::to_string(levelNr), true);
	Texture2D* blocksTex = ResourceManager::GetInstance()->LoadTexture("blocks.png");
	MyEngine::GameObject* levelObj = new MyEngine::GameObject();
	levelObj->AddComponent(new RenderComponent());
	levelScene->Add(levelObj);
	SceneManager::GetInstance()->AddScene(levelScene);
	std::vector<std::vector<bool>> hasCollider(25);
	MyEngine::GameObject* pPlayer = CreatePlayer(1, levelNr, windowHeight, levelScene);

	int cellDimension = windowHeight / 25;
	for (size_t cellIndex{}; cellIndex < 32; cellIndex++)
	{
		size_t highestLine = 0;
		bool hasWall = false;
		for (size_t lineIndex{}; lineIndex < 25; lineIndex++)
		{
			hasCollider[lineIndex].push_back(false);
			if (level.Lines[lineIndex][cellIndex])
				levelObj->GetComponent<RenderComponent>()->AddTexture(blocksTex, false, false, 0, 0, 0.0f, cellDimension, cellDimension, { 0.0f, 0.0f }, -1, { (levelNr % 10) * 0.1f, (levelNr / 10) * 0.1f }, { 0.1f, 0.1f }, { cellIndex, 25 - lineIndex });

			if (level.Lines[lineIndex][cellIndex] && !hasWall)
			{
				highestLine = lineIndex;
				hasWall = true;
			}
			if (hasWall && (!level.Lines[lineIndex][cellIndex] || lineIndex == 24))
			{
				if (lineIndex - highestLine > 1)
				{
					MyEngine::GameObject* collider = new MyEngine::GameObject();
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
		bool hasWall = false;
		for (size_t cellIndex{}; cellIndex < 32; cellIndex++)
		{
			if (level.Lines[lineIndex][cellIndex] && !hasWall && !hasCollider[lineIndex][cellIndex])
			{
				highestLine = cellIndex;
				hasWall = true;
			}
			if (hasWall && (!level.Lines[lineIndex][cellIndex] || cellIndex == 31 || hasCollider[lineIndex][cellIndex]))
			{
				MyEngine::GameObject* collider = new MyEngine::GameObject();
				collider->AddComponent(new PhysicsComponent(PhysicsComponent::PhysicsType::Static, { ((highestLine + (cellIndex - highestLine) / 2.0f) * cellDimension), windowHeight - (cellDimension / 2.0f + lineIndex * cellDimension) }, 0.0f, cellDimension * (cellIndex - highestLine) / 2.0f, cellDimension / 2.0f));
				levelScene->Add(collider, -1.0f);
				hasWall = false;
			}
		}
	}

	for (const Enemy& enemy : level.Enemies)
	{
		levelScene->Add(CreateEnemy(enemy, windowHeight, pPlayer));
	}
}