#include "InputManager.h"

InputManager* InputManager::sInstance = nullptr;

InputManager* InputManager::Instance()
{
	if (sInstance == nullptr)
		sInstance = new InputManager();

	return sInstance;
}

void InputManager::Release()
{
	delete sInstance;
	sInstance = nullptr;
}

InputManager::InputManager()
{
	mKeyboardState = SDL_GetKeyboardState(&mKeyLength);
	mPrevKeyboardState = new Uint8[mKeyLength];
	memcpy(mPrevKeyboardState, mKeyboardState, mKeyLength);

	mLastKey = SDL_SCANCODE_UNKNOWN;
}

InputManager::~InputManager()
{
	delete[] mPrevKeyboardState;
	mPrevKeyboardState = nullptr;
}

bool InputManager::KeyDown(SDL_Scancode scanCode)
{
	return mKeyboardState[scanCode];
}

bool InputManager::KeyPressed(SDL_Scancode scanCode)
{
	return !mPrevKeyboardState[scanCode] && mKeyboardState[scanCode];
}

bool InputManager::KeyReleased(SDL_Scancode scanCode)
{
	return mPrevKeyboardState[scanCode] && !mKeyboardState[scanCode];
}

Vector2 InputManager::MousePos()
{
	return Vector2((float)mMouseXPos, (float)mMouseYPos);
}

void InputManager::LastKey(SDL_Scancode scanCode)
{
	mLastKey = scanCode;
}

SDL_Scancode InputManager::LastKey()
{
	return mLastKey;
}

bool InputManager::MouseButtonDown(MOUSE_BUTTON button)
{
	Uint32 mask = 0;

	switch (button)
	{
	case left:
		mask = SDL_BUTTON_LMASK;
		break;
	case right:
		mask = SDL_BUTTON_RMASK;
		break;
	case middle:
		mask = SDL_BUTTON_MMASK;
		break;
	case back:
		mask = SDL_BUTTON_X1MASK;
		break;
	case forward:
		mask = SDL_BUTTON_X2MASK;
		break;
	}

	return (mMouseState & mask);
}

bool InputManager::MouseButtonPressed(MOUSE_BUTTON button)
{
	Uint32 mask = 0;

	switch (button)
	{
	case left:
		mask = SDL_BUTTON_LMASK;
		break;
	case right:
		mask = SDL_BUTTON_RMASK;
		break;
	case middle:
		mask = SDL_BUTTON_MMASK;
		break;
	case back:
		mask = SDL_BUTTON_X1MASK;
		break;
	case forward:
		mask = SDL_BUTTON_X2MASK;
		break;
	}

	return !(mPrevMouseState & mask) && (mMouseState & mask);
}

bool InputManager::MouseButtonReleased(MOUSE_BUTTON button)
{
	Uint32 mask = 0;

	switch (button)
	{
	case left:
		mask = SDL_BUTTON_LMASK;
		break;
	case right:
		mask = SDL_BUTTON_RMASK;
		break;
	case middle:
		mask = SDL_BUTTON_MMASK;
		break;
	case back:
		mask = SDL_BUTTON_X1MASK;
		break;
	case forward:
		mask = SDL_BUTTON_X2MASK;
		break;
	}

	return (mPrevMouseState & mask) && !(mMouseState & mask);
}

void InputManager::Update()
{
	mMouseState = SDL_GetMouseState(&mMouseXPos, &mMouseYPos);
}

void InputManager::UpdatePrevInput()
{
	memcpy(mPrevKeyboardState, mKeyboardState, mKeyLength);
	mPrevMouseState = mMouseState;

	mLastKey = SDL_SCANCODE_UNKNOWN;
}