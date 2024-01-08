#include "PauseState.h"

#include <iomanip>

#include "CommonStates.h"
#include "WindowUtils.h"
#include "MenuState.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

PauseState::PauseState(GameDataRef data)
  : _data(data), pLogo(WinUtil::Get().GetD3D()), pResume(WinUtil::Get().GetD3D()), pExit(WinUtil::Get().GetD3D())

{
}

void InitialiseSprite(D3D& d3d, Sprite& spr, std::string file, Vector2 origin, Vector2 pos)
{
  // Load texture play button
  spr.SetTex(*d3d.GetCache().LoadTexture(&d3d.GetDevice(), file));
  spr.GetScale() = Vector2(1, 1);

  spr.origin = origin;

  spr.mPos = pos;

  spr.rotation = 0;
}

void PauseState::Initialise()
{
  D3D& d3d = WinUtil::Get().GetD3D();

  // Initialise a new sprite batch
  mBatch = new SpriteBatch(&d3d.GetDeviceCtx());

  int w, h;
  WinUtil::Get().GetClientExtents(w, h);

  pLogo.Initialise("logo.dds", Vector2(w / 2, h / 4.5f));
  pResume.sprite.Initialise("pause/resume.dds", Vector2(w / 2, h / 2.25f));
  pExit.sprite.Initialise("menu/quit.dds", Vector2(w / 2, h / 1.5f));
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
  pResume.sprite.Draw(*mBatch);
  pExit.sprite.Draw(*mBatch);

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