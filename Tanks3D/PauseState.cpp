#include "PauseState.h"

#include <iomanip>

#include "CommonStates.h"
#include "WindowUtils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

PauseState::PauseState(GameDataRef data)
	: _data(data)
{
}

void PauseState::Initialise()
{
	D3D& d3d = WinUtil::Get().GetD3D();

	// Initialises SpriteBatch
	mBatch = new SpriteBatch(&d3d.GetDeviceCtx());

	// Loads a font to the SpriteFont
	mPauseLabel = d3d.GetCache().LoadFont(&d3d.GetDevice(), "fonts/algerian.spritefont");
}

void PauseState::Update(float dTime)
{
}

void PauseState::Render(float dTime)
{
	D3D& d3d = WinUtil::Get().GetD3D();
	d3d.BeginRender(Colours::Black);

	CommonStates dxstate(&d3d.GetDevice());
	mBatch->Begin(SpriteSortMode_Deferred, dxstate.NonPremultiplied(), &d3d.GetWrapSampler());

	// Sets SpriteFont string and draws it to the center of the screen
	std::string text = "PAUSE";
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mPauseLabel->DrawString(mBatch, text.c_str(), Vector2(w / 2, h / 2), Vector4(1, 1, 1, 1));

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

	// Deletes the allocated memory for the SpriteFont
	delete mPauseLabel;
	mPauseLabel = nullptr;
}