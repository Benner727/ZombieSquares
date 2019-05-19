#pragma once

#include "Weapon.h"
#include "InputManager.h"

class Player : public Actor
{
public:
	Player(Map* map);
	~Player();

private:
	InputManager* mInput;
	Timer* mTimer;
	AudioManager* mAudio;

	Texture* mSprite;

	std::vector<Weapon*> mWeapon;
	int mWeaponSlot;

	float mHorizontalSpeed;
	float mVerticalSpeed;
	float mSpeed;

	float mSprintTime;
	bool mSprinting;
	bool mSprintRecharge;

	bool mPerks[4];

	int mPoints;
	int mKills;

	int mFullHealth;
	int mFullShield;

	int mShield;
	float mHealTimer;
	bool mDead;
	float mImmunityTimer;

	float mDoublePointsTimer;

	void HandleMovement();
	void HandleCollision();
	void HandleGun();

	void Hit(PhysEntity* other) override;

public:
	void Health(int health);
	int Health() const;
	int Shield() const;

	std::string WeaponName() const;
	int Ammo() const;
	int Clip() const;
	int ClipSize() const;

	void GiveAmmo(int id = -1);
	void FillAmmo();
	bool HasGun(int id) const;
	void GiveWeapon(Weapon* weapon);
	Weapon* GetWeapon() { return mWeapon[mWeaponSlot]; }

	void SetPerk(int id, bool val);
	bool GetPerk(int id) const;

	void AddPoints(int points);
	int Points() const;
	bool DoublePoints() const { return (mDoublePointsTimer > 0.0f); }

	void AddKill();
	int Kills() const;

	bool Dead() const;

	void Update();
	void Render();
};


