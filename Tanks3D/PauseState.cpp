#include "PauseState.h"

#include <iomanip>

#include "CommonStates.h"
#include "WindowUtils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

PauseState::PauseState(GameDataRef data)
	: _data(data), test(WinUtil::Get().GetD3D())
{
}

void PauseState::Initialise()
{
	D3D& d3d = WinUtil::Get().GetD3D();

	batch = new SpriteBatch(&d3d.GetDeviceCtx());

	mPauseLabel = d3d.GetCache().LoadFont(&d3d.GetDevice(), "fonts/comicSansMS.spritefont");

	test.SetTex(*d3d.GetCache().LoadTexture(&d3d.GetDevice(), "tiles.dds"));
	test.SetScale(Vector2(1, 1));
	test.mPos = Vector2(0, 0);
}

void PauseState::Update(float dTime)
{
}

void PauseState::Render(float dTime)
{
	D3D& d3d = WinUtil::Get().GetD3D();

	CommonStates dxstate(&d3d.GetDevice());
	batch->Begin(SpriteSortMode_Deferred, dxstate.NonPremultiplied(), &d3d.GetWrapSampler());

	std::string text = "PAUSE";
	int w, h;
	WinUtil::Get().GetClientExtents(w, h);
	mPauseLabel->DrawString(batch, text.c_str(), Vector2(w / 2, h / 2), Vector4(1, 1, 1, 1));

	test.Draw(*batch);

	batch->End();
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
			// Escape Pressed
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

	delete batch;
	batch = nullptr;

	delete mPauseLabel;
	mPauseLabel = nullptr;
}