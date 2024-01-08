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
  MouseAndKeys input; // Input
};

typedef std::shared_ptr<GameData> GameDataRef;

// Singleton Game class used for managing, updating and rendering the games states 
class Game : public Singleton<Game>
{
public:
	Game() {}
	// automatically release on deconstruct
	~Game() {
		Release();
	}
	void Update(float dTime);
	void Render(float dTime);
	void Initialise();
	void Release();
	LRESULT WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
private:

	// Reference to GameData.
	GameDataRef _data = std::make_shared<GameData>();
};

#endif
