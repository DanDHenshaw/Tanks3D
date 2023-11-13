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

	mBatch = new SpriteBatch(&d3d.GetDeviceCtx());

	mLogo.SetTex(*d3d.GetCache().LoadTexture(&d3d.GetDevice(), "splash/logo.dds"));
	mLogo.SetScale(Vector2(1, 1));

	mLogo.origin = Vector2(512 / 2, 512 / 2);

	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mLogo.mPos = Vector2(w / 2, h / 2);

	mLogoColor = Vector4(1, 1, 1, 0);
}

void SplashState::Update(float dTime)
{
	mElapsedTime += dTime;

	if (mElapsedTime > mShowtime)
	{
		if(mLogo.FadeOut(dTime, 1))
			// Swap from SplashState to MenuState.
			_data->machine.AddState(StateRef(std::make_unique<MenuState>(_data)), true);

		return;
	}

	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mLogo.mPos = Vector2(w / 2, h / 2);

	mLogo.FadeIn(dTime, 1);
}

void SplashState::Render(float dTime)
{
	D3D& d3d = WinUtil::Get().GetD3D();

	CommonStates dxstate(&d3d.GetDevice());
	mBatch->Begin(SpriteSortMode_Deferred, dxstate.NonPremultiplied(), &d3d.GetWrapSampler());

	mLogo.Draw(*mBatch);

	mBatch->End();
}

LRESULT SplashState::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

void SplashState::Release()
{
	State::Release();

	delete mBatch;
	mBatch = nullptr;
}