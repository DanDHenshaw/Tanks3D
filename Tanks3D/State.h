#pragma once
#include "D3D.h"

class State
{
public:
	/*
	 Initialises the state.
	 */
	virtual void Initialise() = 0;
	/*
	 Updates the state every frame.
		- delta is the DeltaTime
	 */
	virtual void Update(float dTime) = 0;
	/*
	 Draws the state every frame.
		- delta is the DeltaTime
	 */
	virtual void Render(float dTime) = 0;

	// Win32 message handler
	virtual LRESULT WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;

	/*
	 Pauses the state.
	 */
	virtual void Pause() {}
	/*
	 Resumes the state.
	 */
	virtual void Resume() {}

	// Release all data stored in state
	virtual void Release() {}

public:
	// Camera
	const DirectX::SimpleMath::Vector3 mDefCamPos = DirectX::SimpleMath::Vector3(0, 2, -5);
	DirectX::SimpleMath::Vector3 mCamPos = DirectX::SimpleMath::Vector3(0, 2, -5);
};
