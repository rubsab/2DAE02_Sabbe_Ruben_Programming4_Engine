#pragma once
#include "Singleton.h"

namespace MyEngine
{
	class GameObject;
	enum class Event
	{
		
	};

	class Observer : public Singleton<Observer>
	{
	public:
		virtual void Notify(const Event event, const GameObject& object, const int value = 0) = 0;
	};
}