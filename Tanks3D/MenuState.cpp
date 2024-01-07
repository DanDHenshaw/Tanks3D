#include "MenuState.h"

#include <iomanip>

#include "CommonStates.h"
#include "GameState.h"
#include "WindowUtils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

MenuState::MenuState(GameDataRef data)
	: _data(data), pLogo(WinUtil::Get().GetD3D()), pPlay(WinUtil::Get().GetD3D()), pExit(WinUtil::Get().GetD3D())
{
}

void MenuState::Initialise()
{
  D3D& d3d = WinUtil::Get().GetD3D();

  // Initialise a new sprite batch
  mBatch = new SpriteBatch(&d3d.GetDeviceCtx());

  int w, h;
  WinUtil::Get().GetClientExtents(w, h);

  pLogo.Initialise("logo.dds", Vector2(w / 2, h / 4));
  pPlay.sprite.Initialise("menu/play.dds", Vector2(w / 2, h / 2));
  pExit.sprite.Initialise("menu/quit.dds", Vector2(w / 2, h / 1.5f));
}

void MenuState::Update(float dTime)
{
	//_data->machine.AddState(StateRef(std::make_unique<GameState>(_data)), true);
}

void MenuState::Render(float dTime)
{
  D3D& d3d = WinUtil::Get().GetD3D();
  d3d.BeginRender(Colours::Black);

  // Starts the sprite batch
  CommonStates dxstate(&d3d.GetDevice());
  mBatch->Begin(SpriteSortMode_Deferred, dxstate.NonPremultiplied(), &d3d.GetWrapSampler());

  pLogo.Draw(*mBatch);
  pPlay.sprite.Draw(*mBatch);
  pExit.sprite.Draw(*mBatch);

  mBatch->End();

  d3d.EndRender();
}

LRESULT MenuState::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  switch (msg)
  {
    // Respond to a keyboard event.
  case WM_CHAR:
    switch (wParam)
    {
      // If 'Q' is pressed then close the game
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

  // Deletes the allocated memory for the SpriteBatch
  delete mBatch;
  mBatch = nullptr;
}