#pragma once

#include "GameObject.h"
#include "Collision.h"

class Bullet : public	GameObject3D
{
public:
  using GameObject3D::GameObject3D;

  void Initialise(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 rot);

  void Update(float dTime) override;

public:
  Collisions::BoundingSphere GetBoundingSphere() const {
    return collision;
  }

  DirectX::SimpleMath::Vector3 GetPoint() const {
    return point;
  }

private:
  float acceleration;
  // acceleration increase over time
  float accel_ot;

  // max speed for forward movement
  float speed;

private:
  Collisions::BoundingSphere collision;
  DirectX::SimpleMath::Vector3 point;
};