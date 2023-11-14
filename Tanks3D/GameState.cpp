#include "GameState.h"

#include <iostream>

#include "GameObject.h"
#include "GeometryBuilder.h"
#include "Input.h"
#include "PauseState.h"
#include "WindowUtils.h"

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

	//textured lit box
	GameObject3D box(d3d, cubeMesh);
	box.GetScale() = Vector3(0.5f, 0.5f, 0.5f);
	box.GetPosition() = Vector3(0, -0.5f, 1);
	mGameObjects[Modelid::BOX] = &box;
	Material mat = box.GetModel().GetMesh().GetSubMesh(0).material;
	mat.gfxData.Set(Vector4(0.5, 0.5, 0.5, 1), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "tiles.dds");
	mat.texture = "tiles.dds";
	mat.flags |= Material::TFlags::TRANSPARENCY;
	mat.SetBlendFactors(0.5, 0.5, 0.5, 1);
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::BOX]))
		obj->GetModel().SetOverrideMat(&mat);


	//cross
	GameObject3D cross(d3d, cubeMesh);
	cross.GetScale() = Vector3(0.5f, 0.5f, 0.5f);
	cross.GetPosition() = Vector3(1.5f, -0.45f, 1);
	mGameObjects[Modelid::CROSS] = &cross;
	mat.flags &= ~Material::TFlags::TRANSPARENCY;
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "cross.dds");
	mat.texture = "cross";
	mat.flags |= Material::TFlags::ALPHA_TRANSPARENCY;
	mat.flags &= ~Material::TFlags::CCW_WINDING;	//render the back
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::CROSS]))
		obj->GetModel().SetOverrideMat(&mat);

	mGameObjects[Modelid::CROSS2] = mGameObjects[Modelid::CROSS];
	mat.flags |= Material::TFlags::CCW_WINDING;	//render the front
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::CROSS2]))
		obj->GetModel().SetOverrideMat(&mat);

	//window
	GameObject3D window(d3d, cubeMesh);
	window.GetScale() = Vector3(0.75f, 0.75f, 0.75f);
	window.GetPosition() = Vector3(-1.75f, 0, 1.25f);
	mGameObjects[Modelid::WINDOW] = &cross;
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "alphaWindow.dds");
	mat.texture = "alphawindow";
	mat.flags |= Material::TFlags::ALPHA_TRANSPARENCY;
	mat.flags &= ~Material::TFlags::CCW_WINDING;	//render the back
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::WINDOW]))
		obj->GetModel().SetOverrideMat(&mat);

	mGameObjects[Modelid::WINDOW2] = mGameObjects[Modelid::WINDOW];
	mat.flags |= Material::TFlags::CCW_WINDING;	//render the front
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::WINDOW2]))
		obj->GetModel().SetOverrideMat(&mat);

	//quad wood floor
	GameObject3D wFloor(d3d, quadMesh);
	wFloor.GetScale() = Vector3(3, 1, 3);
	wFloor.GetPosition() = Vector3(0, -1, 0);
	wFloor.GetRotation() = Vector3(0, 0, 0);
	mGameObjects[Modelid::FLOOR] = &wFloor;
	mat = wFloor.GetModel().GetMesh().GetSubMesh(0).material;
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
	GameObject3D cb(d3d, "Cube", "two_mat_cube.fbx");
	cb.GetScale() = Vector3(0.09f, 0.09f, 0.09f);
	cb.GetPosition() = Vector3(1, -0.f, -1.5f);
	cb.GetRotation() = Vector3(PI / 2.f, 0, 0);
	mGameObjects[Modelid::SUCK] = &cb;
	mLoadData.loadedSoFar++;

	GameObject3D ms(d3d, "rock", "the_rock/TheRock2.obj");
	ms.GetScale() = Vector3(0.0035f, 0.0035f, 0.0035f);
	ms.GetPosition() = Vector3(-1, 0.25f, -2.5f);
	ms.GetRotation() = Vector3(0, 0, 0);
	mGameObjects[Modelid::ROCK] = &ms;
	mLoadData.loadedSoFar++;

	GameObject3D dr(d3d, "dragon", "dragon/dragon.x");
	dr.GetScale() = Vector3(0.002f, 0.002f, 0.002f);
	dr.GetPosition() = Vector3(-1, 0.f, -1.5f);
	dr.GetRotation() = Vector3(0, PI, 0);
	mGameObjects[Modelid::DRAGON] = &dr;
	mLoadData.loadedSoFar++;

	GameObject3D dx(d3d, "DrX", "drX/DrX.fbx");
	dx.GetScale() = Vector3(.075f, .075f, .075f);
	dx.GetPosition() = Vector3(1, 0.5f, -2.5f);
	dx.GetRotation() = Vector3(PI / 2, PI, 0);
	mGameObjects[Modelid::SCIENTIST] = &dx;
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
			//case 'a':
			//	mCamPos.y += camInc;
			//	break;
			//case 'z':
			//	mCamPos.y -= camInc;
			//	break;
			//case 'd':
			//	mCamPos.x -= camInc;
			//	break;
			//case 'f':
			//	mCamPos.x += camInc;
			//	break;
			//case 'w':
			//	mCamPos.z += camInc;
			//	break;
			//case 's':
			//	mCamPos.z -= camInc;
			//	break;
			//case ' ':
			//	mCamPos = mDefCamPos;
			//	break;

	const float camInc = 200.f * GetElapsedSec();
	if(input.IsPressed(VK_W))
	{
		mCamPos.x += camInc;
	}

	if(mLoadData.loadedSoFar < mLoadData.totalToLoad) return;
	
	gAngle += dTime * 0.5f;

	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::BOX]))
		obj->GetRotation().y = gAngle;

	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::CROSS]))
		obj->GetRotation().y = -gAngle;
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::CROSS2]))
		obj->GetRotation().y = -gAngle;


	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::WINDOW]))
		obj->GetRotation().y = -gAngle * 0.5f;
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::WINDOW2]))
		obj->GetRotation().y = -gAngle * 0.5f;

	std::vector<GameObject*> spinme{ mGameObjects[Modelid::DRAGON], mGameObjects[Modelid::SCIENTIST], mGameObjects[Modelid::ROCK], mGameObjects[Modelid::SUCK] };

	for (size_t i = 0; i < spinme.size(); ++i) {
		if (auto* obj = dynamic_cast<GameObject3D*>(spinme[i]))
		{
			obj->GetPosition().y = (sinf(2 * GetClock() + (PI / 4) * i)) * 0.5f;
			obj->GetRotation().y += (i < 2) ? dTime : -dTime;
		}
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
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::BOX]))
	{
		d3d.GetFX().SetupSpotLight(1, true, obj->GetPosition(), dir, Vector3(0.2f, 0.05f, 0.05f), Vector3(0.01f, 0.01f, 0.01f), Vector3(0.01f, 0.01f, 0.01f));
	}
	dir *= -1;
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::BOX]))
	{
		d3d.GetFX().SetupSpotLight(2, true, obj->GetPosition(), dir, Vector3(0.05f, 0.2f, 0.05f), Vector3(0.01f, 0.01f, 0.01f), Vector3(0.01f, 0.01f, 0.01f));
	}
	float d = sinf(gAngle) * 0.5f + 0.5f;

	if(auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::BOX]))
		obj->GetModel().HasOverrideMat()->SetBlendFactors(d, d, d, 1);

	for (GameObject*& mod : mGameObjects)
		mod->Render(d3d, dTime);

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
	input.Initialise(hwnd, false, false);

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
