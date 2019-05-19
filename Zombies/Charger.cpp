#include "Charger.h"

#include "PhysicsManager.h"

Charger::Charger(Map* map, GameEntity* target)
	: Enemy(map, target)
{
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mSprite = new Texture("ChargerSprite.png");
	mSprite->Parent(this);

	mDirection = -1;
	mRepathTimer = 0.0f;

	mSpeed = 30.0f;
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	mHealth = 100;
	mDamage = 50;
	mDead = false;

	mCharging = false;
	mChargeTimer = rand() % 10 + 10;

	mRechargeTimer = 0.0f;

	AddCollider(new BoxCollider(Vector2(mSprite->ScaledDimensions().x, mSprite->ScaledDimensions().y)));

	mCollisionLayer = CollisionLayers::Hostile;
	mId = PhysicsManager::Instance()->RegisterEntity(this, mCollisionLayer);
}


Charger::~Charger()
{
}

void Charger::HandleMovement()
{
	if (!mCharging)
	{
		mHorizontalSpeed = 0.0f;
		mVerticalSpeed = 0.0f;
	}

	if (mRechargeTimer > 0.0f)
	{
		mRechargeTimer -= mTimer->DeltaTime();
		return;
	}

	if (mTarget == nullptr)
		return;

	if (!mCharging && mChargeTimer <= 0.0f)
	{
		if (mHasLOS)
		{
			mChargeDirection = (mTarget->Pos() - Pos(GameEntity::world)).Normalize();
			mVerticalSpeed = mChargeDirection.y * -mSpeed * mTimer->DeltaTime();
			mHorizontalSpeed = mChargeDirection.x * mSpeed * mTimer->DeltaTime();

			mCharging = true;
			mChargeTimer = rand() % 10 + 6.0f;
			mSpeed *= 10.0f;
		}
	}

	if (mCharging)
	{
		mVerticalSpeed = mChargeDirection.y * -mSpeed * mTimer->DeltaTime();
		mHorizontalSpeed = mChargeDirection.x * mSpeed * mTimer->DeltaTime();
		return;
	}
	else
		mChargeTimer -= mTimer->DeltaTime();

	if ((Pos() - mTarget->Pos()).Magnitude() <= mSprite->ScaledDimensions().x)
		return; //Attack

	HandlePathing();
}

void Charger::HandleCollision()
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
		if (mCharging)
		{
			mCharging = false;
			mSpeed /= 10.0f;
			mRechargeTimer = 1.0f;
		}
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
		if (mCharging)
		{
			mCharging = false;
			mSpeed /= 10.0f;
			mRechargeTimer = 1.0f;
		}
	}

	if (OutsideMap())
		mDead = true;
}