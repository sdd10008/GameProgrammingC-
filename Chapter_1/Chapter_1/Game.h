#pragma once
#include "SDL2\SDL.h"

struct Vector2 {
	float x;
	float y;
};

class Game
{
public:
	Game();
	//　初期化
	bool Initialize();

	// メインループ
	void RunLoop();

	// 終了
	void ShutDown();

private:
	// ヘルパー関数
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	// 変数
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	bool mIsRunning;
	Vector2 mPaddlePos;
	Vector2 mBallPos;
	Vector2 mBallVel;
	int mPaddleDir;
	Uint32 mTicksCount;

	// 定数
	const int PADDLE_HEIGHT = 50;
	const int WINDOW_WIDTH = 1024;
	const int WINDOW_HEIGHT = 768;
	const int THICKNESS = 15;
	const float PADDLE_SPEED = 400.0f;
};