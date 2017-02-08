#include "Gameplay.h"
#include "Texture.h"
#include "Game.h"
#include <time.h>
#include <iostream>

Gameplay::Gameplay(Game* game)
    : GameState(game)
    , mPlayer(NULL)
    , mMissiles()
    , mShuttleTex(NULL)
    , mShotTex(NULL)
    , mIsActive(false)

	//: mShuttleTex(NULL)
	//, mShotTex(NULL)
	, mDonutTex(NULL)
	, mCokeTex(NULL)
	, mAlienTex(NULL)
	, mBottomTex(NULL)
	, mTopTex(NULL)
	//, mAlienShotTex(NULL)
	, mExplosionTex(NULL)
	//, mPlayer(NULL)
	, mExplosion()
	, mEnemy()
	//, mMissiles()
	//, mLaser()
	, mNextSpawnTime(0.0f)
	, mGodMode(false)
	, mScore(0)
	, mBomb(false)
	, mHeal(false)
	, mBombThreshold(0)
	, mHealThreshold(0)
{
}

Gameplay::~Gameplay()
{
}

bool Gameplay::Initialize()
{
	std::cout << "Initializing game" << std::endl;

	// set some window properties
	System::SetWindowSize(480, 640);
	System::SetWindowTitle("Broccoli Defender: Rise of the blood sugar!");

	// get renderer
	SDL_Renderer* renderer = System::GetRenderer();

	//
	// load all textures
	//
	mTopTex = Texture::Load("media/rsz_tablecloth2.png", renderer);
	if (!mTopTex) {
		std::cerr << "*** Failed to load grass texture" << std::endl;
		return false;
	}
	mBottomTex = Texture::Load("media/rsz_plate.png", renderer);
	if (!mBottomTex) {
		std::cerr << "*** Failed to load grass texture" << std::endl;
		return false;
	}

	mShuttleTex = Texture::Load("media/rsz_broccoliman.png", renderer);
	if (!mShuttleTex) {
		std::cerr << "*** Failed to load shuttle texture" << std::endl;
		return false;
	}
	mShotTex = Texture::Load("media/rsz_pea.png", renderer);
	if (!mShotTex) {
		std::cerr << "*** Failed to load shot texture" << std::endl;
		return false;
	}
	/*mAlienShotTex = Texture::Load("media/shot2.png", renderer);
	if (!mAlienShotTex) {
	std::cerr << "*** Failed to load shot texture" << std::endl;
	return false;
	}*/
	mAlienTex = Texture::Load("media/rsz_burger.png", renderer);
	if (!mAlienTex) {
		std::cerr << "*** Failed to load alien texture" << std::endl;
		return false;
	}
	mDonutTex = Texture::Load("media/rsz_donut.png", renderer);
	if (!mDonutTex) {
		std::cerr << "*** Failed to load alien texture" << std::endl;
		return false;
	}
	mCokeTex = Texture::Load("media/rsz_coke.png", renderer);
	if (!mCokeTex) {
		std::cerr << "*** Failed to load alien texture" << std::endl;
		return false;
	}
	mExplosionTex = Texture::Load("media/explosion.tga", renderer);
	if (!mExplosionTex) {
		std::cerr << "*** Failed to load explosion texture" << std::endl;
		return false;
	}

	//
	// spawn player
	//

	Vec2 spawnPos;
	spawnPos.x = 0.5f * System::GetWindowWidth();
	spawnPos.y = 0.75f * System::GetWindowHeight();

	mPlayer = new Player(spawnPos, mShuttleTex, 3);

	mPlayer->SetSpeed(150.0f);


	//spawn alien
	srand((unsigned)time(NULL));
	Vec2 alienSpawn;


	alienSpawn.x = ((0.1f * (rand() % 10)) * System::GetWindowWidth()) + 1;
	alienSpawn.y = 0.01f * System::GetWindowHeight();
	Enemy* enemy = new Enemy(alienSpawn, mAlienTex, 2, true, false, false);
	enemy->SetVelocity(0.0f, 100.0f);
	mEnemy.push_back(enemy);

	mNextSpawnTime = System::GetTime() + 1.0f;
	mBombThreshold = 10000;
	mHealThreshold = 5000;
	mGodMode = false;
	mHeal = false;
	mBomb = false;

	return true;
}

void Gameplay::Shutdown()
{
    ClearLevel();

    // destroy all textures
	Texture::Destroy(mShuttleTex);
	Texture::Destroy(mShotTex);
	Texture::Destroy(mAlienTex);
	Texture::Destroy(mAlienShotTex);
	Texture::Destroy(mExplosionTex);
	Texture::Destroy(mTopTex);
	Texture::Destroy(mBottomTex);
	Texture::Destroy(mDonutTex);
	Texture::Destroy(mCokeTex);
}

