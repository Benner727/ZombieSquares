#include "Enemy.h"

#include "PhysicsManager.h"
#include "EventManager.h"
#include "PowerUp.h"

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<> pathDelay(0.5, 1.0);
std::uniform_real_distribution<> losDelay(0.25, 0.75);

Enemy::Enemy(Map* map, GameEntity* target)
	: Actor(map), mTarget(target)
{
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mSprite = nullptr;

	mDirection = -1;
	mRepathTimer = 0.0f;

	mLOS = new LOS(map);
	mHasLOS = false;
	mLOSTimer = 0.0f;

	mSpeed = 50.0f;
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	mHealth = 100;
	mDamage = 1;
	mDead = false;
}

Enemy::~Enemy()
{
	delete mSprite;

	delete mLOS;

	if (mDead && !(rand() % 35))
		EventManager::Instance()->AddEvent(new PowerUp(mMap, Pos()));
}

void Enemy::FindPath()
{
	if (mRepathTimer <= 0.0f)
	{
		PathFinder pathFinder(mMap);
		std::list<point> path = pathFinder.generatePath(Pos(), mTarget->Pos());

		mRepathTimer = pathDelay(mt);

		if ((mTarget->Pos() - Pos()).Magnitude() > 20.0f * mMap->mTileSize)
			mRepathTimer *= 4.0f;

		Vector2 nextPos;
		int j = 0;

		mPath.clear();
		for (std::list<point>::iterator i = path.begin(); i != path.end(); i++) {
			mPath.push_back(Vector2((*i).x * mMap->mTileSize + mMap->mTileSize * 0.5f, (*i).y * mMap->mTileSize + mMap->mTileSize * 0.5f));

			if (j == 1)
				nextPos = mPath.back();
			j++;
		}

		if (j < 2)
			return;

		if ((Pos() - mPath.front()).Magnitude() < 1.0f)
			mPath.pop_front();
		else
		{
			Vector2 dir = (nextPos - mPath.front()).Normalize();
			float angle = atan2(dir.y, dir.x);
			angle *= RAD_TO_DEG;

			if (abs(angle) == 45.0f || abs(angle) == 135.0f)
			{
				if ((Pos() - nextPos).Magnitude() < floor(mMap->mTileSize * 1.4) - 1)
					mPath.pop_front();
			}
			else if ((Pos() - nextPos).Magnitude() < mMap->mTileSize - 1)
				mPath.pop_front();
		}
	}
}

void Enemy::CheckLOS()
{
	if (mLOSTimer > 0.0f)
	{
		mLOSTimer -= mTimer->DeltaTime();
		return;
	}

	mHasLOS = false;
	mLOSTimer = losDelay(mt);

	if (mTarget == nullptr)
		return;

	if ((mTarget->Pos() - Pos()).Magnitude() > 16.0f * mMap->mTileSize)
		return;

	if (mLOS->inLOS(Pos() + Vector2(mSprite->ScaledDimensions().x * 0.53f, mSprite->ScaledDimensions().y * 0.53f), mTarget->Pos() + Vector2(mSprite->ScaledDimensions().x * 0.53f, mSprite->ScaledDimensions().y * 0.53f))
		&& mLOS->inLOS(Pos() + Vector2(mSprite->ScaledDimensions().x * 0.53f, -mSprite->ScaledDimensions().y * 0.53f), mTarget->Pos() + Vector2(mSprite->ScaledDimensions().x * 0.53f, -mSprite->ScaledDimensions().y * 0.53f))
		&& mLOS->inLOS(Pos() + Vector2(-mSprite->ScaledDimensions().x * 0.53f, mSprite->ScaledDimensions().y * 0.53f), mTarget->Pos() + Vector2(-mSprite->ScaledDimensions().x * 0.53f, mSprite->ScaledDimensions().y * 0.53f))
		&& mLOS->inLOS(Pos() + Vector2(-mSprite->ScaledDimensions().x * 0.53f, -mSprite->ScaledDimensions().y * 0.53f), mTarget->Pos() + Vector2(-mSprite->ScaledDimensions().x * 0.53f, -mSprite->ScaledDimensions().y * 0.53f)))
	{
		mHasLOS = true;
	}
}

