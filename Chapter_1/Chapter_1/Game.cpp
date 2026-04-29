#include "Game.h"
#include "SDL2\SDL.h"
#include <cmath>

Game::Game() : mPaddleDir(0),
mWindow(nullptr),
mRenderer(nullptr),
mIsRunning(true),
mPaddlePos(Vector2{ 0, 384 }),
mBallPos(Vector2{ 512,384 }),
mBallVel(Vector2{ -200.0f,235.0f }),
mTicksCount(0) {
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

	mPaddleDir = 0;

	// [W]を押下したらパドル上昇
	if (state[SDL_SCANCODE_W]) {
		mPaddleDir -= 1;
	}

	// [S]を押下したらパドルを下降
	if (state[SDL_SCANCODE_S]) {
		mPaddleDir += 1;
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

	// 移動量が入力されていた場合のパドル更新処理
	if (mPaddleDir != 0) {

		// 縦移動を更新
		mPaddlePos.y += mPaddleDir * PADDLE_SPEED * deltaTime;

		// 上部壁を通り抜けないように補正
		if (mPaddlePos.y < (PADDLE_HEIGHT / 2.0f + THICKNESS)) {
			mPaddlePos.y = PADDLE_HEIGHT / 2.0f + THICKNESS;
		}
		// 下部壁を通り抜けないように補正
		else if (mPaddlePos.y > (768.0f - PADDLE_HEIGHT / 2.0f - THICKNESS)) {
			mPaddlePos.y = 768.0f - PADDLE_HEIGHT / 2.0f - THICKNESS;
		}
	}

	// ボールの更新
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// 上下壁の衝突判定
	if (mBallPos.y <= THICKNESS && mBallVel.y < 0.0f ||
		mBallPos.y >= WINDOW_HEIGHT - THICKNESS && mBallVel.y >0.0f) {
		mBallVel.y *= -1;
	}

	// 右壁の衝突判定
	if (mBallPos.x >= WINDOW_WIDTH - THICKNESS && mBallVel.x > 0.0f) {
		mBallVel.x *= -1;
	}

	// パドルとボールの高さの絶対値の差を計算
	float diff = std::abs(mBallPos.y - mPaddlePos.y);

	// パドルとの衝突判定
	if (mBallPos.x <= 25.0f && mBallPos.x >= 20.0f && mBallVel.x < 0.0f && diff <= PADDLE_HEIGHT / 2.0f) {
		mBallVel.x *= -1;
	}

}

void Game::GenerateOutput() {
	// 塗りつぶす色を設定
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);

	// 前の画面が残らないように塗りつぶす色でクリア
	SDL_RenderClear(mRenderer);

	// オブジェクトの色を設定
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// オブジェクトのサイズ位置を設定
	SDL_Rect topWall{ 0 , 0 , WINDOW_WIDTH , THICKNESS };
	SDL_Rect bottomWall{ 0 , WINDOW_HEIGHT - THICKNESS , WINDOW_WIDTH , THICKNESS };
	SDL_Rect rightWall{ WINDOW_WIDTH - THICKNESS , 0 , THICKNESS , WINDOW_HEIGHT };
	SDL_Rect paddle{ static_cast<int>(mPaddlePos.x - THICKNESS / 2),static_cast<int>(mPaddlePos.y - PADDLE_HEIGHT / 2),THICKNESS,PADDLE_HEIGHT };
	SDL_Rect ball{ static_cast<int>(mBallPos.x - THICKNESS / 2),static_cast<int>(mBallPos.y - THICKNESS / 2),THICKNESS,THICKNESS };

	// 書き込み
	SDL_RenderFillRect(mRenderer, &topWall);
	SDL_RenderFillRect(mRenderer, &bottomWall);
	SDL_RenderFillRect(mRenderer, &rightWall);
	SDL_RenderFillRect(mRenderer, &paddle);
	SDL_RenderFillRect(mRenderer, &ball);

	// バックバッファとフロントバッファを入れ替える（ダブルバッファ）
	SDL_RenderPresent(mRenderer);
}