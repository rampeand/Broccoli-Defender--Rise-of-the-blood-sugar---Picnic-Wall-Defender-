#ifndef GAMEPLAY_H_
#define GAMEPLAY_H_

#include "GameState.h"
#include "System.h"
#include "Player.h"
#include "Missile.h"
#include "Enemy.h"
#include "Explosion.h"

#include <vector>

class Gameplay : public GameState
{
    bool                    mIsActive;

	Texture*				mTopTex;
	Texture*				mBottomTex;
	Texture*                mShuttleTex;
	Texture*                mShotTex;
	Texture*                mAlienTex;
	Texture*                mDonutTex;
	Texture*                mCokeTex;
	Texture*                mAlienShotTex;
	Texture*                mExplosionTex;
	Player*				    mPlayer;
	std::vector<Explosion*> mExplosion;
	std::vector<Enemy*>     mEnemy;
	std::vector<Missile*>   mMissiles;
	std::vector<Missile*>   mLaser;
	float                   mNextSpawnTime;
	bool                    mGodMode;
	int                     mScore;
	bool                    mBomb;
	bool                    mHeal;
	int                     mBombThreshold;
	int                     mHealThreshold;

public:
                            Gameplay(Game* game);
                            ~Gameplay() override;

    bool                    IsActive() const        { return mIsActive; }

    bool                    Initialize() override;
    void                    Shutdown() override;

    void                    LoadLevel();
    void                    ClearLevel();

    void                    Update(float dt) override;
    void                    Draw(SDL_Renderer* renderer) override;

    void		            OnKeyDown(const SDL_KeyboardEvent& kbe) override;

    float                   WorldLeft() const       { return 0.0f; }
    float                   WorldRight() const      { return (float)System::GetWindowWidth(); }
    float                   WorldTop() const        { return 0.0f; }
    float                   WorldBottom() const     { return (float)System::GetWindowHeight(); }
};

#endif
