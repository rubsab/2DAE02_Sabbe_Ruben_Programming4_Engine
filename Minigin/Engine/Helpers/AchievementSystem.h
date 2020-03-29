#pragma once
#include "Observer.h"

namespace MyEngine
{
	class AchievementSystem final : public Observer
	{
	public:
		virtual void Notify(const Event event, const GameObject& object, const int value = 0) override
		{
			switch (event)
			{
			default:
				break;
			}
		}
	};
}