#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "MathHelper.h"
#include <iostream>
#include <string>

class Graphics
{
public:
	static const int SCREEN_WIDTH = 768;
	static const int SCREEN_HEIGHT = 512;
	const char* WINDOW_TITLE = "Zombies";

private:
	static Graphics* sInstance;
	static bool sInitialized;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;

	SDL_Rect* mCamera;
	Vector2 mLimit;
	Vector2 mOffset;

	Graphics();
	~Graphics();

	bool Init();

public:
	static Graphics* Instance();
	static void Release();
	static bool Initialized();

	SDL_Texture* LoadTexture(std::string path);
	SDL_Texture* CreateTextTexture(TTF_Font* font, std::string text, SDL_Color color);

	void ClearBackBuffer();

	void DrawTexture(SDL_Texture* tex, SDL_Rect* clip = nullptr, SDL_Rect* rend = nullptr, float angle = 0.0f, SDL_RendererFlip flip = SDL_FLIP_NONE, bool ignoreCamera = false);

	void SetLimit(Vector2 vec);
	void Camera(Vector2 vec);
	SDL_Rect* Camera() const { return mCamera; }
	void CameraShake();

	void Render();
};

#endif