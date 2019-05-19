#pragma once

#include "Enemy.h"

class Stalker : public Enemy
{
public:
	Stalker(Map* map, GameEntity* target = nullptr);
	~Stalker();

private:
	bool mStealth;
	float mStealthTimer;

	void HandleAttack();

	void Hit(PhysEntity* other) override;

public:
	void Render() override;
};

