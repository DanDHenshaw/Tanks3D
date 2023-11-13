#include "MenuState.h"

#include <iomanip>

#include "CommonStates.h"
#include "GameState.h"
#include "WindowUtils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

MenuState::MenuState(GameDataRef data)
	: _data(data)
{
}

void MenuState::Initialise()
{
	
}

void MenuState::Update(float dTime)
{
	_data->machine.AddState(StateRef(std::make_unique<GameState>(_data)), true);
}

void MenuState::Render(float dTime)
{

}

LRESULT MenuState::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// Respond to a keyboard event.
	case WM_CHAR:
		switch (wParam)
		{
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			return 0;
		}
	}

	return WinUtil::Get().DefaultMssgHandler(hwnd, msg, wParam, lParam);
}

void MenuState::Release()
{
	State::Release();
}