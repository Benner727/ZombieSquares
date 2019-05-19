#pragma once

#include "Gun.h"

class WeaponLoader
{
public:
	WeaponLoader(Map* map);
	~WeaponLoader();

private:
	Map* mMap;

	std::vector<Weapon*> mWeapons;

	void LoadGuns();

public:
	std::vector<Weapon*> GetWeapons() const { return mWeapons; }

	Weapon* GetWeapon(int index);
	int TotalWeapons() const { return mWeapons.size(); }
};