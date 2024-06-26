#include <iostream>

#include "WindowUtils.h"
#include "D3D.h"
#include "Game.h"

int w(1024), h(768);
std::string name = "Tanks3D";

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CHAR:
	case WM_INPUT:
		// Sends input to the game manager
		return Game::Get().WindowsMssgHandler(hwnd, msg, wParam, lParam);
	}
	return WinUtil::DefaultMssgHandler(hwnd, msg, wParam, lParam);
}

//main entry point for the game
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	if (!WinUtil::Get().InitMainWindow(w, h, hInstance, name, MainWndProc, true))
		assert(false);

	D3D d3d;
	if (!d3d.InitDirect3D())
		assert(false);
	WinUtil::Get().SetD3D(d3d);
	d3d.GetCache().SetAssetPath("data/");

	Game game;
	game.Initialise();

	bool canUpdateRender;
	float dTime = 0;
	while (WinUtil::Get().BeginLoop(canUpdateRender))
	{
		if (canUpdateRender && dTime > 0)
		{
			game.Update(dTime);
			game.Render(dTime);
		}
		dTime = WinUtil::Get().EndLoop(canUpdateRender);
	}

	game.Release();
	d3d.ReleaseD3D(true);

	return 0;
}