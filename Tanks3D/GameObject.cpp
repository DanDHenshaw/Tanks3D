#include "GameObject.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Setup(Model& m, Mesh& source, const Vector3& scale, const DirectX::SimpleMath::Vector3& pos, const Vector3& rot)
{
	m.Initialise(source);
	m.GetScale() = scale;
	m.GetPosition() = pos;
	m.GetRotation() = rot;
}

void Setup(Model& m, Mesh& source, float scale, const Vector3& pos, const Vector3& rot)
{
	Setup(m, source, Vector3(scale, scale, scale), pos, rot);
}

//************************************************

GameObject3D::GameObject3D(D3D& d3d, std::string name, std::string fileName)
	: mName(name)
{
	GameObject3D::Initialise(d3d, fileName);
}

GameObject3D::GameObject3D(D3D& d3d, Mesh& mesh)
{
	// Initialises the model
	Setup(mModel, mesh, 1, Vector3(0, 0, 0), Vector3(0, 0, 0));
}

void GameObject3D::Update(float dTime)
{
	if (!IsActive()) return;
}

void GameObject3D::Initialise(D3D& d3d, std::string fileName)
{
	// Creates a mesh with object name 
	Mesh& mesh = d3d.GetMeshMgr().CreateMesh(mName);
	// Creates the mesh using the 3d object file
	mesh.CreateFrom("../bin/data/" + fileName, d3d);
	// Initialises the model
	Setup(mModel, mesh, 1, Vector3(0, 0, 0), Vector3(0, 0, 0));
}

void GameObject3D::Render(D3D& d3d, float dTime)
{
	if (!IsActive()) return;

	// Renders the model with lighting data
	d3d.GetFX().Render(mModel);
}

//************************************************

GameObject2D::GameObject2D(D3D& d3d, std::string fileName, SpriteBatch* batch)
	: mSprite(d3d), mBatch(batch)
{
	GameObject2D::Initialise(d3d, fileName);
}

void GameObject2D::Update(float dTime)
{
	if (!IsActive()) return;
}

void GameObject2D::Initialise(D3D& d3d, std::string fileName)
{
	if (!IsActive()) return;

	// Loads a texture to the sprite
	mSprite.SetTex(*d3d.GetCache().LoadTexture(&d3d.GetDevice(), fileName));
	// Sets scale and pos
	mSprite.GetScale() = Vector2(1, 1);
	mSprite.mPos = Vector2(0, 0);
}

void GameObject2D::Render(D3D& d3d, float dTime)
{
	mSprite.Draw(*mBatch);
}
