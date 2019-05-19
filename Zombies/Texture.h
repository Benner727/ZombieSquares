#ifndef TEXTURE_H
#define TEXTURE_H

#include "GameEntity.h"
#include "AssetManager.h"

class Texture : public GameEntity
{
protected:
	SDL_Texture* mTex;

	Graphics* mGraphics;

	std::string mText;

	int mWidth;
	int mHeight;

	bool mClipped;
	SDL_Rect mRenderRect;
	SDL_Rect mClipRect;

	SDL_RendererFlip mFlip;

	bool mIgnoreCamera;

	bool mFlashRed;
	int mFlashFrames;

	bool mDim;

public:
	Texture(std::string filename);
	Texture(std::string filename, int x, int y, int w, int h);
	Texture(std::string text, std::string fontpath, int size, SDL_Color);
	~Texture();

	Vector2 ScaledDimensions();

	std::string GetText() const { return mText; }

	void Flip(SDL_RendererFlip flip);

	void IgnoreCamera(bool ignore);

	void Color(Uint8 red, Uint8 green, Uint8 blue);
	void FlashRed();

	void Dim();

	virtual void Render();
};

#endif