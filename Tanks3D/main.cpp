#include <iostream>

#include "WindowUtils.h"
#include "D3D.h"
#include "Game.h"

int w(1024), h(768);
std::string name = "Tanks3D";

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//do something game specific here
	switch (msg)
	{
		// Respond to a keyboard event.
	case WM_CHAR:
	case WM_INPUT:
		return Game::Get().WindowsMssgHandler(hwnd, msg, wParam, lParam);
	}
	return WinUtil::DefaultMssgHandler(hwnd, msg, wParam, lParam);
}

void renderer(Game* game, float* dTime)
{
	bool canRender;

	while (WinUtil::Get().BeginLoop(canRender))
	{
		if (canRender && *dTime > 0)
		{
			game->Render(*dTime);
		}
	}
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

	bool canUpdate;
	float dTime = 0;

	bool canRender = false;

	while (WinUtil::Get().BeginLoop(canUpdate))
	{
		if (canUpdate && dTime > 0)
		{
			game.Update(dTime);
			game.Render(dTime);
		}
		dTime = WinUtil::Get().EndLoop(canUpdate);
	}

	game.Release();
	d3d.ReleaseD3D(true);

	return 0;
}