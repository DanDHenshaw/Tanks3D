#include "GameState.h"

#include <iostream>

#include "GeometryBuilder.h"
#include "PauseState.h"
#include "WindowUtils.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

GameState::GameState(GameDataRef data)
	: _data(data)
{
}

void GameState::Initialise()
{
	D3D& d3d = WinUtil::Get().GetD3D();

	mQuad.Initialise(BuildQuad(d3d.GetMeshMgr()));
	mBCross.Initialise(BuildCube(d3d.GetMeshMgr()));
	mBall.Initialise(BuildSphere(d3d.GetMeshMgr(), 17, 17));
	mBWin = mBCross;

	//textured lit box
	mBox = mBCross;
	mBox.GetPosition() = Vector3(0, -0.5f, 1);
	mBox.GetScale() = Vector3(0.5f, 0.5f, 0.5f);
	Material mat = mQuad.GetMesh().GetSubMesh(0).material;
	mat.gfxData.Set(Vector4(0.5, 0.5, 0.5, 1), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "tiles.dds");
	mat.texture = "tiles.dds";
	mat.flags |= Material::TFlags::TRANSPARENCY;
	mat.SetBlendFactors(0.5, 0.5, 0.5, 1);
	mBox.SetOverrideMat(&mat);

	mat.flags &= ~Material::TFlags::TRANSPARENCY;
	mBRot = mBCross;
	mBRot.GetScale() = Vector3(0.25f, 0.25f, 0.25f);
	mBRot.GetPosition() = Vector3(-1, -0.5f, -1.f);
	mBRot.SetOverrideMat(&mat);

	mBScale = mBCross;
	mBScale.GetScale() = Vector3(0.25f, 0.25f, 0.25f);
	mBScale.GetPosition() = Vector3(0, -0.5f, -1.5f);
	mBScale.SetOverrideMat(&mat);

	mBScroll = mBCross;
	mBScroll.GetScale() = Vector3(0.25f, 0.25f, 0.25f);
	mBScroll.GetPosition() = Vector3(1, -0.5f, -1.f);
	mBScroll.SetOverrideMat(&mat);

	mBCross.GetScale() = Vector3(0.5f, 0.5f, 0.5f);
	mBCross.GetPosition() = Vector3(1.5f, -0.45f, 1);
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "cross.dds");
	mat.texture = "cross";
	mat.flags |= Material::TFlags::ALPHA_TRANSPARENCY;
	mBCross.SetOverrideMat(&mat);

	mBWin.GetScale() = Vector3(0.75f, 0.75f, 0.75f);
	mBWin.GetPosition() = Vector3(-1.75f, 0, 1.25f);
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "alphaWindow.dds");
	mat.texture = "alphawindow";
	mat.flags |= Material::TFlags::ALPHA_TRANSPARENCY;
	mBWin.SetOverrideMat(&mat);

	//wood floor
	mQuad.GetScale() = Vector3(3, 1, 3);
	mQuad.GetPosition() = Vector3(0, -1, 0);
	mat = mQuad.GetMesh().GetSubMesh(0).material;
	mat.gfxData.Set(Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 0), Vector4(0.9f, 0.8f, 0.8f, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "floor.dds");
	mat.texture = "floor.dds";
	mQuad.SetOverrideMat(&mat);

	d3d.GetCache().LoadTexture(&d3d.GetDevice(), "wall.dds");

	//ball
	mBall.GetScale() = Vector3(0.4f, 0.4f, 0.4f);
	mBall.GetPosition() = Vector3(2.25f, -0.5f, 0);
	Material* pMat = &mBall.GetMesh().GetSubMesh(0).material;
	pMat->gfxData.Diffuse = Vector4(0.5f, 0.5f, 1, 0);
	pMat->gfxData.Ambient = Vector4(1, 1, 1, 0);
	pMat->gfxData.Specular = Vector4(1, 1, 1, 10);

	d3d.GetFX().SetupDirectionalLight(0, true, Vector3(-0.7f, -0.7f, 0.7f), Vector3(0.47f, 0.47f, 0.47f), Vector3(0.15f, 0.15f, 0.15f), Vector3(0.25f, 0.25f, 0.25f));
}

