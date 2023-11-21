#include "GameState.h"

#include <iostream>

#include "GameObject.h"
#include "GeometryBuilder.h"
#include "Input.h"
#include "PauseState.h"
#include "WindowUtils.h"
#include "Tank.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

GameState::GameState(GameDataRef data)
	: _data(data)
{
}

void GameState::Load()
{
	D3D& d3d = WinUtil::Get().GetD3D();

	GameObject* obj;
	mGameObjects.insert(mGameObjects.begin(), Modelid::TOTAL, obj);

	Mesh& quadMesh = BuildQuad(d3d.GetMeshMgr());
	Mesh& cubeMesh = BuildCube(d3d.GetMeshMgr());

	//quad wood floor
	GameObject3D wFloor(d3d, quadMesh);
	wFloor.GetScale() = Vector3(3, 1, 3);
	wFloor.GetPosition() = Vector3(0, -1, 0);
	wFloor.GetRotation() = Vector3(0, 0, 0);
	mGameObjects[Modelid::FLOOR] = &wFloor;
	Material mat = wFloor.GetModel().GetMesh().GetSubMesh(0).material;
	mat.gfxData.Set(Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "floor.dds");
	mat.texture = "floor.dds";
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::FLOOR]))
		obj->GetModel().SetOverrideMat(&mat);

	//back wall
	GameObject3D bWall(d3d, quadMesh);
	bWall.GetScale() = Vector3(3, 1, 1.5f);
	bWall.GetPosition() = Vector3(0, 0.5f, 3);
	bWall.GetRotation() = Vector3(-PI / 2, 0, 0);
	mGameObjects[Modelid::BACK_WALL] = &bWall;
	mat.gfxData.Set(Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "wall.dds");
	mat.texture = "wall.dds";
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::BACK_WALL]))
		obj->GetModel().SetOverrideMat(&mat);

	//left wall
	GameObject3D lWall(d3d, quadMesh);
	lWall.GetScale() = Vector3(3, 1, 1.5f);
	lWall.GetPosition() = Vector3(-3, 0.5f, 0);
	lWall.GetRotation() = Vector3(-PI / 2, -PI / 2, 0);
	mGameObjects[Modelid::LEFT_WALL] = &lWall;
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::LEFT_WALL]))
		obj->GetModel().SetOverrideMat(&mat);
	mLoadData.loadedSoFar++;

	//hero models
	Tank tank(d3d, "tank", "tank/tank.fbx");
	tank.GetScale() = Vector3(.15f, .15f, .15f);
	tank.GetPosition() = Vector3(0, 0, 0);
	tank.GetRotation() = Vector3(PI / 2, PI / 2, 0);
	mGameObjects[Modelid::TANK] = &tank;
	mLoadData.loadedSoFar++;

	d3d.GetFX().SetupDirectionalLight(0, true, Vector3(-0.7f, -0.7f, 0.7f), Vector3(0.47f, 0.47f, 0.47f), Vector3(0.15f, 0.15f, 0.15f), Vector3(0.25f, 0.25f, 0.25f));
}

void GameState::Initialise()
{
	input.Initialise(WinUtil::Get().GetMainWnd(), true, false);

	D3D& d3d = WinUtil::Get().GetD3D();

	pFontBatch = new SpriteBatch(&d3d.GetDeviceCtx());
	assert(pFontBatch);
	pFont = new SpriteFont(&d3d.GetDevice(), L"../bin/data/fonts/algerian.spritefont");
	assert(pFont);

	mLoadData.totalToLoad = 2;
	mLoadData.loadedSoFar = 0;
	mLoadData.running = true;
	mLoadData.loader = std::async(std::launch::async, &GameState::Load, this);
}

void GameState::Update(float dTime)
{
	const float camInc = 200.f * GetElapsedSec();

	if(mLoadData.loadedSoFar < mLoadData.totalToLoad) return;
	
	if (auto* obj = dynamic_cast<Tank*>(mGameObjects[Modelid::TANK]))
		obj->Input(input, dTime);
}

void GameState::Render(float dTime)
{
	if (mLoadData.running)
	{
		if (!mLoadData.loader._Is_ready())
		{
			RenderLoad(dTime);
			return;
		}
		mLoadData.loader.get();
		mLoadData.running = false;
		return;
	}

	D3D& d3d = WinUtil::Get().GetD3D();
	d3d.BeginRender(Colours::Black);

	float alpha = 0.5f + sinf(gAngle * 2) * 0.5f;

	d3d.GetFX().SetPerFrameConsts(d3d.GetDeviceCtx(), mCamPos);

	CreateViewMatrix(d3d.GetFX().GetViewMatrix(), mCamPos, Vector3(0, 0, 0), Vector3(0, 1, 0));
	CreateProjectionMatrix(d3d.GetFX().GetProjectionMatrix(), 0.25f * PI, WinUtil::Get().GetAspectRatio(), 1, 1000.f);
	Matrix w = Matrix::CreateRotationY(sinf(gAngle));
	d3d.GetFX().SetPerObjConsts(d3d.GetDeviceCtx(), w);

	for (GameObject*& mod : mGameObjects)
		mod->Render(d3d, dTime);

	d3d.EndRender();
	input.PostProcess();
}

void GameState::RenderLoad(float dTime)
{
	D3D& d3d = WinUtil::Get().GetD3D();
	d3d.BeginRender(Colours::Black);

	pFontBatch->Begin();

	static int pips = 0;
	static float elapsed = 0;
	elapsed += dTime;
	if (elapsed > 0.25f) {
		pips++;
		elapsed = 0;
	}
	if (pips > 10)
		pips = 0;
	std::wstringstream ss;
	ss << L"Loading meshes(" << (int)(((float)mLoadData.loadedSoFar / (float)mLoadData.totalToLoad) * 100.f) << L"%) ";
	for (int i = 0; i < pips; ++i)
		ss << L'.';
	pFont->DrawString(pFontBatch, ss.str().c_str(), Vector2(100, 200), Colours::White, 0, Vector2(0, 0), Vector2(1.f, 1.f));


	ss.str(L"");
	ss << L"FPS:" << (int)(1.f / dTime);
	pFont->DrawString(pFontBatch, ss.str().c_str(), Vector2(10, 550), Colours::White, 0, Vector2(0, 0), Vector2(0.5f, 0.5f));


	pFontBatch->End();

	d3d.EndRender();
}

LRESULT GameState::WindowsMssgHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//do something game specific here
	switch (msg)
	{
	case WM_INPUT:
		input.MessageEvent((HRAWINPUT)lParam);
		// Respond to a keyboard event.
	case WM_CHAR:
		switch (wParam)
		{
		case VK_ESCAPE:
			// Escape Pressed
			_data->machine.AddState(StateRef(std::make_unique<PauseState>(_data)), false);
			return 0;
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			return 0;
		}
	}

	//default message handling (resize window, full screen, etc)
	return WinUtil::Get().DefaultMssgHandler(hwnd, msg, wParam, lParam);
}

void GameState::Release()
{
	State::Release();

	delete pFontBatch;
	pFontBatch = nullptr;
	delete pFont;
	pFont = nullptr;
}
