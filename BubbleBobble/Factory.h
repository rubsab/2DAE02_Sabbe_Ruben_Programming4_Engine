#pragma once
#include <string>
#include "DataHolder.h"
#include "Level.h"
#include "Engine\Managers\ResourceManager.h"
#include "Engine\Managers\SceneManager.h"
#include "Engine\Scene\Scene.h"
#include "Engine\Scene\GameObject.h"
#include "Engine\Components\RenderComponent.h"
#include "Engine\Components\PhysicsComponent.h"
#include "Engine\Components\TransformComponent.h"
#include "Engine\Components\TextComponent.h"
#include "MyComponents.h"
#include "Engine\Managers\InputManager.h"
#include "LevelManager.h"
using namespace MyEngine;

#define currentLevel DataHolder::GetInstance()->GetLevel(LevelManager::GetInstance()->GetCurrentLevel())

inline MyEngine::GameObject* CreateEnemy(const Enemy& enemy, const int windowHeight)
{
	const int cellDimension = windowHeight / 25;
	MyEngine::GameObject* enemyObj = new MyEngine::GameObject();
	enemyObj->AddComponent(new RenderComponent());
	enemyObj->GetComponent<RenderComponent>()->AddTexture("Enemy.png", true, false, 1, 4, 0.25f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 0, { 0.0f, int(enemy.Type) / 8.0f }, { 1.0f, 0.125 });
	enemyObj->GetComponent<RenderComponent>()->AddTexture("Enemy.png", true, false, 1, 4, 0.25f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 1, { 0.0f, int(enemy.Type) / 8.0f }, { 1.0f, 0.125 }, { 0.0f, 0.0f }, true);
	enemyObj->SetState(0);
	enemyObj->AddComponent(new PhysicsComponent(PhysicsComponent::PhysicsType::Kinematic, { cellDimension / 2.0f + enemy.Col * cellDimension, windowHeight - (cellDimension / 2.0f + (enemy.Row + 2) * cellDimension) }, 0.0f, float(cellDimension), float(cellDimension), 1.0f, 0.3f, 0.0f));
	enemyObj->AddComponent(new EnemyBehaviourComponent(int(enemy.Type), enemy.Delay));
	DataHolder::GetInstance()->AddEnemy(enemyObj);

	return enemyObj;
}

inline void CreateBubble(MyEngine::GameObject* player, int playerNr, int windowHeight)
{
	const int cellDimension = windowHeight / 25;
	MyEngine::GameObject* bubbleObj = new MyEngine::GameObject();
	SceneManager::GetInstance()->GetActiveScene()->Add(bubbleObj, -2.5f);
	bubbleObj->AddComponent(new RenderComponent());
	bubbleObj->GetComponent<RenderComponent>()->AddTexture("bubbles.png", true, false, 1, 4, 0.25f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 0, { 0.0f, 0.1f * playerNr }, { 1.0f, 0.1f });
	bubbleObj->SetState(0);
	bubbleObj->AddComponent(new PhysicsComponent(PhysicsComponent::PhysicsType::Kinematic, player->GetComponent<TransformComponent>()->GetPosition(), 0.0f, float(cellDimension), float(cellDimension), 1.0f, 0.3f, 0.0f));
	bubbleObj->AddComponent(new BubbleBehaviourComponent(bool(player->GetState() % 2)));
	player->GetComponent<PlayerMovementComponent>()->Shoot();
}

