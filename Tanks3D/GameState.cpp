#include "GameState.h"

#include <iostream>

#include "GameObject.h"
#include "GeometryBuilder.h"
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
	mGameObjects[Modelid::BOX] = &box;
	mGameObjects[Modelid::BOX]->GetScale() = Vector3(0.5f, 0.5f, 0.5f);
	mGameObjects[Modelid::BOX]->GetPosition() = Vector3(0, -0.5f, 1);
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
	mGameObjects[Modelid::CROSS] = &cross;
	mat.flags &= ~Material::TFlags::TRANSPARENCY;
	mGameObjects[Modelid::CROSS]->GetScale() = Vector3(0.5f, 0.5f, 0.5f);
	mGameObjects[Modelid::CROSS]->GetPosition() = Vector3(1.5f, -0.45f, 1);
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
	mGameObjects[Modelid::WINDOW] = &cross;
	mGameObjects[Modelid::WINDOW]->GetScale() = Vector3(0.75f, 0.75f, 0.75f);
	mGameObjects[Modelid::WINDOW]->GetPosition() = Vector3(-1.75f, 0, 1.25f);
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
	mGameObjects[Modelid::FLOOR] = &wFloor;
	mGameObjects[Modelid::FLOOR]->GetScale() = Vector3(3, 1, 3);
	mGameObjects[Modelid::FLOOR]->GetPosition() = Vector3(0, -1, 0);
	mGameObjects[Modelid::FLOOR]->GetRotation() = Vector3(0, 0, 0);
	mat = wFloor.GetModel().GetMesh().GetSubMesh(0).material;
	mat.gfxData.Set(Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "floor.dds");
	mat.texture = "floor.dds";
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::FLOOR]))
		obj->GetModel().SetOverrideMat(&mat);


	//back wall
	GameObject3D bWall(d3d, quadMesh);
	mGameObjects[Modelid::BACK_WALL] = &bWall;
	mGameObjects[Modelid::BACK_WALL]->GetScale() = Vector3(3, 1, 1.5f);
	mGameObjects[Modelid::BACK_WALL]->GetPosition() = Vector3(0, 0.5f, 3);
	mGameObjects[Modelid::BACK_WALL]->GetRotation() = Vector3(-PI / 2, 0, 0);
	mat.gfxData.Set(Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "wall.dds");
	mat.texture = "wall.dds";
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::BACK_WALL]))
		obj->GetModel().SetOverrideMat(&mat);

	//left wall
	GameObject3D lWall(d3d, quadMesh);
	mGameObjects[Modelid::LEFT_WALL] = &lWall;
	mGameObjects[Modelid::LEFT_WALL]->GetScale() = Vector3(3, 1, 1.5f);
	mGameObjects[Modelid::LEFT_WALL]->GetPosition() = Vector3(-3, 0.5f, 0);
	mGameObjects[Modelid::LEFT_WALL]->GetRotation() = Vector3(-PI / 2, -PI / 2, 0);
	if (auto* obj = dynamic_cast<GameObject3D*>(mGameObjects[Modelid::LEFT_WALL]))
		obj->GetModel().SetOverrideMat(&mat);
	mLoadData.loadedSoFar++;

	//hero models
	GameObject3D cb(d3d, "Cube", "two_mat_cube.fbx");
	mGameObjects[Modelid::SUCK] = &cb;
	mGameObjects[Modelid::SUCK]->GetScale() = Vector3(0.09f, 0.09f, 0.09f);
	mGameObjects[Modelid::SUCK]->GetPosition() = Vector3(1, -0.f, -1.5f);
	mGameObjects[Modelid::SUCK]->GetRotation() = Vector3(PI / 2.f, 0, 0);
	mLoadData.loadedSoFar++;

	GameObject3D ms(d3d, "rock", "the_rock/TheRock2.obj");
	mGameObjects[Modelid::ROCK] = &ms;
	mGameObjects[Modelid::ROCK]->GetScale() = Vector3(0.0035f, 0.0035f, 0.0035f);
	mGameObjects[Modelid::ROCK]->GetPosition() = Vector3(-1, 0.25f, -2.5f);
	mGameObjects[Modelid::ROCK]->GetRotation() = Vector3(0, 0, 0);
	mLoadData.loadedSoFar++;

	GameObject3D dr(d3d, "dragon", "dragon/dragon.x");
	dr.GetScale() = Vector3(0.002f, 0.002f, 0.002f);
	dr.GetPosition() = Vector3(-1, 0.f, -1.5f);
	dr.GetRotation() = Vector3(0, PI, 0);
	mGameObjects[Modelid::DRAGON] = &dr;
	mLoadData.loadedSoFar++;

	GameObject3D dx(d3d, "DrX", "drX/DrX.fbx");
	mGameObjects[Modelid::SCIENTIST] = &dx;
	mGameObjects[Modelid::SCIENTIST]->GetScale() = Vector3(.075f, .075f, .075f);
	mGameObjects[Modelid::SCIENTIST]->GetPosition() = Vector3(1, 0.5f, -2.5f);
	mGameObjects[Modelid::SCIENTIST]->GetRotation() = Vector3(PI / 2, PI, 0);
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
	mGameObjects[Modelid::BOX]->GetRotation().y = gAngle;

	mGameObjects[Modelid::CROSS]->GetRotation().y = -gAngle;
	mGameObjects[Modelid::CROSS2]->GetRotation().y = -gAngle;

	mGameObjects[Modelid::WINDOW]->GetRotation().y = -gAngle * 0.5f;
	mGameObjects[Modelid::WINDOW2]->GetRotation().y = -gAngle * 0.5f;

	std::vector<GameObject*> spinme{ mGameObjects[Modelid::DRAGON], mGameObjects[Modelid::SCIENTIST], mGameObjects[Modelid::ROCK], mGameObjects[Modelid::SUCK] };

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
	d3d.GetFX().SetupSpotLight(1, true, mGameObjects[Modelid::BOX]->GetPosition(), dir, Vector3(0.2f, 0.05f, 0.05f), Vector3(0.01f, 0.01f, 0.01f), Vector3(0.01f, 0.01f, 0.01f));
	dir *= -1;
	d3d.GetFX().SetupSpotLight(2, true, mGameObjects[Modelid::BOX]->GetPosition(), dir, Vector3(0.05f, 0.2f, 0.05f), Vector3(0.01f, 0.01f, 0.01f), Vector3(0.01f, 0.01f, 0.01f));
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