void Gameplay::LoadLevel()
{
    ClearLevel();

    //
    // spawn player
    //

    Vec2 spawnPos;
    spawnPos.x = 0.5f * System::GetWindowWidth();
    spawnPos.y = 0.75f * System::GetWindowHeight();

	mPlayer = new Player(spawnPos, mShuttleTex, 3);
    mPlayer->SetSpeed(150.0f);

	//spawn alien
	srand((unsigned)time(NULL));
	Vec2 alienSpawn;

	alienSpawn.x = ((0.1f * (rand() % 10)) * System::GetWindowWidth()) + 1;
	alienSpawn.y = 0.01f * System::GetWindowHeight();
	Enemy* enemy = new Enemy(alienSpawn, mAlienTex, 2, true, false, false);
	enemy->SetVelocity(0.0f, 100.0f);
	mEnemy.push_back(enemy);

	mNextSpawnTime = System::GetTime() + 1.0f;
	mBombThreshold = 10000;
	mHealThreshold = 5000;
	mGodMode = false;
	mHeal = false;
	mBomb = false;

}

void Gameplay::ClearLevel()
{
    delete mPlayer;
    mPlayer = NULL;

    for (auto it = mMissiles.begin(); it != mMissiles.end(); it++) {
        Missile* m = *it;
        delete m;
    }
    mMissiles.clear();

	mEnemy.clear();
	//delete enemy missiles
	for (unsigned i = 0; i < mLaser.size(); i++) {
		delete mLaser[i];
	}
	mExplosion.clear();  
}

