#pragma once
#include <vector>
#include "glm\common.hpp"

enum EnemyType
{
	ZenChan = 0,
	HideGons = 1,
	Banebou = 2,
	Pulpul = 3,
	Monsta = 4,
	Drunk = 5,
	Mighta = 6,
	Invader = 7
};

struct Enemy
{
	EnemyType Type;
	int Row, Col;
	float Delay;
};

typedef std::vector<bool> Line;

struct Level
{
	std::vector<Line> Lines;
	std::vector<Enemy> Enemies;
	int WindowWidth, WindowHeight;

	bool IsWallInFront(const glm::fvec2& pos, bool isLookingLeft) const
	{
		int checkCol = int(pos.x / 20.0f + (isLookingLeft ? -1.0f : 1.0f));
		int checkRow = int((WindowHeight - pos.y) / 20.0f - 0.1f);
		if (checkRow < 24)
			return Lines[checkRow][checkCol] || Lines[checkRow + 1][checkCol];
		else
			return Lines[checkRow][checkCol];
	}

	bool IsWallBelow(const glm::fvec2& pos) const
	{
		int checkCol = int(pos.x / 20.0f);
		int checkRow = int((WindowHeight - pos.y) / 20.0f + 1.0f);
		return Lines[checkRow][checkCol];
	}

	bool IsWallAbove(const glm::fvec2& pos) const
	{
		int checkCol = int(pos.x / 20.0f);
		int checkRow = int((WindowHeight - pos.y) / 20.0f - 1.0f);
		return Lines[checkRow][checkCol];
	}
};