inline MyEngine::GameObject* CreatePlayer(const int playerNr, const int windowHeight)
{
	const int cellDimension = windowHeight / 25;
	MyEngine::GameObject* playerObj = new MyEngine::GameObject({ 0.0f, 0.0f }, 0.0f, false);
	playerObj->AddComponent(new RenderComponent());
	std::string file = (playerNr == 1 ? "heroGreen.png" : "heroBlue.png");
	playerObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture(file), true, false, 1, 4, 0.25f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 0, { 0.0f, 0.0f }, { 1.0f, 0.3333f });
	playerObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture(file), true, false, 1, 4, 0.25f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 1, { 0.0f, 0.0f }, { 1.0f, 0.3333f }, { 0.0f, 0.0f }, true);
	playerObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture(file), false, false, 0, 0, 0.0f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 2, { 0.0f, 0.0f }, { 0.25f, 0.3333f });
	playerObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture(file), false, false, 0, 0, 0.0f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 3, { 0.0f, 0.0f }, { 0.25f, 0.3333f }, { 0.0f, 0.0f }, true);
	playerObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture(file), true, false, 1, 2, 0.1f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 4, { 0.0f, 2.0f / 3.0f }, { 0.5f, 1.0f / 3.0f });
	playerObj->GetComponent<RenderComponent>()->AddTexture(ResourceManager::GetInstance()->LoadTexture(file), true, false, 1, 2, 0.1f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, 5, { 0.0f, 2.0f / 3.0f }, { 0.5f, 1.0f / 3.0f }, { 0.0f, 0.0f }, true);
	playerObj->AddComponent(new PhysicsComponent(PhysicsComponent::PhysicsType::Kinematic, { cellDimension * 3.0f, cellDimension * 20.0f }, 0.0f, float(cellDimension), float(cellDimension), 1.0f, 0.3f, 0.0f));
	playerObj->AddComponent(new PlayerMovementComponent());
	playerObj->SetState(2);
	DataHolder::GetInstance()->AddPlayer(playerObj);

	switch (playerNr)
	{
	case(1):
		InputManager::GetInstance()->AddCommand({ {'D', Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_RIGHT, Hardware::Controller, 0} }, new Command{ [playerObj]() {playerObj->SetState(0); }, ButtonState::Down });
		InputManager::GetInstance()->AddCommand({ {'A', Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_LEFT, Hardware::Controller, 0} }, new Command{ [playerObj]() {playerObj->SetState(1); }, ButtonState::Down });
		InputManager::GetInstance()->AddCommand({ {'D', Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_RIGHT, Hardware::Controller, 0} }, new Command{ [playerObj]() {playerObj->SetState(2); }, ButtonState::Released });
		InputManager::GetInstance()->AddCommand({ {'A', Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_LEFT, Hardware::Controller, 0} }, new Command{ [playerObj]() {playerObj->SetState(3); }, ButtonState::Released });
		InputManager::GetInstance()->AddCommand({ {'W', Hardware::KeyBoard}, {XINPUT_GAMEPAD_A, Hardware::Controller, 0} }, new Command{ [playerObj]() {playerObj->GetComponent<PlayerMovementComponent>()->Jump(); }, ButtonState::Pressed });
		InputManager::GetInstance()->AddCommand({ {VK_SPACE, Hardware::KeyBoard}, {XINPUT_GAMEPAD_X, Hardware::Controller, 0} }, new Command{ [playerObj, windowHeight]() {CreateBubble(playerObj, 0, windowHeight); }, ButtonState::Pressed });
		break;
	case(2):
		InputManager::GetInstance()->AddCommand({ {VK_RIGHT, Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_RIGHT, Hardware::Controller, 1} }, new Command{ [playerObj]() {playerObj->SetState(0); }, ButtonState::Down });
		InputManager::GetInstance()->AddCommand({ {VK_LEFT, Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_LEFT, Hardware::Controller, 1} }, new Command{ [playerObj]() {playerObj->SetState(1); }, ButtonState::Down });
		InputManager::GetInstance()->AddCommand({ {VK_RIGHT, Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_RIGHT, Hardware::Controller, 1} }, new Command{ [playerObj]() {playerObj->SetState(2); }, ButtonState::Released });
		InputManager::GetInstance()->AddCommand({ {VK_LEFT, Hardware::KeyBoard}, {XINPUT_GAMEPAD_DPAD_LEFT, Hardware::Controller, 1} }, new Command{ [playerObj]() {playerObj->SetState(3); }, ButtonState::Released });
		InputManager::GetInstance()->AddCommand({ {VK_UP, Hardware::KeyBoard}, {XINPUT_GAMEPAD_A, Hardware::Controller, 1} }, new Command{ [playerObj]() {playerObj->GetComponent<PlayerMovementComponent>()->Jump(); }, ButtonState::Pressed });
		InputManager::GetInstance()->AddCommand({ {VK_NUMPAD0, Hardware::KeyBoard}, {XINPUT_GAMEPAD_X, Hardware::Controller, 1} }, new Command{ [playerObj, windowHeight]() {CreateBubble(playerObj, 1, windowHeight); }, ButtonState::Pressed });
		break;
	default:
		break;
	}

	return playerObj;
}

inline MyEngine::GameObject* CreateFruitDrop(int enemyType, const glm::fvec2& pos)
{
	int cellDimension = currentLevel.WindowHeight / 25;
	MyEngine::GameObject* fruitDrop = new MyEngine::GameObject();
	fruitDrop->AddComponent(new RenderComponent());
	fruitDrop->GetComponent<RenderComponent>()->AddTexture("drops.png", false, false, 0, 0, 0.0f, cellDimension * 2, cellDimension * 2, { 0.5f, 0.5f }, -1, { 0.0f, 0.125f * enemyType }, { 1.0f, 0.125f });
	fruitDrop->AddComponent(new PhysicsComponent(PhysicsComponent::PhysicsType::Kinematic, pos, 0.0f, float(cellDimension), float(cellDimension), 1.0f, 0.3f, 0.0f));
	fruitDrop->AddComponent(new FruitDropComponent());
	return fruitDrop;
}

