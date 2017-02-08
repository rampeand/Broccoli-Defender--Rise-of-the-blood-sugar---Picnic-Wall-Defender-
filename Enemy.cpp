#include "Enemy.h"
#include "System.h"


Enemy::Enemy(const Vec2& pos, const Texture* tex, int hp, const bool isBasic, const bool isFast, const bool isBig)
: mTex(tex)
, mCenter(pos)
, mVelocity()
, mHealth(hp)
, mIsBasic(isBasic)
, mIsFast(isFast)
, mIsBig(isBig)
{
	
}


Enemy::~Enemy()
{
}

void Enemy::Update(float dt)
{
	mCenter.x += mVelocity.x * dt;
	mCenter.y += mVelocity.y * dt;
}

void Enemy::Draw(SDL_Renderer* renderer) const
{
	// check if we have a valid texture
	if (mTex) {

		// compute alien on screen
		SDL_Rect screenRect;
		screenRect.w = mTex->GetWidth();
		screenRect.h = mTex->GetHeight();
		screenRect.x = (int)(mCenter.x - mTex->GetWidth() / 2);
		screenRect.y = (int)(mCenter.y - mTex->GetHeight() / 2);

		// draw textured alien
		SDL_RenderCopy(renderer, mTex->GetSDLTexture(), NULL, &screenRect);

	}
	else {
		// draw a placeholder
		SDL_Rect screenRect;
		screenRect.w = 64;
		screenRect.h = 64;
		screenRect.x = (int)(mCenter.x - screenRect.w / 2);
		screenRect.y = (int)(mCenter.y - screenRect.h / 2);

		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		SDL_RenderFillRect(renderer, &screenRect);
	}
}