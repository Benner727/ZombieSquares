#include "Gun.h"

#include <iomanip>

#include "Bullet.h"
#include "Explosive.h"

Gun::Gun(Map* map, Vector2 weaponOffset)
	: Weapon(map, weaponOffset)
{
	Pos(mWeaponOffset);

	mIndex = 0;

	mAutomatic = false;

	mName = "9mm Pistol";

	mMaxAmmo = 75;
	mAmmo = mMaxAmmo;
	mClipSize = 15;
	mClip = mClipSize;

	mDamage = 25;

	mPenetrates = false;

	mType = 3;

	mFireDelay = 0.2f;
	mFireTimer = 0.0f;

	mReloadDelay = 2.13f;
	mReloadTimer = 0.0f;
	
	mReloading = false;
	mSingleReload = true;

	for (int i = 0; i < 4; ++i)
		mPerks[i] = false;
}

Gun::~Gun()
{
}

void Gun::HandleReload()
{
	if (mReloading && mReloadTimer <= 0.0f)
	{
		if (mSingleReload)
		{
			int reloadAmount = mClipSize - mClip;

			if (reloadAmount <= mAmmo)
			{
				mAmmo -= reloadAmount;
				mClip = mClipSize;
			}
			else
			{
				mClip += mAmmo;
				mAmmo = 0;
			}
		}
		else if (mClip < mClipSize && mAmmo > 0)
		{
			mClip++;
			mAmmo--;
			mReloadTimer = mReloadDelay;

			if (mClip == mClipSize || !mAmmo)
				mAudio->StopSFX(2);
			else
				mAudio->PlaySFX("reload.wav", 0, 2);
		}
	}
}

void Gun::CancelReload()
{
	mReloadTimer = 0.0f;
	mReloading = false;
	mAudio->StopSFX(2);
}

void Gun::Reload()
{
	if (mReloading)
		return;

	if (mAmmo > 0)
	{
		if (mClip != mClipSize)
		{
			mReloading = true;
			mReloadTimer = mReloadDelay;

			if (!mSingleReload)
				mReloadTimer *= 3.0f;

			if (mReloadTimer > 0.0f)
				mAudio->PlaySFX("reload.wav", 0, 2);
		}
	}
}

bool Gun::Fire()
{
	if (mClip > 0)
	{
		CancelReload();

		if (mFireTimer <= 0.0f)
		{
			Graphics::Instance()->CameraShake();

			Bullet* bullet = nullptr;
			switch (mType)
			{
			case 1: //Single Shot
				bullet = new Bullet(mMap, Pos() + RotateVector(mBulletOffset, Rotation()), Rotation(), Parent());
				bullet->Damage(mDamage * (mAmplified ? 1.25f : 1.0f));
				bullet->Penetrates(mPenetrates);
				ProjectileManager::Instance()->AddProjectile(bullet);
				break;
			case 2: //Double Shot
				bullet = new Bullet(mMap, Pos() + RotateVector(mBulletOffset, Rotation()), Rotation(), Parent());
				bullet->Translate(VEC2_UP * 8.0f, GameEntity::SPACE::local);
				bullet->Damage(mDamage * (mAmplified ? 1.25f : 1.0f));
				bullet->Penetrates(mPenetrates);
				ProjectileManager::Instance()->AddProjectile(bullet);

				bullet = new Bullet(mMap, Pos() + RotateVector(mBulletOffset, Rotation()), Rotation(), Parent());
				bullet->Translate(-VEC2_UP * 8.0f, GameEntity::SPACE::local);
				bullet->Damage(mDamage * (mAmplified ? 1.25f : 1.0f));
				bullet->Penetrates(mPenetrates);
				ProjectileManager::Instance()->AddProjectile(bullet);
				break;
			case 3: //Triple Shot
				for (int i = -9; i <= 9; i += 9)
				{
					bullet = new Bullet(mMap, Pos() + RotateVector(mBulletOffset, Rotation()), Rotation() + i, Parent());
					bullet->Damage(mDamage * (mAmplified ? 1.25f : 1.0f));
					bullet->Penetrates(mPenetrates);
					ProjectileManager::Instance()->AddProjectile(bullet);
				}
				break;
			case 4: //Explosive
				ProjectileManager::Instance()->AddProjectile(new Explosive(mMap, Pos() + RotateVector(mBulletOffset, Rotation()), Rotation(), mUpgraded, Parent()));
				break;
			}

			mFireTimer = mFireDelay;


			if (mType == 1 && mPenetrates) //Rifle
				mAudio->PlaySFX("rifle.wav", 0, 1);
			else if (mType == 3 && mPenetrates) //Shotgun
				mAudio->PlaySFX("shotgun.wav", 0, 1);
			else if (mType == 1 && mAutomatic) //Machine gun
				mAudio->PlaySFX("cg1.wav", 0, 1);
			else if (mType == 1 || mType == 2) //Pistol
				mAudio->PlaySFX("pistol.wav", 0, 1);

			mClip--;
			if (mClip < 1)
			{
				Reload();
			}
		}

		return true;
	}
	else if (!mReloading)
		Reload();

	return false;
}

void Gun::Upgrade()
{
	CancelReload();

	mDamage *= 2.0f;
	mClip = mClipSize;
	mMaxAmmo *= 1.5f;
	mAmmo = mMaxAmmo;

	mUpgraded = true;
}

Gun* Gun::clone() const
{
	return new Gun(*this);
}

std::istream & operator>>(std::istream & is, Gun & gun)
{
	is >> gun.mIndex >> std::quoted(gun.mName)
		>> gun.mAutomatic >> gun.mClipSize >> gun.mMaxAmmo
		>> gun.mDamage >> gun.mFireDelay >> gun.mReloadDelay
		>> gun.mType >> gun.mSingleReload >> gun.mPenetrates;

	gun.mClip = gun.mClipSize;
	gun.mAmmo = gun.mMaxAmmo;

	return is;
}