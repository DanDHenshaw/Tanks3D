#pragma once

#include "Sprite.h"

class Button
{
public:
  Button::Button(D3D& d3d);

  bool IsMouseOverButton(DirectX::SimpleMath::Vector2 mousePos);

  void ButtonHover(bool isHovering);

public:
  Sprite sprite;

  DirectX::SimpleMath::Vector2 ogScale;
};