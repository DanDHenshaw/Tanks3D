#include "Button.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

bool Button::IsMouseOverButton(DirectX::SimpleMath::Vector2 mousePos)
{
  Vector2 halfDim = GetSprite().GetTexData().dim / 2;

  Vector4 dimToScreen;
  dimToScreen.x = GetPosition().x - halfDim.x;
  dimToScreen.y = GetPosition().y - halfDim.y;
  dimToScreen.z = GetPosition().x + halfDim.x;
  dimToScreen.w = GetPosition().y + halfDim.y;

  if ((mousePos.x > dimToScreen.x && mousePos.x < dimToScreen.z) &&
    mousePos.y > dimToScreen.y && mousePos.y < dimToScreen.w) 
  {
    return true;
  }

  return false;
}

void Button::ButtonHover(bool isHovering)
{
  if (isHovering)
  {
    GetScale() = ogScale * 1.05f;
  }
  else {
    GetScale() = ogScale;
  }
}

void Button::Initialise(DirectX::SimpleMath::Vector2 pos)
{
  GameObject2D::Initialise(pos);

  ogScale = GetScale();
}
