#include "Projectile.h"

#include "Player.h"
#include "PhysicsManager.h"

Projectile::Projectile(Map* map, Vector2 pos, float rotation, GameEntity* user, bool shell)
	: Actor(map), mUser(user), mShell(shell)
{
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	mSpeed = 500.0f;

	mPenetrates = false;
	mReduceDamage = false;

	mSprite = nullptr;
}

Projectile::~Projectile()
{
	delete mSprite;
}

void Projectile::Hit(PhysEntity* other)
{
	if (other == nullptr)
	{
		Active(false);
		return;
	}

	if (std::find(mHitBy.begin(), mHitBy.end(), other->Id()) != mHitBy.end())
		return;

	mHitBy.push_back(other->Id());

	if (mUser != nullptr)
	{
		if (dynamic_cast<Player*>(mUser))
			dynamic_cast<Player*>(mUser)->AddPoints(5);
	}

	//Penetration chance
	if (!mPenetrates || !(rand() % 3))
		Active(false);
	else
		mReduceDamage = true;
}

bool Projectile::CleanUp()
{
	return !(Active());
}

void Projectile::HandleCollision()
{
	if (!PhysicsManager::Instance()->SharingCell(this))
		return;

	Pos(PrevPos());
	do
	{
		Translate(VEC2_RIGHT * sgn(mHorizontalSpeed), local);
		mHorizontalSpeed -= sgn(mHorizontalSpeed);
	} while (!PhysicsManager::Instance()->CheckCollision(this) && mHorizontalSpeed > 0.0f);
}

void Projectile::Update()
{
	if (Active())
	{
		mSprite->Update();

		mHorizontalSpeed = mSpeed * mTimer->DeltaTime();

		Translate(VEC2_RIGHT * mHorizontalSpeed, local);

		HandleCollision();

		if (mReduceDamage)
		{
			mDamage *= mShell ? 0.5f : 0.75f;
			mReduceDamage = false;
		}

		if (HasLeftWall() || HasRightWall() || HasCeiling() || HasGround() || InsideTile())
		{
			//Active(false);
			Hit(nullptr);
		}
	}
}

void Projectile::Render()
{
	if (Active())
		mSprite->Render();

	PhysEntity::Render();
}