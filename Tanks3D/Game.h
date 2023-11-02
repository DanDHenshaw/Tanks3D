#ifndef GAME_H
#define GAME_H

#include "Mesh.h"
#include "Model.h"
#include "singleton.h"

#include "StateMachine.h"

/*
 Reference to all the games data.
	- machine is a reference to the StateMachine.
	- window is a reference to the RenderWindow.
	- assets is a reference to the AssetManager.
	- input is a reference to the InputManager.
 */
struct GameData
{
	StateMachine machine;
};

typedef std::shared_ptr<GameData> GameDataRef;

class Game : public Singleton<Game>
{
public:
	Game() {}
	~Game() {
		Release();
	}
	void Update(float dTime);
	void Render(float dTime);
	void Initialise();
	void Release();
	LRESULT WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	const DirectX::SimpleMath::Vector3 mDefCamPos = DirectX::SimpleMath::Vector3(0, 2, -5);
	DirectX::SimpleMath::Vector3 mCamPos = DirectX::SimpleMath::Vector3(0, 2, -5);
	Model mBox, mBRot, mBScale, mBScroll, mBCross, mQuad, mBWin, mBall;

private:

	// Reference to GameData.
	GameDataRef _data = std::make_shared<GameData>();

	float gAngle = 0;
};

#endif
