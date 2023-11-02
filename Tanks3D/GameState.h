#pragma once

#include "State.h"
#include "Game.h"

// Inherits the State class from 'State.h'
class GameState : public State
{
public:
	GameState(GameDataRef data);

	void Initialise() override;
	void Update(float dTime) override;
	void Render(float dTime) override;
	LRESULT WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) override;
	void Release() override;

	void Pause() override;
	void Resume() override;
private:
	// Reference to GameData.
	GameDataRef _data;

	float gAngle = 0;

	Model mBox, mBRot, mBScale, mBScroll, mBCross, mQuad, mBWin, mBall;
};