#pragma once

#include "Sprite.h"

#include "GameObject.h"

class Button : public GameObject2D
{
public:
  using GameObject2D::GameObject2D;

  bool IsMouseOverButton(DirectX::SimpleMath::Vector2 mousePos);

  void ButtonHover(bool isHovering);

  virtual void Initialise(DirectX::SimpleMath::Vector2 pos) override;

public:
  DirectX::SimpleMath::Vector2 ogScale;
};