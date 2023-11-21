#pragma once

#include "State.h"
#include "Game.h"

#include <future>

#include "GameObject.h"
#include "Input.h"

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

	std::vector<GameObject*> mGameObjects;
	enum Modelid { FLOOR, BACK_WALL, LEFT_WALL, TANK, TOTAL = 4 };

private: // Input
	MouseAndKeys input;

private:
	// Reference to GameData.
	GameDataRef _data;

	struct LoadData
	{
		std::future<void> loader;
		int totalToLoad = 0;
		int loadedSoFar = 0;
		bool running = false;
	};
	LoadData mLoadData;
	float gAngle = 0;
	DirectX::SpriteBatch* pFontBatch = nullptr;
	DirectX::SpriteFont* pFont = nullptr;

	void Load();
	void RenderLoad(float dTime);
};