void Gameplay::Update(float dt)
{
	// get world bounds
	float worldLeft = WorldLeft();
	float worldRight = WorldRight();
	float worldTop = WorldTop();
	float worldBottom = WorldBottom();

	// update player
	if (mPlayer != NULL) {
		mPlayer->Update(dt);

		// keep the player within world bounds
		if (mPlayer->Left() < worldLeft) {
			mPlayer->SetLeft(worldLeft);

		}
		else if (mPlayer->Right() > worldRight) {
			mPlayer->SetRight(worldRight);

		}
		if (mPlayer->Top() < worldTop) {
			mPlayer->SetTop(worldTop);

		}
		else if (mPlayer->Bottom() > worldBottom) {
			mPlayer->SetBottom(worldBottom);

		}


	}




	// update missiles

	for (unsigned i = 0; i < mMissiles.size(); ) {
		Missile* m = mMissiles[i];


		m->Update(dt);

		float threshold = m->Height() / 2;


		// remove the missile if it went off screen
		if (m->Left() > worldRight || m->Right() < worldLeft || m->Top() > worldBottom || m->Bottom() < worldTop) {
			// missile is out of world bounds: remove it
			delete m;
			mMissiles[i] = mMissiles.back();
			mMissiles.pop_back();

		}
		//If the player missile hit the enemy, destroy both and move to the next
		else {
			for (unsigned j = 0; j < mEnemy.size();) {
				Enemy* mAlien = mEnemy[j];
				if (mAlien != NULL && Distance(m->Center(), mAlien->Center()) < threshold + 25) {
					Explosion* e = new Explosion(Vec2(m->Center().x, m->Top()), mExplosionTex);
					mExplosion.push_back(e);

					delete m;
					mMissiles[i] = mMissiles.back();
					mMissiles.pop_back();
					int hp = mAlien->Hp();
					hp--;
					mAlien->SetHp(hp);
					if (mAlien->Hp() == 0 && mAlien->IsBasic()) {
						mScore = mScore + 500;
						std::cout << mScore << std::endl;
						e = new Explosion(Vec2(mAlien->Center().x, mAlien->Top()), mExplosionTex, 1.5f, 0.85f);
						mExplosion.push_back(e);
						mAlien = NULL;
						mEnemy[j] = mEnemy.back();
						mEnemy.pop_back();
					}
					else if (mAlien->Hp() == 0 && mAlien->IsFast()) {
						mScore = mScore + 1000;
						std::cout << mScore << std::endl;
						e = new Explosion(Vec2(mAlien->Center().x, mAlien->Top()), mExplosionTex, 1.5f, 0.85f);
						mExplosion.push_back(e);
						mAlien = NULL;
						mEnemy[j] = mEnemy.back();
						mEnemy.pop_back();
					}
					else if (mAlien->Hp() == 0 && mAlien->IsBig()) {
						mScore = mScore + 2000;
						std::cout << mScore << std::endl;
						e = new Explosion(Vec2(mAlien->Center().x, mAlien->Top()), mExplosionTex, 1.5f, 0.85f);
						mExplosion.push_back(e);
						mAlien = NULL;
						mEnemy[j] = mEnemy.back();
						mEnemy.pop_back();
					}
				}
				else {
					++j;
				}
			}
			// missile is still within world bounds: keep it and move on to the next one


			++i;

		}
	}


	// update enemy

	for (unsigned i = 0; i < mEnemy.size();) {
		Enemy* mAlien = mEnemy[i];
		mAlien->Update(dt);
		float threshold = mAlien->Width() / 2;
		//If the player collides with an alien, remove the player
		if (mPlayer != NULL && mAlien->Bottom() >= (0.75f * System::GetWindowHeight())) {
			int hp = mPlayer->Hp();
			if (mGodMode == true) {
				hp = hp - 0;
			}
			else {
				hp--;
			}

			mPlayer->SetHp(hp);
			Explosion* e = new Explosion(Vec2(mAlien->Center().x, mAlien->Top()), mExplosionTex, 1.5f, 0.85f);
			mExplosion.push_back(e);
			delete mAlien;
			mEnemy[i] = mEnemy.back();
			mEnemy.pop_back();
			if (mPlayer->Hp() == 0) {
				std::cout << "Game Over" << std::endl;
				e = new Explosion(Vec2(mPlayer->Center().x, mPlayer->Top()), mExplosionTex, 2.0f, 0.85f);
				mExplosion.push_back(e);
				mPlayer = NULL;
			}


		}
		// remove the enemy if it went off screen
		else if (mAlien->Left() < worldLeft || mAlien->Right() > worldRight || mAlien->Top() > worldBottom || mAlien->Bottom() < worldTop) {
			// enemy is out of world bounds: remove it
			delete mAlien;
			mEnemy[i] = mEnemy.back();
			mEnemy.pop_back();
		}
		else {
			// enemy is still within world bounds: keep it and move on to the next one
			++i;
		}
	}
	// iterate and play out the explosion animation
	for (unsigned i = 0; i < mExplosion.size(); ) {
		Explosion* e = mExplosion[i];
		e->Update(dt);


		// remove the explosion if it is finished
		if (e->IsFinished()) {
			delete e;
			mExplosion[i] = mExplosion.back();
			mExplosion.pop_back();
		}
		else {
			++i;
		}
	}
	if (mScore >= mBombThreshold) {
		mBomb = true;
		std::cout << "Bomb is available" << std::endl;
		mBombThreshold += 10000;
	}
	if (mScore >= mHealThreshold) {
		mHeal = true;
		std::cout << "Heal is available" << std::endl;
		mHealThreshold += 5000;
	}

	// get current time
	float t = System::GetTime();
	//spawn new enemies
	// is it time to spawn next effect?

	if (t >= mNextSpawnTime) {
		srand((unsigned)time(NULL));
		Vec2 alienSpawn;
		alienSpawn.x = ((0.1f * (rand() % 10)) * System::GetWindowWidth()) + 1;
		alienSpawn.y = 0.01f * System::GetWindowHeight();
		int type = rand() % 3 + 1;
		if (type == 1 && mScore >= 0) {
			Enemy* enemy = new Enemy(alienSpawn, mAlienTex, 2, true, false, false);
			enemy->SetVelocity(0.0f, 100.0f);
			mEnemy.push_back(enemy);
		}
		else if (type == 2 && mScore >= 1000) {//mScore >= 10000 original value
			Enemy* enemy = new Enemy(alienSpawn, mDonutTex, 3, false, true, false);
			enemy->SetVelocity(0.0f, 150.0f);
			mEnemy.push_back(enemy);
		}
		else if (type == 3 && mScore >= 2000) { //mScore >= 20000 original value
			Enemy* enemy = new Enemy(alienSpawn, mCokeTex, 5, false, false, true);
			enemy->SetVelocity(0.0f, 50.0f);
			mEnemy.push_back(enemy);
		}

		mNextSpawnTime = t + rand() % 2 + 1;

	}

    mIsActive = true;
}

