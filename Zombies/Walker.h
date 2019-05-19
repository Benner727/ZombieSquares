#pragma once

#include "Enemy.h"

class Walker : public Enemy
{
public:
	Walker(Map* map, GameEntity* target = nullptr);
	~Walker();

private:
	float mAttackTimer;

	void HandleMovement();
	void HandleAttack();
};