void GameState::Update(float dTime)
{
	gAngle += dTime * 0.5f;
	mBox.GetRotation().y = gAngle;

	mBCross.GetRotation().y = -gAngle;

	mBWin.GetRotation().y = -gAngle * 0.5f;

	mBScroll.GetRotation().y = sinf(gAngle);
	mBRot.GetRotation().y = cosf(gAngle);
	mBScale.GetRotation().y = -cosf(gAngle);
}

void GameState::Render(float dTime)
{
	D3D& d3d = WinUtil::Get().GetD3D();

	float alpha = 0.5f + sinf(gAngle * 2) * 0.5f;


	d3d.GetFX().SetPerFrameConsts(d3d.GetDeviceCtx(), mCamPos);

	CreateViewMatrix(d3d.GetFX().GetViewMatrix(), mCamPos, Vector3(0, 0, 0), Vector3(0, 1, 0));
	CreateProjectionMatrix(d3d.GetFX().GetProjectionMatrix(), 0.25f * PI, WinUtil::Get().GetAspectRatio(), 1, 1000.f);
	Matrix w = Matrix::CreateRotationY(sinf(gAngle));
	d3d.GetFX().SetPerObjConsts(d3d.GetDeviceCtx(), w);

	//floor
	mQuad.GetRotation() = Vector3(0, 0, 0);
	mQuad.GetScale() = Vector3(3, 1, 3);
	mQuad.GetPosition() = Vector3(0, -1, 0);
	d3d.GetFX().Render(mQuad);

	Material mat = mQuad.GetMesh().GetSubMesh(0).material;
	mat.gfxData.Set(Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
	mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "wall.dds");
	mat.texture = "wall.dds";

	//walls
	mQuad.GetRotation() = Vector3(-PI / 2, 0, 0);
	mQuad.GetScale() = Vector3(3, 1, 1.5f);
	mQuad.GetPosition() = Vector3(0, 0.5f, 3);
	d3d.GetFX().Render(mQuad, &mat);

	mQuad.GetRotation() = Vector3(-PI / 2, -PI / 2, 0);
	mQuad.GetScale() = Vector3(3, 1, 1.5f);
	mQuad.GetPosition() = Vector3(-3, 0.5f, 0);
	d3d.GetFX().Render(mQuad, &mat);


	//main cube - forced transparency under pogram control
	Vector3 dir = Vector3(1, 0, 0);
	Matrix m = Matrix::CreateRotationY(gAngle);
	dir = dir.TransformNormal(dir, m);
	d3d.GetFX().SetupSpotLight(1, true, mBox.GetPosition(), dir, Vector3(0.2f, 0.05f, 0.05f), Vector3(0.01f, 0.01f, 0.01f), Vector3(0.01f, 0.01f, 0.01f));
	dir *= -1;
	d3d.GetFX().SetupSpotLight(2, true, mBox.GetPosition(), dir, Vector3(0.05f, 0.2f, 0.05f), Vector3(0.01f, 0.01f, 0.01f), Vector3(0.01f, 0.01f, 0.01f));
	float d = sinf(gAngle) * 0.5f + 0.5f;
	mBox.HasOverrideMat()->SetBlendFactors(d, d, d, 1);
	d3d.GetFX().Render(mBox);

	//rotating
	mBRot.HasOverrideMat()->texTrsfm.angle = gAngle;
	d3d.GetFX().Render(mBRot);

	//scaling
	float scale = sinf(gAngle * 5) * 0.5f + 0.75f;
	mBScale.HasOverrideMat()->texTrsfm.scale = Vector2(scale, scale);
	d3d.GetFX().Render(mBScale);

	//scrolling
	mBScroll.HasOverrideMat()->texTrsfm.translate = Vector2(gAngle, 0);
	d3d.GetFX().Render(mBScroll);

	//alpha transparency
	mBCross.HasOverrideMat()->flags &= ~Material::TFlags::CCW_WINDING;	//render the back
	d3d.GetFX().Render(mBCross);
	mBCross.HasOverrideMat()->flags |= Material::TFlags::CCW_WINDING;	//render the front
	d3d.GetFX().Render(mBCross);

	mBWin.HasOverrideMat()->flags &= ~Material::TFlags::CCW_WINDING;	//render the back
	d3d.GetFX().Render(mBWin);
	mBWin.HasOverrideMat()->flags |= Material::TFlags::CCW_WINDING;	//render the front
	d3d.GetFX().Render(mBWin);

	d3d.GetFX().Render(mBall);
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
}
