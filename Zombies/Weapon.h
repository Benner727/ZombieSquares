#pragma once

#include "ProjectileManager.h"

class Weapon : public GameEntity
{
public:
	Weapon(Map* map, Vector2 weaponOffset = VEC2_ZERO);
	~Weapon();

private:
	Timer* mTimer;

protected:
	AudioManager* mAudio;

	Texture* mSprite;
	std::string mName;

	Map* mMap;

	Vector2 mWeaponOffset;
	Vector2 mBulletOffset;

	int mIndex;

	bool mAutomatic;

	int mDamage;

	int mMaxAmmo;
	int mAmmo;
	int mClipSize;
	int mClip;

	float mFireDelay;
	float mFireTimer;

	float mReloadDelay;
	float mReloadTimer;

	bool mPerks[4];

	bool mAmplified;
	bool mUpgraded;

	virtual void HandleReload();
	void CheckPerks();

public:
	int Index() const;

	bool Automatic() const;

	std::string Name() const;

	int Ammo() const;
	int Clip() const;
	int ClipSize() const;
	bool Upgraded() const;
	
	void GiveAmmo();

	virtual void CancelReload() = 0;
	virtual void Reload() = 0;
	virtual bool Fire() = 0;
	virtual void Upgrade() = 0;

	void Aim(float angle);

	void Update();
	void Render();

	virtual Weapon* clone() const = 0;
};