inline void CreateLevel(int levelNr, const int windowHeight, float delay)
{
	const Level& level = DataHolder::GetInstance()->GetLevel(levelNr);
	Scene* levelScene = new Scene("Level" + std::to_string(levelNr), true);
	Texture2D* blocksTex = ResourceManager::GetInstance()->LoadTexture("blocks.png");
	MyEngine::GameObject* levelObj = new MyEngine::GameObject();
	levelObj->AddComponent(new RenderComponent());
	levelScene->Add(levelObj);
	SceneManager::GetInstance()->AddScene(levelScene);
	SceneManager::GetInstance()->SetSceneActiveDelayed(levelScene->GetName(), delay);
	std::vector<std::vector<bool>> hasCollider(25);
	int cellDimension = windowHeight / 25;
	std::vector<GameObject*>& players = DataHolder::GetInstance()->GetPlayers();
	for (GameObject* pPlayer : players)
		levelScene->Add(pPlayer);

	levelScene->Invoke(
		[players, windowHeight, cellDimension](){
			players[0]->GetComponent<TransformComponent>()->SetPosition(cellDimension * 4.0f, cellDimension * 3.0f);
			players[0]->SetState(2);
			players[1]->GetComponent<TransformComponent>()->SetPosition((windowHeight * 32 / 25) - cellDimension * 4.0f, cellDimension * 3.0f);
			players[1]->SetState(3);}
		, 0.0f);

	levelObj->AddComponent(new TextComponent(std::to_string(levelNr), ResourceManager::GetInstance()->LoadFont("slkscr.ttf", 32), { 255, 255, 255, 255 }, { 0.0f, 0.0f }, 0.0f, { -2.0f, float(windowHeight) + 5.0f}));
	for (size_t cellIndex{}; cellIndex < 32; cellIndex++)
	{
		size_t highestLine = 0;
		bool hasWall = false;
		for (size_t lineIndex{}; lineIndex < 25; lineIndex++)
		{
			hasCollider[lineIndex].push_back(false);
			if (level.Lines[lineIndex][cellIndex] && !(lineIndex == 0 && (cellIndex == 0 || cellIndex == 1)))
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
		levelScene->Add(CreateEnemy(enemy, windowHeight));
	}
}

inline void CreateMenu(int windowHeight)
{
	Scene* pMenuScene = new Scene("MenuScene");
	InputManager::GetInstance()->AddCommand({ {VK_ESCAPE, Hardware::KeyBoard}, {XINPUT_GAMEPAD_START, Hardware::Controller, 0}, {XINPUT_GAMEPAD_START, Hardware::Controller, 1} }, new Command{ []() { InputManager::GetInstance()->Quit(); }, ButtonState::Pressed, "MenuScene" });
	InputManager::GetInstance()->AddCommand({ {VK_ESCAPE, Hardware::KeyBoard}, {XINPUT_GAMEPAD_START, Hardware::Controller, 0}, {XINPUT_GAMEPAD_START, Hardware::Controller, 1} }, new Command{ []() {LevelManager::GetInstance()->Notify(MyEngine::Event(LevelManager::LevelManagerEvent::GoBackToMenu)); }, ButtonState::Pressed, "" });
	InputManager::GetInstance()->AddCommand({ {'1', Hardware::KeyBoard }, {XINPUT_GAMEPAD_LEFT_SHOULDER, Hardware::Controller, 0} }, new Command{ [windowHeight]() {DataHolder::GetInstance()->GetPlayers().back()->SetActive(false); CreateLevel(0, windowHeight, 0.0f); }, ButtonState::Pressed, "MenuScene" });
	InputManager::GetInstance()->AddCommand({ {'2', Hardware::KeyBoard }, {XINPUT_GAMEPAD_RIGHT_SHOULDER, Hardware::Controller, 0}, {XINPUT_GAMEPAD_RIGHT_SHOULDER, Hardware::Controller, 1} }, new Command{ [windowHeight]() {DataHolder::GetInstance()->GetPlayers().back()->SetActive(true); CreateLevel(0, windowHeight, 0.0f); }, ButtonState::Pressed, "MenuScene" });
	SceneManager::GetInstance()->AddScene(pMenuScene);
}