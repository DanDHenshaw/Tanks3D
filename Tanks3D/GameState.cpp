#include "GameState.h"

#include <iostream>

#include "GeometryBuilder.h"
#include "PauseState.h"
#include "WindowUtils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Setup(Model& m, Mesh& source, const Vector3& scale, const DirectX::SimpleMath::Vector3& pos, const Vector3& rot)
{
	m.Initialise(source);
	m.GetScale() = scale;
	m.GetPosition() = pos;
	m.GetRotation() = rot;
}

void Setup(Model& m, Mesh& source, float scale, const Vector3& pos, const Vector3& rot)
{
	Setup(m, source, Vector3(scale, scale, scale), pos, rot);
}

GameState::GameState(GameDataRef data)
	: _data(data)
{
}

void GameState::Load()
{
	D3D& d3d = WinUtil::Get().GetD3D();

	Model m;
	mModels.insert(mModels.begin(), Modelid::TOTAL, m);

	Mesh& quadMesh = BuildQuad(d3d.GetMeshMgr());
	Mesh& cubeMesh = BuildCube(d3d.GetMeshMgr());

	//textured lit box
	mModels[Modelid::BOX].Initialise(cubeMesh);
	mModels[Modelid::BOX].GetPosition() = Vector3(0, -0.5f, 1);
	mModels[Modelid::BOX].GetScale() = Vector3(0.5f, 0.5f, 0.5f);
	Material mat = mModels[3].GetMesh().GetSubMesh(0).material;
	mat.gfxData.Set(Vector4(0.5, 0.5, 0.5, 1), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "tiles.dds");
	mat.texture = "tiles.dds";
	mat.flags |= Material::TFlags::TRANSPARENCY;
	mat.SetBlendFactors(0.5, 0.5, 0.5, 1);
	mModels[Modelid::BOX].SetOverrideMat(&mat);

	//cross
	mModels[Modelid::CROSS].Initialise(cubeMesh);
	mat.flags &= ~Material::TFlags::TRANSPARENCY;
	mModels[Modelid::CROSS].GetScale() = Vector3(0.5f, 0.5f, 0.5f);
	mModels[Modelid::CROSS].GetPosition() = Vector3(1.5f, -0.45f, 1);
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "cross.dds");
	mat.texture = "cross";
	mat.flags |= Material::TFlags::ALPHA_TRANSPARENCY;
	mat.flags &= ~Material::TFlags::CCW_WINDING;	//render the back
	mModels[Modelid::CROSS].SetOverrideMat(&mat);

	mModels[Modelid::CROSS2] = mModels[Modelid::CROSS];
	mat.flags |= Material::TFlags::CCW_WINDING;	//render the front
	mModels[Modelid::CROSS2].SetOverrideMat(&mat);

	//window
	mModels[Modelid::WINDOW].Initialise(cubeMesh);
	mModels[Modelid::WINDOW].GetScale() = Vector3(0.75f, 0.75f, 0.75f);
	mModels[Modelid::WINDOW].GetPosition() = Vector3(-1.75f, 0, 1.25f);
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "alphaWindow.dds");
	mat.texture = "alphawindow";
	mat.flags |= Material::TFlags::ALPHA_TRANSPARENCY;
	mat.flags &= ~Material::TFlags::CCW_WINDING;	//render the back
	mModels[Modelid::WINDOW].SetOverrideMat(&mat);

	mModels[Modelid::WINDOW2] = mModels[Modelid::WINDOW];
	mat.flags |= Material::TFlags::CCW_WINDING;	//render the front
	mModels[Modelid::WINDOW2].SetOverrideMat(&mat);

	//quad wood floor
	Setup(mModels[Modelid::FLOOR], quadMesh, Vector3(3, 1, 3), Vector3(0, -1, 0), Vector3(0, 0, 0));
	mat = mModels[Modelid::FLOOR].GetMesh().GetSubMesh(0).material;
	mat.gfxData.Set(Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "floor.dds");
	mat.texture = "floor.dds";
	mModels[Modelid::FLOOR].SetOverrideMat(&mat);

	//back wall
	Setup(mModels[Modelid::BACK_WALL], quadMesh, Vector3(3, 1, 1.5f), Vector3(0, 0.5f, 3), Vector3(-PI / 2, 0, 0));
	mat.gfxData.Set(Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "wall.dds");
	mat.texture = "wall.dds";
	mModels[Modelid::BACK_WALL].SetOverrideMat(&mat);

	//left wall
	Setup(mModels[Modelid::LEFT_WALL], quadMesh, Vector3(3, 1, 1.5f), Vector3(-3, 0.5f, 0), Vector3(-PI / 2, -PI / 2, 0));
	mModels[Modelid::LEFT_WALL].SetOverrideMat(&mat);
	mLoadData.loadedSoFar++;

	//hero models
	Mesh& cb = d3d.GetMeshMgr().CreateMesh("Cube");
	cb.CreateFrom("../bin/data/two_mat_cube.fbx", d3d);
	Setup(mModels[Modelid::SUCK], cb, 0.09f, Vector3(1, -0.f, -1.5f), Vector3(PI / 2.f, 0, 0));
	mLoadData.loadedSoFar++;

	Mesh& ms = d3d.GetMeshMgr().CreateMesh("rock");
	ms.CreateFrom("../bin/data/the_rock/TheRock2.obj", d3d);
	Setup(mModels[Modelid::ROCK], ms, 0.0035f, Vector3(-1, 0.25f, -2.5f), Vector3(0, 0, 0));
	mLoadData.loadedSoFar++;

	Mesh& dr = d3d.GetMeshMgr().CreateMesh("dragon");
	dr.CreateFrom("../bin/data/dragon/dragon.x", d3d);
	Setup(mModels[Modelid::DRAGON], dr, 0.002f, Vector3(-1, 0.f, -1.5f), Vector3(0, PI, 0));
	mLoadData.loadedSoFar++;

	Mesh& dx = d3d.GetMeshMgr().CreateMesh("DrX");
	dx.CreateFrom("../bin/data/drX/DrX.fbx", d3d);
	Setup(mModels[Modelid::SCIENTIST], dx, .075f, Vector3(1, 0.5f, -2.5f), Vector3(PI / 2, PI, 0));
	mLoadData.loadedSoFar++;

	d3d.GetFX().SetupDirectionalLight(0, true, Vector3(-0.7f, -0.7f, 0.7f), Vector3(0.47f, 0.47f, 0.47f), Vector3(0.15f, 0.15f, 0.15f), Vector3(0.25f, 0.25f, 0.25f));
}

