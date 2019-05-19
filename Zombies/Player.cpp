#include "Player.h"

#include "PhysicsManager.h"
#include "PowerUp.h"

Player::Player(Map* map)
	: Actor(map)
{
	mInput = InputManager::Instance();
	mTimer = Timer::Instance();
	mAudio = AudioManager::Instance();

	mSprite = new Texture("HumanSprite.png");
	mSprite->Parent(this);

	mNonPlayer = false;

	mWeaponSlot = 0;

	mSpeed = 75.0f;
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	mSprintTime = 3.0f;
	mSprinting = false;
	mSprintRecharge = false;

	for (int i = 0; i < 4; ++i)
		mPerks[i] = false;

	mPoints = 0;
	mKills = 0;

	mFullHealth = 100;
	mHealth = mFullHealth;
	
	mFullShield = 50;
	mShield = mFullShield;

	mHealTimer = 1.0f;
	mDead = false;

	mImmunityTimer = 0.0f;

	mDoublePointsTimer = 0.0f;

	AddCollider(new BoxCollider(Vector2(mSprite->ScaledDimensions().x, mSprite->ScaledDimensions().y)));

	mCollisionLayer = CollisionLayers::Friendly;
	mId = PhysicsManager::Instance()->RegisterEntity(this, mCollisionLayer);
}

Player::~Player()
{
	delete mSprite;

	for (std::vector<Weapon*>::iterator it = mWeapon.begin(); it != mWeapon.end(); ++it)
		delete * it;
}

void Player::HandleMovement()
{
	mHorizontalSpeed = 0.0f;
	mVerticalSpeed = 0.0f;

	mSprinting = false;

	if (mInput->KeyDown(SDL_SCANCODE_A)) //Move Left
	{
		mHorizontalSpeed = -mSpeed * mTimer->DeltaTime();
	}
	else if (mInput->KeyDown(SDL_SCANCODE_D)) //Move Right
	{
		mHorizontalSpeed = mSpeed * mTimer->DeltaTime();
	}

	if (mInput->KeyDown(SDL_SCANCODE_W)) //Move Up
	{
		mVerticalSpeed = mSpeed * mTimer->DeltaTime();
	}
	else if (mInput->KeyDown(SDL_SCANCODE_S)) //Move Down
	{
		mVerticalSpeed = -mSpeed * mTimer->DeltaTime();
	}

	if (mInput->KeyDown(SDL_SCANCODE_LSHIFT)) //Sprint
	{
		if (mSprintTime > 0.0f && !mSprintRecharge)
			mSprinting = true;
	}

	if (mSprinting)
	{
		mHorizontalSpeed *= 1.66f;
		mVerticalSpeed *= 1.66f;

		mSprintTime -= mTimer->DeltaTime();

		if (mSprintTime <= 0.0f)
			mSprintRecharge = true;
	}
	else if (mSprintTime < 3.0f)
		mSprintTime += mTimer->DeltaTime() * 0.5f;

	if (mSprintTime >= 3.0f)
	{
		mSprintTime = 3.0f;
		mSprintRecharge = false;
	}
}

void Player::HandleCollision()
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

void Player::HandleGun()
{
	if (mWeapon.size() < 1)
		return;

	int prevSlot = mWeaponSlot;

	if (mInput->KeyPressed(SDL_SCANCODE_Q))
		--mWeaponSlot;
	else if (mInput->KeyPressed(SDL_SCANCODE_E))
		++mWeaponSlot;

	if (prevSlot != mWeaponSlot) mWeapon[prevSlot]->CancelReload();

	if (mInput->KeyPressed(SDL_SCANCODE_1))
		mWeaponSlot = 0;
	else if (mInput->KeyPressed(SDL_SCANCODE_2) && mWeapon.size() > 1)
		mWeaponSlot = 1;
	else if (mInput->KeyPressed(SDL_SCANCODE_3) && mWeapon.size() > 2)
		mWeaponSlot = 2;

	if (mWeaponSlot < 0)
		mWeaponSlot = mWeapon.size() - 1;
	else if (mWeaponSlot > mWeapon.size() - 1)
		mWeaponSlot = 0;

	if (mInput->KeyDown(SDL_SCANCODE_R)) //Reload
		mWeapon[mWeaponSlot]->Reload();

	Vector2 mousePos = mInput->MousePos() + Vector2(Graphics::Instance()->Camera()->x, Graphics::Instance()->Camera()->y);
	Vector2 direction = (mousePos - Pos(GameEntity::world)).Normalize();
	float angle = atan2(direction.y, direction.x) * RAD_TO_DEG;
	mWeapon[mWeaponSlot]->Aim(angle);

	if ((mInput->MouseButtonDown(InputManager::MOUSE_BUTTON::left) && mWeapon[mWeaponSlot]->Automatic())
		|| mInput->MouseButtonPressed(InputManager::MOUSE_BUTTON::left))
	{
		mWeapon[mWeaponSlot]->Fire();
	}

	mWeapon[mWeaponSlot]->Update();
}