void Gameplay::Draw(SDL_Renderer* renderer)
{
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); //black bg
	SDL_SetRenderDrawColor(renderer, 100, 149, 237, 255);//blue bg
	SDL_RenderClear(renderer);

	SDL_Rect screenRectBottom;
	screenRectBottom.w = mBottomTex->GetWidth();
	screenRectBottom.h = mBottomTex->GetHeight();
	screenRectBottom.x = 0;
	screenRectBottom.y = 475;

	SDL_Rect screenRectTop;
	screenRectTop.w = mTopTex->GetWidth();
	screenRectTop.h = mTopTex->GetHeight();
	screenRectTop.x = 0;
	screenRectTop.y = 0;

	SDL_RenderCopy(renderer, mTopTex->GetSDLTexture(), NULL, &screenRectTop);
	SDL_RenderCopy(renderer, mBottomTex->GetSDLTexture(), NULL, &screenRectBottom);

	// draw missiles
	for (unsigned i = 0; i < mMissiles.size(); i++) {
		mMissiles[i]->Draw(renderer);
	}

	// draw player
	if (mPlayer != NULL) {
		mPlayer->Draw(renderer);
	}
	//draw explosions
	for (unsigned i = 0; i < mExplosion.size(); i++) {
		mExplosion[i]->Draw(renderer);
	}
	
	//draw aliens
	for (unsigned i = 0; i < mEnemy.size(); i++) {
		mEnemy[i]->Draw(renderer);
	}
}

void Gameplay::OnKeyDown(const SDL_KeyboardEvent& kbe)
{
    switch (kbe.keysym.sym) {
    case SDLK_ESCAPE:
        std::cout << "User pressed Escape" << std::endl;
        mGame->EnterMainMenu();
        break;

    case SDLK_SPACE:
        // fire a missile
        {
            Missile* missile = new Missile(mPlayer->Center(), mShotTex);
            missile->SetVelocity(0.0f, -400.0f);
            mMissiles.push_back(missile);
        }
        break;
    
	case SDLK_r:
	{
		//reset game, remove missiles and aliens and redraw the player at the starting position
		for (unsigned i = 0; i < mMissiles.size(); i++) {
			delete mMissiles[i];
		}
		mMissiles.clear();
		/*for (unsigned i = 0; i < mLaser.size(); i++) {
		delete mLaser[i];
		}
		mLaser.clear();*/
		for (unsigned i = 0; i < mEnemy.size(); i++) {
			delete mEnemy[i];
		}
		mEnemy.clear();
		mScore = 0;
		mGodMode = false;
		mHeal = false;
		mBomb = false;
		mHealThreshold = 5000;
		mBombThreshold = 10000;
		Vec2 spawnPos;
		spawnPos.x = 0.5f * System::GetWindowWidth();
		spawnPos.y = 0.75f * System::GetWindowHeight();
		mPlayer = new Player(spawnPos, mShuttleTex, 3);
		break;
	}
	case SDLK_g:
	{
		if (mGodMode == false) {
			mGodMode = true;
			std::cout << "God Mode on" << std::endl;
		}
		else if (mGodMode == true) {
			mGodMode = false;
			std::cout << "God Mode off" << std::endl;
		}
		break;
	}
	case SDLK_b:
	{
		if (mBomb == true) {
			mBomb = false;
			std::cout << "Bomb has been used" << std::endl;
			for (unsigned i = 0; i < mEnemy.size(); i++) {

				Enemy* mAlien = mEnemy[i];
				if (mAlien->IsBasic()) {
					mScore += 500;
					std::cout << mScore << std::endl;
					Explosion* e = new Explosion(Vec2(mAlien->Center().x, mAlien->Top()), mExplosionTex, 1.5f, 0.85f);
					mExplosion.push_back(e);
					mAlien = NULL;
					mEnemy[i] = mEnemy.back();
					mEnemy.pop_back();
				}
				else if (mEnemy[i]->IsFast()) {
					mScore += 1000;
					std::cout << mScore << std::endl;
					Explosion* e = new Explosion(Vec2(mAlien->Center().x, mAlien->Top()), mExplosionTex, 1.5f, 0.85f);
					mExplosion.push_back(e);
					mAlien = NULL;
					mEnemy[i] = mEnemy.back();
					mEnemy.pop_back();
				}
				else if (mEnemy[i]->IsBig()) {
					mScore += 2000;
					std::cout << mScore << std::endl;
					Explosion* e = new Explosion(Vec2(mAlien->Center().x, mAlien->Top()), mExplosionTex, 1.5f, 0.85f);
					mExplosion.push_back(e);
					mAlien = NULL;
					mEnemy[i] = mEnemy.back();
					mEnemy.pop_back();
				}

			}

		}
		break;
	}

	case SDLK_h:
	{
		if (mHeal == true && mPlayer != NULL) {
			mHeal = false;
			mPlayer->SetHp(3);
			std::cout << mPlayer->Hp() << std::endl;
			std::cout << "All health restored" << std::endl;
		}
		break;
	}
}
}
