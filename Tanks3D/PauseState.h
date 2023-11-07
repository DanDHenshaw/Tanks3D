#pragma once

#include "Game.h"
#include "Sprite.h"
#include "SpriteFont.h"
#include "State.h"

// Inherits the State class from 'State.h'
class PauseState : public State
{
public:
	PauseState(GameDataRef data);

	void Initialise() override;
	void Update(float dTime) override;
	void Render(float dTime) override;
	LRESULT WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void Release() override;

private:
	// Reference to GameData.
	GameDataRef _data;

	DirectX::SpriteBatch* batch = nullptr;

	DirectX::SpriteFont *mPauseLabel = nullptr;

	Sprite test;
};