void Player::Hit(PhysEntity* other)
{
	if (other == nullptr)
		return;

	if (std::find(mHitBy.begin(), mHitBy.end(), other->Id()) != mHitBy.end())
		return;

	if (dynamic_cast<PowerUp*>(other))
	{
		switch (dynamic_cast<PowerUp*>(other)->Type())
		{
		case 0: //Health
			mHealth = mFullHealth;
			break;
		case 1: //Points
			mDoublePointsTimer = 30.0f;
			break;
		case 2: //Ammo
			FillAmmo();
			break;
		}

		mAudio->PlaySFX("pickCoin.wav", 0, 3);
		return;
	}

	mHitBy.push_back(other->Id());

	mSprite->FlashRed();
	
	if (mShield >= other->Damage())
		mShield -= other->Damage();
	else
	{
		mHealth = (mHealth - other->Damage()) + mShield;
		mShield = 0;
	}

	if (mHealth < 0) mHealth = 0;

	mDead = (mHealth <= 0);
}

void Player::Health(int health)
{
	mHealth = health;
}

int Player::Health() const
{
	return mHealth;
}

int Player::Shield() const
{
	return mShield;
}

std::string Player::WeaponName() const
{
	return mWeapon[mWeaponSlot]->Name();
}

int Player::Ammo() const
{
	return mWeapon[mWeaponSlot]->Ammo();
}

int Player::Clip() const
{
	return mWeapon[mWeaponSlot]->Clip();
}

int Player::ClipSize() const
{
	return mWeapon[mWeaponSlot]->ClipSize();
}

void Player::GiveAmmo(int id)
{
	if (id == -1)
		mWeapon[mWeaponSlot]->GiveAmmo();
	else
	{
		for (auto& weapon : mWeapon)
		{
			if (weapon->Index() == id)
			{
				weapon->GiveAmmo();
				break;
			}
		}
	}
}

void Player::FillAmmo()
{
	for (auto* weapon : mWeapon)
		weapon->GiveAmmo();
}

bool Player::HasGun(int id) const
{
	for (auto& weapon : mWeapon)
	{
		if (weapon->Index() == id)
			return true;
	}

	return false;
}

void Player::GiveWeapon(Weapon * weapon)
{
	if (!HasGun(weapon->Index()))
	{
		if (mWeapon.size() < 3)
		{
			mWeapon.push_back(weapon);
			mWeapon.back()->Pos(Pos());
			mWeapon.back()->Parent(this);
			mWeapon[mWeaponSlot]->CancelReload();
			mWeaponSlot = mWeapon.size() - 1;
		}
		else
		{
			delete mWeapon[mWeaponSlot];
			mWeapon[mWeaponSlot] = weapon;
			mWeapon[mWeaponSlot]->Pos(Pos());
			mWeapon[mWeaponSlot]->Parent(this);
		}
	}
}

void Player::SetPerk(int id, bool val)
{
	if (id < 0 || id > 3)
		return;

	mPerks[id] = val;

	if (id == 3 && val)
	{
		float ratio = (float)mHealth / mFullHealth;
		
		mFullHealth *= 1.25f;
		mFullShield *= 1.25f;

		mHealth = mFullHealth * ratio;
	}
}

bool Player::GetPerk(int id) const
{
	if (id < 0 || id > 3)
		return false;

	return mPerks[id];
}

void Player::AddPoints(int points)
{
	if (DoublePoints() && points > 0)
		points *= 2;

	if (points < 0)
		mAudio->PlaySFX("buySound.wav", 0, 3);

	mPoints += points;
}

int Player::Points() const
{
	return mPoints;
}

void Player::AddKill()
{
	mKills++;
}

int Player::Kills() const
{
	return mKills;
}

bool Player::Dead() const
{
	return mDead;
}

void Player::Update()
{
	HandleMovement();
	HandleCollision();
	HandleGun();

	if (mHealTimer <= 0.0f)
	{
		if (mShield < mFullShield)
			mShield++;
		mHealTimer = 1.0f;
	}
	else
		mHealTimer -= mTimer->DeltaTime();

	if (mImmunityTimer <= 0.0f)
	{
		mHitBy.clear();
		mImmunityTimer = 0.33f;
	}
	else if (mHitBy.size())
		mImmunityTimer -= mTimer->DeltaTime();

	if (mDoublePointsTimer > 0.0f)
		mDoublePointsTimer -= mTimer->DeltaTime();

	Graphics::Instance()->Camera(Pos());
}

void Player::Render()
{
	mSprite->Render();

	if (mWeapon.size())
		mWeapon[mWeaponSlot]->Render();

	PhysEntity::Render();
}