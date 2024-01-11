#include "WinState.h"

#include <iomanip>

#include "CommonStates.h"
#include "GameState.h"
#include "WindowUtils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

WinState::WinState(GameDataRef data)
  : _data(data), pBackground(WinUtil::Get().GetD3D(), "game/ground/stony_dirt_path_diff_1k.dds"), pLogo(WinUtil::Get().GetD3D(), "logo.dds"), pPlay(WinUtil::Get().GetD3D(), "menu/play.dds"), pExit(WinUtil::Get().GetD3D(), "menu/quit.dds")
{
}

void WinState::Initialise()
{
  D3D& d3d = WinUtil::Get().GetD3D();

  // Initialise a new sprite batch
  mBatch = new SpriteBatch(&d3d.GetDeviceCtx());

  int w, h;
  WinUtil::Get().GetClientExtents(w, h);

  if (_data->isP1Winner)
  {
    pLogo.Initialise(d3d, "win/player_one.dds");
  }
  else 
  {
    pLogo.Initialise(d3d, "win/player_two.dds");

  }

  pBackground.Initialise(Vector2(w / 2, h / 2));
  pLogo.Initialise(Vector2(w / 2, h / 4.5f));
  pPlay.Initialise(Vector2(w / 2, h / 2.25f));
  pExit.Initialise(Vector2(w / 2, h / 1.5f));
}

void WinState::Update(float dTime)
{
  if (pPlay.IsMouseOverButton(_data->input.GetMousePos(true)))
  {
    pPlay.ButtonHover(true);

    if (_data->input.GetMouseButton(MouseAndKeys::ButtonT::LBUTTON))
    {
      _data->machine.AddState(StateRef(std::make_unique<GameState>(_data)), true);
    }
  }
  else
  {
    pPlay.ButtonHover(false);
  }

  if (pExit.IsMouseOverButton(_data->input.GetMousePos(true)))
  {
    pExit.ButtonHover(true);

    if (_data->input.GetMouseButton(MouseAndKeys::ButtonT::LBUTTON))
    {
      PostQuitMessage(0);
    }
  }
  else
  {
    pExit.ButtonHover(false);
  }
}

void WinState::Render(float dTime)
{
  D3D& d3d = WinUtil::Get().GetD3D();
  d3d.BeginRender(Colours::Black);

  // Starts the sprite batch
  CommonStates dxstate(&d3d.GetDevice());
  mBatch->Begin(SpriteSortMode_Deferred, dxstate.NonPremultiplied(), &d3d.GetWrapSampler());

  pBackground.Draw(*mBatch);
  pLogo.Draw(*mBatch);
  pPlay.Draw(*mBatch);
  pExit.Draw(*mBatch);

  mBatch->End();

  d3d.EndRender();
}

LRESULT WinState::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

void WinState::Release()
{
  State::Release();

  // Deletes the allocated memory for the SpriteBatch
  delete mBatch;
  mBatch = nullptr;
}