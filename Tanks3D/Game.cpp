#include "WindowUtils.h"
#include "D3D.h"
#include "Game.h"

#include "SplashState.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Game::Initialise()
{
	// Adds the SplashState to the StateMachine
	_data->machine.AddState(StateRef(std::make_unique<SplashState>(_data)));
}

void Game::Release()
{
	// releases the state machine
	_data->machine.Release();
}

void Game::Update(float dTime)
{
	_data->machine.ProcessStateChanges();

	// updates active state in the state machine
	_data->machine.GetActiveState()->Update(dTime);
}

void Game::Render(float dTime)
{
	// renders active state in the state machine
	_data->machine.GetActiveState()->Render(dTime);
}

LRESULT Game::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// runs the win32 message handler on the active state
	return _data->machine.GetActiveState()->WindowsMssgHandler(hwnd, msg, wParam, lParam);
}

