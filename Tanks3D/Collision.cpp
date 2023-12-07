#include "Collision.h"

#include <algorithm>

using namespace DirectX;
using namespace DirectX::SimpleMath;

float Collisions::GetDistance(DirectX::SimpleMath::Vector3 point, BoundingSphere sphere)
{
  return sqrt(
    (point.x - sphere.pos.x) * (point.x - sphere.pos.x) +
    (point.y - sphere.pos.y) * (point.y - sphere.pos.y) +
    (point.z - sphere.pos.z) * (point.z - sphere.pos.z)
  );
}

float Collisions::GetDistance(BoundingSphere a, BoundingSphere b)
{
  return sqrt(
    (a.pos.x - b.pos.x) * (a.pos.x - b.pos.x) +
    (a.pos.y - b.pos.y) * (a.pos.y - b.pos.y) +
    (a.pos.z - b.pos.z) * (a.pos.z - b.pos.z)
  );
}

// BoundingBox collision
bool Collisions::PointInsideAABB(DirectX::SimpleMath::Vector3 point, BoundingBox box)
{
  return (
    point.x >= box.pos.x - box.min.x &&
    point.x <= box.pos.x + box.max.x &&
    point.y >= box.pos.y - box.min.y &&
    point.y <= box.pos.y + box.max.y &&
    point.z >= box.pos.z - box.min.z &&
    point.z <= box.pos.z + box.max.z
  );
}

bool Collisions::Intersect(BoundingBox a, BoundingBox b)
{
  return (
    a.pos.x - a.min.x >= b.pos.x - b.min.x &&
    a.pos.x + a.max.x <= b.pos.x + b.max.x &&
    a.pos.y - a.min.y >= b.pos.y - b.min.y &&
    a.pos.y + a.max.y <= b.pos.y + b.max.y &&
    a.pos.z - a.min.z >= b.pos.z - b.min.z &&
    a.pos.z + a.max.z <= b.pos.z + b.max.z
  );
}

// Sphere collision
bool Collisions::PointInsideSphere(DirectX::SimpleMath::Vector3 point, BoundingSphere sphere)
{
  return GetDistance(point, sphere) < sphere.radius;
}

bool Collisions::Intersect(BoundingSphere a, BoundingSphere b)
{
  return GetDistance(a, b) < a.radius + b.radius;
}

// Bounding Sphere collision vs BoundingBox collision
bool Collisions::Intersect(BoundingBox box, BoundingSphere sphere)
{
  float x = std::fmax(box.pos.x - box.min.x, std::fmin(sphere.pos.x, box.pos.x + box.max.x));
  float y = std::fmax(box.pos.y - box.min.y, std::fmin(sphere.pos.y, box.pos.y + box.max.y));
  float z = std::fmax(box.pos.z - box.min.z, std::fmin(sphere.pos.z, box.pos.z + box.max.z));

  return GetDistance(Vector3(x, y, z), sphere) < sphere.radius;
}