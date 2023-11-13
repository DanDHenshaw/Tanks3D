#include "WindowUtils.h"
#include "D3D.h"
#include "Game.h"

#include "SplashState.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

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
	D3D& d3d = WinUtil::Get().GetD3D();
	d3d.BeginRender(Colours::Black);

	_data->machine.GetActiveState()->Render(dTime);

	d3d.EndRender();
}

LRESULT Game::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return _data->machine.GetActiveState()->WindowsMssgHandler(hwnd, msg, wParam, lParam);
}

