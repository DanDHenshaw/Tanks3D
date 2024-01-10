#include "PauseState.h"

#include <iomanip>

#include "CommonStates.h"
#include "WindowUtils.h"
#include "MenuState.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

PauseState::PauseState(GameDataRef data)
  : _data(data), pLogo(WinUtil::Get().GetD3D(), "logo.dds"), pResume(WinUtil::Get().GetD3D(), "pause/resume.dds"), pExit(WinUtil::Get().GetD3D(), "menu/quit.dds")

{
}

void PauseState::Initialise()
{
  D3D& d3d = WinUtil::Get().GetD3D();

  // Initialise a new sprite batch
  mBatch = new SpriteBatch(&d3d.GetDeviceCtx());

  int w, h;
  WinUtil::Get().GetClientExtents(w, h);

  pLogo.Initialise(Vector2(w / 2, h / 4.5f));
  pResume.Initialise(Vector2(w / 2, h / 2.25f));
  pExit.Initialise(Vector2(w / 2, h / 1.5f));
}

void PauseState::Update(float dTime)
{
  if (pResume.IsMouseOverButton(_data->input.GetMousePos(true)))
  {
    pResume.ButtonHover(true);

    if (_data->input.GetMouseButton(MouseAndKeys::ButtonT::LBUTTON))
    {
      _data->machine.RemoveState();
    }
  }
  else
  {
    pResume.ButtonHover(false);
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

void PauseState::Render(float dTime)
{
  D3D& d3d = WinUtil::Get().GetD3D();
  d3d.BeginRender(Colours::Black);

  // Starts the sprite batch
  CommonStates dxstate(&d3d.GetDevice());
  mBatch->Begin(SpriteSortMode_Deferred, dxstate.NonPremultiplied(), &d3d.GetWrapSampler());

  pLogo.Draw(*mBatch);
  pResume.Draw(*mBatch);
  pExit.Draw(*mBatch);

  mBatch->End();

  d3d.EndRender();
}

LRESULT PauseState::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		// Respond to a keyboard event.
	case WM_CHAR:
		switch (wParam)
		{
		case 27:
			// Remove the current state from the state machine
			_data->machine.RemoveState();
			return 0;
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			return 0;
		}
	}

	return WinUtil::Get().DefaultMssgHandler(hwnd, msg, wParam, lParam);
}

void PauseState::Release()
{
  State::Release();

  // Deletes the allocated memory for the SpriteBatch
  delete mBatch;
  mBatch = nullptr;
}