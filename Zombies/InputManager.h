#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL.h>
#include <string.h>
#include "MathHelper.h"

class InputManager
{
public:
	enum MOUSE_BUTTON {left = 0, right, middle, back, forward};

private:
	static InputManager* sInstance;

	Uint8* mPrevKeyboardState;
	const Uint8* mKeyboardState;
	int mKeyLength;

	Uint32 mPrevMouseState;
	Uint32 mMouseState;

	int mMouseXPos;
	int mMouseYPos;

	SDL_Scancode mLastKey;

	InputManager();
	~InputManager();
public:
	static InputManager* Instance();
	static void Release();

	bool KeyDown(SDL_Scancode scanCode);
	bool KeyPressed(SDL_Scancode scanCode);
	bool KeyReleased(SDL_Scancode scanCode);

	bool MouseButtonDown(MOUSE_BUTTON button);
	bool MouseButtonPressed(MOUSE_BUTTON button);
	bool MouseButtonReleased(MOUSE_BUTTON button);

	Vector2 MousePos();

	void LastKey(SDL_Scancode scanCode);
	SDL_Scancode LastKey();

	void Update();
	void UpdatePrevInput();
};

#endif