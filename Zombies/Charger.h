#pragma once

#include "Enemy.h"

class Charger : public Enemy
{
public:
	Charger(Map* map, GameEntity* target = nullptr);
	~Charger();

private:
	bool mCharging;
	float mChargeTimer;
	Vector2 mChargeDirection;

	float mRechargeTimer;

	void HandleMovement();
	void HandleCollision();
};

