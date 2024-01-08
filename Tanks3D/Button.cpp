#include "Button.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

Button::Button(D3D& d3d)
  : sprite(d3d)
{
  ogScale = sprite.GetScale();
}

bool Button::IsMouseOverButton(DirectX::SimpleMath::Vector2 mousePos)
{
  Vector2 halfDim = sprite.GetTexData().dim / 2;

  Vector4 dimToScreen;
  dimToScreen.x = sprite.mPos.x - halfDim.x;
  dimToScreen.y = sprite.mPos.y - halfDim.y;
  dimToScreen.z = sprite.mPos.x + halfDim.x;
  dimToScreen.w = sprite.mPos.y + halfDim.y;

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
    sprite.GetScale() = ogScale * 1.05f;
  }
  else {
    sprite.GetScale() = ogScale;
  }
}
