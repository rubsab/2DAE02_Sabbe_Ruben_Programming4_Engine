#pragma once
#include <vector>

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
typedef std::pair<std::vector<Line>, std::vector<Enemy>> Level;
