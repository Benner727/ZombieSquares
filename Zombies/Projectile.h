#pragma once

#include "AudioManager.h"
#include "AnimatedTexture.h"
#include "Actor.h"

class Projectile : public Actor
{
public:
	Projectile(Map* map, Vector2 pos, float rotation, GameEntity* user = nullptr, bool shell = false);
	~Projectile();

private:
	float mHorizontalSpeed;
	float mVerticalSpeed;

protected:
	Timer* mTimer;
	AudioManager* mAudio;

	GameEntity* mUser;

	AnimatedTexture* mSprite;

	float mSpeed;

	bool mPenetrates;
	bool mReduceDamage;
	bool mShell;

	void HandleCollision();

	void Hit(PhysEntity* other) override;

public:
	bool CleanUp();

	void Update();
	void Render();
};
