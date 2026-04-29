#include <vector>
#include <algorithm>
#include "Game.h"
#include "Actor.h"
#include <SDL2\SDL.h>
#include <SDL2\SDL_Image.h>

Game::Game() :mUpdatingActors(false),
mWindow(nullptr),
mRenderer(nullptr),
mIsRunning(true),
mTicksCount(0)
{
}

bool Game::Initialize() {
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0) {
		SDL_Log("SDL Initilize Failed:%s", SDL_GetError());
		return false;
	}

	mWindow = SDL_CreateWindow("Game Programming in C++ Pong Game", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

	if (!mWindow) {
		SDL_Log("Create Window Failed:%s", SDL_GetError());
		return false;
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!mRenderer) {
		SDL_Log("Create Renderer Failed: %s", SDL_GetError());
		return false;
	}

	return true;
}

void Game::ShutDown() {
	while (!mActors.empty()) {
		delete mActors.back();
	}

	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::RunLoop() {
	while (mIsRunning) {
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	// イベントをポーリング
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			// 画面を閉じたら終了
		case SDL_QUIT:
			mIsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);

	// [Escape]を押下したら終了
	if (state[SDL_SCANCODE_ESCAPE]) {
		mIsRunning = false;
	}
}

void Game::UpdateGame() {
	// フレーム制限(最低16msを固定)
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// 前回フレームとの差の時刻を取得
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// 時刻上限を設定
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	// SDL 初期化してからの時刻を取得
	mTicksCount = SDL_GetTicks();

	mUpdatingActors = true;

	for (auto actor : mActors) {
		actor->Update(deltaTime);
	}
	mUpdatingActors = false;

	for (auto pending : mPendingActors) {
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	std::vector<Actor*> deadActors;

	for (auto actor : mActors) {
		if (actor->getState() == Actor::EDead) {
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors) {
		delete actor;
	}
}

void Game::GenerateOutput() {
	// 塗りつぶす色を設定
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);

	// 前の画面が残らないように塗りつぶす色でクリア
	SDL_RenderClear(mRenderer);

	// バックバッファとフロントバッファを入れ替える（ダブルバッファ）
	SDL_RenderPresent(mRenderer);
}

void Game::AddActor(Actor* actor) {
	// queue actor in temporary list during updated
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else {
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor) {
	// remove the specified actor from the list
	mActors.erase(std::remove(mActors.begin(), mActors.end(), actor), mActors.end());
	mPendingActors.erase(std::remove(mPendingActors.begin(), mPendingActors.end(), actor), mPendingActors.end());
}