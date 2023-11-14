#include "WindowUtils.h"
#include "D3D.h"
#include "Game.h"

#include "SplashState.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

bool canUpdateRender;

void Game::Initialise()
{
	_data->machine.AddState(StateRef(std::make_unique<SplashState>(_data)));
}

void Game::Release()
{
	_data->machine.CloseStack();
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

