#include "SplashState.h"

#include <chrono>

#include "CommonStates.h"
#include "MenuState.h"
#include "WindowUtils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

SplashState::SplashState(GameDataRef data)
	: _data(data), mLogo(WinUtil::Get().GetD3D())
{
}

void SplashState::Initialise()
{
	D3D& d3d = WinUtil::Get().GetD3D();

	// Initialise a new sprite batch
	mBatch = new SpriteBatch(&d3d.GetDeviceCtx());

	// Load texture and set its alpha to 0
	mLogo.SetTex(*d3d.GetCache().LoadTexture(&d3d.GetDevice(), "splash/company_logo.dds"));
	mLogo.colour.w = 0;
	mLogo.GetScale() = Vector2(1, 1);

	mLogo.origin = Vector2(512 / 2, 512 / 2);

	// Places logo in the center of the screen
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mLogo.mPos = Vector2(w / 2, h / 2);

	mLogo.rotation = 0;
}

void SplashState::Update(float dTime)
{
	mElapsedTime += dTime;

	if (mElapsedTime > mShowtime)
	{
		// Fades the logo out and once faded transition to the MenuState
		if(mLogo.FadeOut(dTime, mShowtime / 2))
			// Swap from SplashState to MenuState.
			_data->machine.AddState(StateRef(std::make_unique<MenuState>(_data)), true);

		return;
	}

	// Fades the logo in
	mLogo.FadeIn(dTime, mShowtime / 2);
}

void SplashState::Render(float dTime)
{
	D3D& d3d = WinUtil::Get().GetD3D();
	d3d.BeginRender(Colours::Black);

	// Starts the sprite batch
	CommonStates dxstate(&d3d.GetDevice());
	mBatch->Begin(SpriteSortMode_Deferred, dxstate.NonPremultiplied(), &d3d.GetWrapSampler());

	mLogo.Draw(*mBatch);

	mBatch->End();

	d3d.EndRender();
}

LRESULT SplashState::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

void SplashState::Release()
{
	State::Release();

	// Deletes the allocated memory for the SpriteBatch
	delete mBatch;
	mBatch = nullptr;
}