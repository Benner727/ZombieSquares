#pragma once

#include "Projectile.h"

class ProjectileManager
{
private:
	static ProjectileManager* sInstance;

	ProjectileManager();
	~ProjectileManager();

	std::vector<Projectile*> mProjectiles;

public:
	static ProjectileManager* Instance();
	static void Release();

	void AddProjectile(Projectile* projectile);

	void Update();
	void Render();
};

