#include "Texture.h"

Texture::Texture(std::string filename)
{
	mGraphics = Graphics::Instance();
	mTex = AssetManager::Instance()->GetTexture(filename);

	mText = "";

	SDL_QueryTexture(mTex, NULL, NULL, &mWidth, &mHeight);

	mClipped = false;

	mRenderRect.w = mWidth;
	mRenderRect.h = mHeight;

	mFlip = SDL_FLIP_NONE;

	mIgnoreCamera = false;

	mFlashRed = false;
	mFlashFrames = 0;

	mDim = false;
}

Texture::Texture(std::string filename, int x, int y, int w, int h)
{
	mGraphics = Graphics::Instance();
	mTex = AssetManager::Instance()->GetTexture(filename);

	mClipped = true;

	mText = "";

	mWidth = w;
	mHeight = h;

	mRenderRect.w = mWidth;
	mRenderRect.h = mHeight;

	mClipRect.x = x;
	mClipRect.y = y;
	mClipRect.w = mWidth;
	mClipRect.h = mHeight;

	mFlip = SDL_FLIP_NONE;

	mIgnoreCamera = false;

	mFlashRed = false;
	mFlashFrames = 0;
}

Texture::Texture(std::string text, std::string fontpath, int size, SDL_Color color)
{
	mGraphics = Graphics::Instance();
	mTex = AssetManager::Instance()->GetText(text, fontpath, size, color);

	mText = text;

	mClipped = false;

	SDL_QueryTexture(mTex, NULL, NULL, &mWidth, &mHeight);

	mRenderRect.w = mWidth;
	mRenderRect.h = mHeight;

	mFlip = SDL_FLIP_NONE;

	mIgnoreCamera = false;

	mFlashRed = false;
	mFlashFrames = 0;
}

Texture::~Texture()
{
	mTex = nullptr;
	mGraphics = nullptr;
}

Vector2 Texture::ScaledDimensions()
{
	Vector2 scaledDimensions = Scale();
	scaledDimensions.x *= mWidth;
	scaledDimensions.y *= mHeight;

	return scaledDimensions;
}

void Texture::Flip(SDL_RendererFlip flip)
{
	mFlip = flip;
}

void Texture::IgnoreCamera(bool ignore)
{
	mIgnoreCamera = ignore;
}

void Texture::Color(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(mTex, red, green, blue);
}

void Texture::FlashRed()
{
	mFlashRed = true;
}

void Texture::Dim()
{
	mDim = true;
}

void Texture::Render()
{
	Vector2 pos = Pos(world);
	Vector2 scale = Scale(world);

	mRenderRect.x = (int)(pos.x - mWidth * scale.x * 0.5f);
	mRenderRect.y = (int)(pos.y - mHeight *  scale.y * 0.5f);

	mRenderRect.w = (int)(mWidth * scale.x);
	mRenderRect.h = (int)(mHeight * scale.y);

	if (mFlashRed && mFlashFrames >= 0)
	{
		Color(255, 55, 55);
		mFlashFrames++;
	}
	else if (mDim)
	{
		Color(125, 125, 125);
		mDim = false;
	}
	else
	{
		Color(255, 255, 255);
		if (mFlashFrames < 0) mFlashFrames++;
	}

	mGraphics->DrawTexture(mTex, (mClipped)? &mClipRect : nullptr, &mRenderRect, Rotation(world), mFlip, mIgnoreCamera);

	if (mFlashRed && mFlashFrames >= 6)
	{
		Color(255, 255, 255);
		mFlashRed = false;
		mFlashFrames = -12;
	}
}