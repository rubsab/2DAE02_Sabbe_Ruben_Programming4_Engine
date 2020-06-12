#pragma once
#include "Singleton.h"

namespace MyEngine
{
	class GameObject;
	enum Event
	{
		e0, e1, e2, e3, e4, e5, e6, e7, e8, e9
	};

	template <class T>
	class Observer : public Singleton<T>
	{
	public:
		virtual void Notify(const Event event, const int value = 0, const GameObject* object = nullptr) = 0;
	};
}