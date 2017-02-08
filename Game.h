#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>
#include <vector>

#include "System.h"
#include "Player.h"
#include "Missile.h"
#include "Enemy.h"
#include "Explosion.h"

#include "Gameplay.h"
#include "MainMenu.h"

class Game
{
    Gameplay*               mGameplayState;
    MainMenu*               mMainMenuState;

    GameState*              mCurrentState;
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
                            Game();
                            ~Game();

    bool				    Initialize();
    void				    Shutdown();

    void				    Update(float dt);
    void				    Draw(SDL_Renderer* renderer);
    
    void				    OnWindowResized(int w, int h);
    void				    OnKeyDown(const SDL_KeyboardEvent& kbe);
    void				    OnKeyUp(const SDL_KeyboardEvent& kbe);
    void				    OnMouseDown(const SDL_MouseButtonEvent& mbe);
    void				    OnMouseUp(const SDL_MouseButtonEvent& mbe);
    void				    OnMouseMotion(const SDL_MouseMotionEvent& mme);

    Gameplay*               GetGameplayState() const    { return mGameplayState; }
    MainMenu*               GetMainMenuState() const    { return mMainMenuState; }

    void                    EnterMainMenu();
    void                    EnterGameplay();

	float                   WorldLeft() const { return 0.0f; }
	float                   WorldRight() const { return (float)System::GetWindowWidth(); }
	float                   WorldTop() const { return 0.0f; }
	float                   WorldBottom() const { return (float)System::GetWindowHeight(); }
};

#endif
