#pragma once

#include "Game.h"
#include "Sprite.h"
#include "State.h"

#include "GameObject.h"

// Inherits the State class from 'State.h'
class SplashState : public State
{
public:
	SplashState(GameDataRef data);

	void Initialise() override;
	void Update(float dTime) override;
	void Render(float dTime) override;
	LRESULT WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void Release() override;

private:
	// Reference to GameData.
	GameDataRef _data;

	// Length of time the splash screen is shown on screen
	float mShowtime = 3.0f;
	float mElapsedTime = 0;

	DirectX::SpriteBatch* mBatch = nullptr;

	GameObject2D mLogo;
	DirectX::SimpleMath::Vector4 mLogoColor;
	float mLogoAlpha = 0;
};