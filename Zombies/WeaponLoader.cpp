#include "WeaponLoader.h"

WeaponLoader::WeaponLoader(Map* map)
	: mMap(map)
{
	LoadGuns();
}


WeaponLoader::~WeaponLoader()
{
}

void WeaponLoader::LoadGuns()
{
	std::string fullPath = basePath;
	fullPath.append("Assets/guns.txt");
	std::ifstream infile(fullPath);
	if (!infile) std::cerr << "guns.txt could not be opened!" << std::endl;

	Gun gun = Gun(mMap);
	while (infile >> gun)
	{
		mWeapons.push_back(new Gun(gun));
	}
}

Weapon* WeaponLoader::GetWeapon(int index)
{
	for (auto& weapon : mWeapons)
	{
		if (weapon->Index() == index)
			return weapon->clone();
	}

	return nullptr;
}