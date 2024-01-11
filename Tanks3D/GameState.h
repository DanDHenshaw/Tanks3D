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
	enum Modelid { FLOOR, PLAYER1, BULLET1, PLAYER2, BULLET2, TOTAL = 5 };

public:
  // Camera
  float gAngle = 0;

  const DirectX::SimpleMath::Vector3 mDefCamPos = DirectX::SimpleMath::Vector3(0, 4, -10);
  DirectX::SimpleMath::Vector3 mCamPos = DirectX::SimpleMath::Vector3(0, 4, -10);

private:
  int p1Score = 0, p2Score = 0;
  int maxScore = 5;
  bool playerScored = false;
  float roundCooldown = 2.0f;
  float elapsedRoundCooldown = 0.0f;

  DirectX::SimpleMath::Vector3 p1StartPos;
  DirectX::SimpleMath::Vector3 p2StartPos;

  DirectX::SimpleMath::Vector3 p1StartRot = DirectX::SimpleMath::Vector3(PI / 2, PI / 2, 0);
  DirectX::SimpleMath::Vector3 p2StartRot = DirectX::SimpleMath::Vector3(PI / 2, -PI / 2, 0);

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
	DirectX::SpriteBatch* pFontBatch = nullptr;
	DirectX::SpriteFont* pFont = nullptr;

	void Load();
	void RenderLoad(float dTime);
};