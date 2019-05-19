#include "Graphics.h"

Graphics* Graphics::sInstance = nullptr;
bool Graphics::sInitialized = false;

Graphics* Graphics::Instance()
{
	if (sInstance == nullptr)
		sInstance = new Graphics();

	return sInstance;
}

void Graphics::Release()
{
	delete sInstance;
	sInstance = nullptr;

	sInitialized = false;
}

bool Graphics::Initialized()
{
	return sInitialized;
}

Graphics::Graphics()
{
	sInitialized = Init();
}

Graphics::~Graphics()
{
	SDL_DestroyWindow(mWindow);
	mWindow = nullptr;

	SDL_DestroyRenderer(mRenderer);
	mRenderer = nullptr;

	delete mCamera;
	mCamera = nullptr;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

bool Graphics::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "SDL Initialization Error: " << SDL_GetError() << std::endl;
		return false;
	}

	mWindow = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (mWindow == nullptr)
	{
		std::cout << "Window Creation Error: " << SDL_GetError() << std::endl;
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	
	if (mRenderer == nullptr)
	{
		std::cout << "Renderer Creation Error: " << SDL_GetError() << std::endl;
		return false;
	}

	SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);

	int flags = IMG_INIT_PNG;
	if (!(IMG_Init(flags) & flags))
	{
		std::cout << "IMG Initialization Error: " << IMG_GetError() << std::endl;
		return false;
	}

	if (TTF_Init() < 0)
	{
		std::cout << "TTF Initialization Error: " << TTF_GetError() << std::endl;
		return false;
	}

	mCamera = new SDL_Rect{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	return true;
}

SDL_Texture* Graphics::LoadTexture(std::string path)
{
	SDL_Texture* tex = nullptr;

	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == nullptr)
	{
		std::cout << "Image Load Error: Path " << path.c_str() << " - Error " << IMG_GetError() << std::endl;
		return tex;
	}

	tex = SDL_CreateTextureFromSurface(mRenderer, surface);
	if (tex == nullptr)
	{
		std::cout << "Create Texture Error: " << SDL_GetError() << std::endl;
		return tex;
	}

	SDL_FreeSurface(surface);

	return tex;
}

SDL_Texture* Graphics::CreateTextTexture(TTF_Font* font, std::string text, SDL_Color color)
{
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
	if (surface == nullptr)
	{
		std::cout << "Text Render Error: " << TTF_GetError() << std::endl;
		return nullptr;
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(mRenderer, surface);
	if (tex == nullptr)
	{
		std::cout << "Text Texture Creation Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	SDL_FreeSurface(surface);
	
	return tex;
}

void Graphics::ClearBackBuffer()
{
	SDL_RenderClear(mRenderer);
}

void Graphics::DrawTexture(SDL_Texture* tex, SDL_Rect* clip, SDL_Rect* rend, float angle, SDL_RendererFlip flip, bool ignoreCamera)
{
	if (!ignoreCamera)
	{
		Vector2 size = RotateVector(Vector2(rend->w, rend->h), angle);
		size.x = abs(size.x);
		size.y = abs(size.y);

		if (rend->x + size.x < mCamera->x) //Left of camera
			return;
		else if (rend->x > mCamera->x + mCamera->w) //Right of camera
			return;
		else if (rend->y + size.y < mCamera->y) //Above camera
			return;
		else if (rend->y > mCamera->y + mCamera->h) //Below camera
			return;

		rend->x -= mCamera->x + mOffset.x;
		rend->y -= mCamera->y + mOffset.y;
	}

	SDL_RenderCopyEx(mRenderer, tex, clip, rend, angle, NULL, flip);
}

void Graphics::SetLimit(Vector2 vec)
{
	mLimit = vec;
}

void Graphics::Camera(Vector2 vec)
{
	mCamera->x = vec.x - mCamera->w * 0.5f;
	mCamera->y = vec.y - mCamera->h * 0.5f;

	if (mCamera->x < 0.0f)
		mCamera->x = 0.0f;
	else if (mCamera->x + mCamera->w > mLimit.x)
		mCamera->x = mLimit.x - mCamera->w;

	if (mCamera->y <= 0)
		mCamera->y = 0.0f;
	else if (mCamera->y + mCamera->h > mLimit.y)
		mCamera->y = mLimit.y - mCamera->h;
}

void Graphics::CameraShake()
{
	mOffset.x += rand() % 4 + 1;
	mOffset.y += rand() % 4 + 1;
}

void Graphics::Render()
{
	SDL_RenderPresent(mRenderer);
	mOffset = VEC2_ZERO;
}