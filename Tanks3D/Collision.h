#pragma once

#include "SimpleMath.h"
#include "GeometryBuilder.h"
#include "WindowUtils.h"

#include "GameObject.h"

namespace Collisions 
{
  struct BoundingBox {
    DirectX::SimpleMath::Vector3 pos;
    DirectX::SimpleMath::Vector3 min;
    DirectX::SimpleMath::Vector3 max;

    BoundingBox()
      : d3d (WinUtil::Get().GetD3D()), DebugModel(d3d, BuildCube(d3d.GetMeshMgr()))
    {
    }

    D3D& d3d;
    GameObject3D DebugModel;

    void UpdatePos(DirectX::SimpleMath::Vector3 _pos) { pos = _pos; }
  };

  struct BoundingSphere {
    DirectX::SimpleMath::Vector3 pos;
    float radius;

    void UpdatePos(DirectX::SimpleMath::Vector3 _pos) { pos = _pos; }
  };

  float GetDistance(DirectX::SimpleMath::Vector3 point, BoundingSphere sphere);
  float GetDistance(BoundingSphere a, BoundingSphere b);

  // BoundingBox collision
  bool PointInsideAABB(DirectX::SimpleMath::Vector3 point, BoundingBox box);
  bool Intersect(BoundingBox a, BoundingBox b);

  // Sphere collision
  bool PointInsideSphere(DirectX::SimpleMath::Vector3 point, BoundingSphere sphere);
  bool Intersect(BoundingSphere a, BoundingSphere b);

  // Bounding Sphere collision vs BoundingBox collision
  bool Intersect(BoundingBox box, BoundingSphere sphere);
}

