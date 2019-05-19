#pragma once

#include "Enemy.h"

class Zombie : public Enemy
{
public:
	Zombie(Map* map, GameEntity* target = nullptr);
	~Zombie();

private:
	bool mAngry;

public:
	void Hit(PhysEntity* other) override;
};