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

  // Initialises GameObject vector with ModeLid::TOTAL amount of GameObjects
  GameObject* obj;
  mGameObjects.insert(mGameObjects.begin(), Modelid::TOTAL, obj);

  //ground model
  // Initialises a game object 3d with a ground.fbx file
  GameObject3D ground(d3d, "ground", "ground/ground.fbx");
  ground.GetScale() = Vector3(10);
  ground.GetPosition() = Vector3(0, 0, 0);
  ground.GetRotation() = Vector3(PI / 2, 0, 0);
  Material mat = ground.GetModel().GetMesh().GetSubMesh(0).material;
  mat.gfxData.Set (Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 0), Vector4(1, 1, 1, 1));
  mat.texTrsfm.scale = Vector2(7.5f);
  ground.GetModel().SetOverrideMat(&mat);
  mGameObjects[Modelid::FLOOR] = &ground;
  mLoadData.loadedSoFar++;

  //tank models
  // Initialises a tank object with a tank.fbx file
  Tank p1(d3d, "tank", "tank/tank.fbx");
  p1.GetScale() = Vector3(.15f, .15f, .15f);
  p1.GetPosition() = Vector3(-2, ground.GetPosition().x, 0);
  p1.GetRotation() = Vector3(PI / 2, PI / 2, 0);
  p1.Initialise(VK_W, VK_S, VK_A, VK_D);
  // Loads the tank1.dds texture file into the material
  mat.texTrsfm.scale = Vector2(1);
  mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "tank/tank1.dds");
  mat.texture = "tank/tank1.dds";
  // Replaces the previous material with the new one
  p1.GetModel().SetOverrideMat(&mat);
  mGameObjects[Modelid::PLAYER1] = &p1;
  mLoadData.loadedSoFar++;

  // Initialises a second tank object by copying the p1 tank to p2
  Tank p2 = p1;
  p2.GetScale() = Vector3(.15f, .15f, .15f);
  p2.GetPosition() = Vector3(2, ground.GetPosition().x, 0);
  p2.GetRotation() = Vector3(PI / 2, -PI / 2, 0);
  p2.Initialise(VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT);
  // Loads the tank2.dds texture file into the material
  mat.texTrsfm.scale = Vector2 (1);
  mat.pTextureRV = d3d.GetCache().LoadTexture(&d3d.GetDevice(), "tank/tank2.dds");
  mat.texture = "tank/tank2.dds";
  // Replaces the previous material with the new one
  p2.GetModel().SetOverrideMat(&mat);
  mGameObjects[Modelid::PLAYER2] = &p2;
  mLoadData.loadedSoFar++;

  d3d.GetFX().SetupDirectionalLight(0, true, Vector3(-1.4f, -1.4f, 1.4f), Vector3(0.47f, 0.47f, 0.47f), Vector3(0.15f, 0.15f, 0.15f), Vector3(0.25f, 0.25f, 0.25f));
}

void GameState::Initialise()
{
  input.Initialise(WinUtil::Get().GetMainWnd(), true, false);

  D3D& d3d = WinUtil::Get().GetD3D();

  pFontBatch = new SpriteBatch(&d3d.GetDeviceCtx());
  assert(pFontBatch);
  // Loads a font into the SpriteFont
  pFont = new SpriteFont(&d3d.GetDevice(), L"../bin/data/fonts/algerian.spritefont");
  assert(pFont);

  // Sets the total number of objects to load
  mLoadData.totalToLoad = (int)Modelid::TOTAL;
  mLoadData.loadedSoFar = 0;
  mLoadData.running = true;
  // Loads the objects using multiple threads
  mLoadData.loader = std::async(std::launch::async, &GameState::Load, this);
}

void GameState::Update(float dTime)
{
  // If the object data hasn't fully loaded then don't update
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

  for(int i = 0; i < mGameObjects.size(); i++)
  {
    mGameObjects[i]->Update(dTime);
  }
  
  Tank* p1;
  // Attempts to cast the tank from GameObject to Tank object
  if(p1 = dynamic_cast<Tank*>(mGameObjects[Modelid::PLAYER1]));
    //p1->Input(input, dTime);

  Tank* p2;
  if(p2 = dynamic_cast<Tank*>(mGameObjects[Modelid::PLAYER2]));
    //p2->Input(input, dTime);

  if(Collisions::PointInsideSphere(p1->GetForwardPoint(), p2->GetBoundingSphere()))
  {
    p1->Input(input, dTime, false, true);
  }
  else if(Collisions::PointInsideSphere(p1->GetBackwardPoint(), p2->GetBoundingSphere()))
  {
    p1->Input(input, dTime, true, false);
  }
  else
  {
    p1->Input(input, dTime);
  }

  if(Collisions::PointInsideSphere(p2->GetForwardPoint(), p1->GetBoundingSphere()))
  {
    p2->Input(input, dTime, false, true);
  }
  else if(Collisions::PointInsideSphere(p2->GetBackwardPoint(), p1->GetBoundingSphere()))
  {
    p2->Input(input, dTime, true, false);
  }
  else
  {
    p2->Input(input, dTime);
  }
}

void GameState::Render(float dTime)
{
  // If the object data hasn't fully loaded then don't update
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

  // Renders the gameobjects inside the vector of objects
  for (GameObject*& obj : mGameObjects)
    obj->Render(d3d, dTime);

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
    break;
    // Respond to a keyboard event.
  case WM_CHAR:
    switch (wParam)
    {
    case VK_1:
      if (Tank* obj = dynamic_cast<Tank*>(mGameObjects[Modelid::PLAYER1]))
        obj->IsActive() = !obj->IsActive();
      return 0;
    case VK_2:
      if (Tank* obj = dynamic_cast<Tank*>(mGameObjects[Modelid::PLAYER2]))
        obj->IsActive() = !obj->IsActive();
      return 0;
    case VK_ESCAPE:
      // Adds a new PauseState over the current state. When PauseState is removed it will transition back to this state.
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

  // Deletes the allocated memory for the SpriteBatch
  delete pFontBatch;
  pFontBatch = nullptr;

  // Deletes the allocated memory for the SpriteFont
  delete pFont;
  pFont = nullptr;
}
