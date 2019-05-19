#ifndef ANIMATEDTEXTURE_H
#define ANIMATEDTEXTURE_H

#include "Timer.h"
#include "Texture.h"

class AnimatedTexture : public Texture
{
public:
	enum WRAP_MODE { once = 0, loop = 1 };
	enum ANIM_DIR { horizontal = 0, vertical = 1 };

private:
	Timer* mTimer;

	int mStartX;
	int mStartY;

	float mAnimationTimer;
	float mAnimationSpeed;
	float mTimePerFrame;

	int mFrameCount;

	WRAP_MODE mWrapMode;
	ANIM_DIR mAnimationDirection;

	bool mAnimationDone;

public:
	AnimatedTexture(std::string filename, int x, int y, int w, int h, int frameCount, float animationSpeed, ANIM_DIR animationDirection);
	~AnimatedTexture();

	void WrapMode(WRAP_MODE mode);

	void ResetAnimation();

	bool IsAnimating();

	void Update();
};

#endif