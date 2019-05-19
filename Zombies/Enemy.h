#pragma once

#include "Actor.h"
#include "PathFinder.h"
#include "FOV.h"
#include "LOS.h"
#include "AudioManager.h"

#include <random>
#include <ctime>

class Enemy : public Actor
{
public:
	Enemy(Map* map, GameEntity* target = nullptr);
	~Enemy();

protected:
	AudioManager* mAudio;
	Timer* mTimer;

	Texture* mSprite;

	GameEntity* mTarget;

	int mDirection;
	std::list<Vector2> mPath;
	float mRepathTimer;

	LOS* mLOS;
	bool mHasLOS;
	float mLOSTimer;

	float mHorizontalSpeed;
	float mVerticalSpeed;
	float mSpeed;

	int mHealth;
	bool mDead;

	void FindPath();
	void CheckLOS();

	virtual void HandleMovement();
	virtual void HandleAttack();
	virtual void HandleCollision();
	virtual void HandlePathing();

	bool IgnoreCollisions() override;
	void Hit(PhysEntity* other) override;

public:
	void Damage(int damage);
	void Health(int health);
	int Health() const;

	bool Dead() const;

	void Update();
	void Render();
};