void Enemy::HandlePathing()
{
	FindPath();

	if (!mPath.empty())
	{
		if ((Pos() - mPath.front()).Magnitude() < 1.0f)
			mPath.pop_front();

		Vector2 dir = (mPath.front() - Pos(GameEntity::world)).Normalize();
		float angle = atan2(dir.y, dir.x);

		mHorizontalSpeed = cos(angle) * mSpeed * mTimer->DeltaTime();
		mVerticalSpeed = sin(angle) * -mSpeed * mTimer->DeltaTime();

		//std::cout << "Start: " << Pos().x << ", " << Pos().y << std::endl;
		//std::cout << "Goal: " << mPath.front().x << " , " << mPath.front().y << std::endl;
		//std::cout << "Direction: " << mHorizontalSpeed << ", " << mVerticalSpeed << std::endl;
		//system("PAUSE>nul");
	}
}

void Enemy::HandleMovement()
{
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	if (mTarget == nullptr)
		return;

	if ((Pos() - mTarget->Pos()).Magnitude() <= mSprite->ScaledDimensions().x)
		return; //Attack

	if (mHasLOS)
	{
		Vector2 dir = (mTarget->Pos() - Pos(GameEntity::world)).Normalize();
		float angle = atan2(dir.y, dir.x);

		mVerticalSpeed = sin(angle) * -mSpeed * mTimer->DeltaTime();
		mHorizontalSpeed = cos(angle) * mSpeed * mTimer->DeltaTime();

		return;
	}

	HandlePathing();
}

void Enemy::HandleAttack()
{
}

void Enemy::HandleCollision()
{
	Translate(VEC2_RIGHT * mHorizontalSpeed);
	if (HasLeftWall() || HasRightWall())
	{
		Pos(PrevPos());
		do
		{
			Translate(VEC2_RIGHT * sgn(mHorizontalSpeed));
			mHorizontalSpeed -= sgn(mHorizontalSpeed);
		} while (!HasLeftWall() && !HasRightWall());
		Pos(PrevPos());

		mHorizontalSpeed = 0.0f;
	}

	Translate(VEC2_UP * mVerticalSpeed);
	if (HasGround() || HasCeiling())
	{
		Pos(PrevPos());
		do
		{
			Translate(VEC2_UP * sgn(mVerticalSpeed));
			mHorizontalSpeed -= sgn(mVerticalSpeed);
		} while (!HasGround() && !HasCeiling());
		Pos(PrevPos());

		mVerticalSpeed = 0.0f;
	}

	if (OutsideMap())
		mDead = true;
}

bool Enemy::IgnoreCollisions()
{
	return mDead;
}

void Enemy::Hit(PhysEntity* other)
{
	if (other == nullptr)
		return;

	if (other->CollisionLayer() == PhysEntity::CollisionLayers::FriendlyProjectiles)
	{
		if (std::find(mHitBy.begin(), mHitBy.end(), other->Id()) != mHitBy.end())
			return;

		mHitBy.push_back(other->Id());

		mSprite->FlashRed();

		mHealth -= other->Damage();

		mDead = (mHealth <= 0);

		if (mDead)
			mAudio->PlaySFX("fall.wav", 0, 5);
	}
}

void Enemy::Damage(int damage)
{
	mDamage = damage;
}

void Enemy::Health(int health)
{
	mHealth = health;
}

int Enemy::Health() const
{
	return mHealth;
}

bool Enemy::Dead() const
{
	return mDead;
}

void Enemy::Update()
{
	CheckLOS();
	HandleMovement();
	HandleCollision();
	HandleAttack();

	if (mRepathTimer > 0.0f)
		mRepathTimer -= mTimer->DeltaTime();
}

void Enemy::Render()
{
	mSprite->Render();

	PhysEntity::Render();
}
