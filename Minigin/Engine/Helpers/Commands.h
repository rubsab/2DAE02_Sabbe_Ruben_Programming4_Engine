#pragma once
namespace MyEngine
{
	class GameObject;
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute(GameObject* object) = 0;
	};
}