#include "Bullet.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Bullet::Initialise(Vector3 pos, Vector3 rot)
{
  GetPosition() = pos;
  GetPosition().y += 0.4f;
  GetRotation() = rot;

  // Movement
  acceleration = 0.0f;
  accel_ot = 10.0f;

  speed = 1.0f;
}

void Bullet::Update(float dTime)
{
  if (!IsActive()) return;

  GameObject3D::Update(dTime);

  Vector3 pos(0, 0, 0);

  // Increase acceleration by accel_ot * dTime
  acceleration += accel_ot * dTime;
  // Don't increase the acceleration if the speed is faster than the max forward speed
  if (acceleration >= speed) acceleration = speed;

  float angle = GetRotation().y + PI / 2;
  // Remove acceleration * cos(angle) from the position x 
  pos.x -= acceleration * cos(angle);
  // Add acceleration * sin(angle) from the position z
  pos.z += acceleration * sin(angle);

  // Set position
  GetPosition() += pos * dTime;

  collision.UpdatePos(GetPosition());

  // Gets the rotation + 90 degrees to account for the models rotation
  point = GetPosition();
  point.x -= 0.25f * cos(angle);
  point.z += 0.25f * sin(angle);
}