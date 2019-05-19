#include "Weapon.h"

#include "Player.h"

Weapon::Weapon(Map* map, Vector2 weaponOffset)
	: mMap(map), mWeaponOffset(weaponOffset)
{
	mTimer = Timer::Instance();

	mAudio = AudioManager::Instance();

	mSprite = nullptr;

	mBulletOffset = VEC2_ZERO;
	mBulletOffset.x = 10.0f;

	mAutomatic = false;

	mName = "NULL";

	mDamage = 25;

	mMaxAmmo = 9999;
	mAmmo = mMaxAmmo;
	mClipSize = 50;
	mClip = mClipSize;

	mFireDelay = 0.07f;
	mFireTimer = 0.0f;

	mReloadDelay = 1.0f;
	mReloadTimer = 0.0f;

	for (int i = 0; i < 4; ++i)
		mPerks[i] = false;

	mAmplified = false;
	mUpgraded = false;
}

Weapon::~Weapon()
{
	delete mSprite;
}

void Weapon::HandleReload()
{
}

void Weapon::CheckPerks()
{
	if (!dynamic_cast<Player*>(Parent()))
		return;

	for (int i = 0; i < 4; ++i)
	{
		if (!mPerks[i] && dynamic_cast<Player*>(Parent())->GetPerk(i))
		{
			mPerks[i] = true;

			switch (i)
			{
			case 0:
				mReloadDelay *= 0.75f;
				break;
			case 1:
				mAmplified = true;
				break;
			case 2:
				mClipSize = round(mClipSize * 1.5f);
				break;
			case 3:
				//Doesn't effect gun.
				break;
			}

			break;
		}
	}
}

int Weapon::Index() const
{
	return mIndex;
}

bool Weapon::Automatic() const
{
	return mAutomatic;
}

std::string Weapon::Name() const
{
	return mName;
}

int Weapon::Ammo() const
{
	return mAmmo;
}

int Weapon::Clip() const
{
	return mClip;
}

int Weapon::ClipSize() const
{
	return mClipSize;
}

bool Weapon::Upgraded() const
{
	return mUpgraded;
}

void Weapon::GiveAmmo()
{
	mAmmo = mMaxAmmo;
}

void Weapon::Aim(float angle)
{
	Rotation(angle);
}

void Weapon::Update()
{
	CheckPerks();

	if (mFireTimer > 0.0f)
		mFireTimer -= mTimer->DeltaTime();

	if (mReloadTimer > 0.0f)
		mReloadTimer -= mTimer->DeltaTime();

	HandleReload();
}

void Weapon::Render()
{
	if (mSprite)
		mSprite->Render();
}