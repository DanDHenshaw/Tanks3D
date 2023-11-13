#pragma once

#include "Game.h"
#include "Sprite.h"
#include "SpriteFont.h"
#include "State.h"

// Inherits the State class from 'State.h'
class MenuState : public State
{
public:
	MenuState(GameDataRef data);

	void Initialise() override;
	void Update(float dTime) override;
	void Render(float dTime) override;
	LRESULT WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void Release() override;

private:
	// Reference to GameData.
	GameDataRef _data;
};