void GameState::Initialise()
{
	D3D& d3d = WinUtil::Get().GetD3D();
	pFontBatch = new SpriteBatch(&d3d.GetDeviceCtx());
	assert(pFontBatch);
	pFont = new SpriteFont(&d3d.GetDevice(), L"../bin/data/fonts/algerian.spritefont");
	assert(pFont);

	mLoadData.totalToLoad = 5;
	mLoadData.loadedSoFar = 0;
	mLoadData.running = true;
	mLoadData.loader = std::async(std::launch::async, &GameState::Load, this);
}

void GameState::Update(float dTime)
{
	if(mLoadData.loadedSoFar < mLoadData.totalToLoad) return;
	
	gAngle += dTime * 0.5f;
	mModels[Modelid::BOX].GetRotation().y = gAngle;

	mModels[Modelid::CROSS].GetRotation().y = -gAngle;
	mModels[Modelid::CROSS2].GetRotation().y = -gAngle;

	mModels[Modelid::WINDOW].GetRotation().y = -gAngle * 0.5f;
	mModels[Modelid::WINDOW2].GetRotation().y = -gAngle * 0.5f;

	std::vector<Model*> spinme{ &mModels[Modelid::DRAGON], &mModels[Modelid::ROCK], &mModels[Modelid::SCIENTIST], &mModels[Modelid::SUCK] };

	for (size_t i = 0; i < spinme.size(); ++i) {
		spinme[i]->GetPosition().y = (sinf(2 * GetClock() + (PI / 4) * i)) * 0.5f;
		spinme[i]->GetRotation().y += (i < 2) ? dTime : -dTime;
	}
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

	//main cube - forced transparency under pogram control
	Vector3 dir = Vector3(1, 0, 0);
	Matrix m = Matrix::CreateRotationY(gAngle);
	dir = dir.TransformNormal(dir, m);
	d3d.GetFX().SetupSpotLight(1, true, mModels[Modelid::BOX].GetPosition(), dir, Vector3(0.2f, 0.05f, 0.05f), Vector3(0.01f, 0.01f, 0.01f), Vector3(0.01f, 0.01f, 0.01f));
	dir *= -1;
	d3d.GetFX().SetupSpotLight(2, true, mModels[Modelid::BOX].GetPosition(), dir, Vector3(0.05f, 0.2f, 0.05f), Vector3(0.01f, 0.01f, 0.01f), Vector3(0.01f, 0.01f, 0.01f));
	float d = sinf(gAngle) * 0.5f + 0.5f;
	mModels[Modelid::BOX].HasOverrideMat()->SetBlendFactors(d, d, d, 1);

	for (auto& mod : mModels)
		d3d.GetFX().Render(mod);

	d3d.EndRender();
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
	const float camInc = 200.f * GetElapsedSec();
	//do something game specific here
	switch (msg)
	{
		// Respond to a keyboard event.
	case WM_CHAR:
		switch (wParam)
		{
		case 27:
			// Escape Pressed
			_data->machine.AddState(StateRef(std::make_unique<PauseState>(_data)), false);
			return 0;
		case 'q':
		case 'Q':
			PostQuitMessage(0);
			return 0;
		case 'a':
			mCamPos.y += camInc;
			break;
		case 'z':
			mCamPos.y -= camInc;
			break;
		case 'd':
			mCamPos.x -= camInc;
			break;
		case 'f':
			mCamPos.x += camInc;
			break;
		case 'w':
			mCamPos.z += camInc;
			break;
		case 's':
			mCamPos.z -= camInc;
			break;
		case ' ':
			mCamPos = mDefCamPos;
			break;
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
