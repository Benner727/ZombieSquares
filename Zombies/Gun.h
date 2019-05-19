#pragma once

#include "Weapon.h"

class Gun : public Weapon
{
public:
	Gun(Map* map, Vector2 weaponOffset = VEC2_ZERO);
	~Gun();

private:
	bool mPenetrates;

	int mType; //1=Single, 2=Burst, 3=Spread

	bool mSingleReload;
	bool mReloading;

	void HandleReload();

public:
	void CancelReload();
	void Reload();
	bool Fire();
	void Upgrade();

	friend std::istream& operator>>(std::istream& is, Gun& gun);

	virtual Gun* clone() const override;
};