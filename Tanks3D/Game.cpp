#include "WindowUtils.h"
#include "D3D.h"
#include "Game.h"

#include "GameState.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Game::Initialise()
{
	_data->machine.AddState(StateRef(new GameState(_data)));
}

void Game::Release()
{
	_data->machine.GetActiveState()->Release();
}

void Game::Update(float dTime)
{
	_data->machine.ProcessStateChanges();

	_data->machine.GetActiveState()->Update(dTime);
}

void Game::Render(float dTime)
{
	_data->machine.GetActiveState()->Render(dTime);

}

LRESULT Game::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return _data->machine.GetActiveState()->WindowsMssgHandler(hwnd, msg, wParam, lParam);
}

