#pragma once

#include "D3D.h"

#include "Model.h"
#include "Sprite.h"

class GameObject
{
public:
	virtual ~GameObject() = default;
	virtual void Update(float dTime) = 0;
	virtual void Render(D3D& d3d, float dTime) = 0;

	virtual void Initialise(D3D& d3d, std::string fileName) = 0;
};

class GameObject3D : public GameObject
{
public:
	GameObject3D(D3D& d3d, std::string name, std::string fileName);
	GameObject3D(D3D& d3d, Mesh& mesh);

	virtual void Update(float dTime) override;
	void Render(D3D& d3d, float dTime);

	Model& GetModel() { return mModel; }

	DirectX::SimpleMath::Vector3& GetPosition() { return mModel.GetPosition(); }
	DirectX::SimpleMath::Vector3& GetRotation() { return mModel.GetRotation(); }
	DirectX::SimpleMath::Vector3& GetScale() { return mModel.GetScale(); }
private:
	virtual void Initialise(D3D& d3d, std::string fileName) override;
private:
	const std::string mName;
	Model mModel;
};

class GameObject2D : public GameObject
{
public:
	GameObject2D(D3D& d3d, std::string fileName);

	virtual void Update(float dTime) override;
	void Render(D3D& d3d, float dTime) override;

	DirectX::SimpleMath::Vector2& GetPosition() { return mSprite.mPos; }
	float& GetRotation() { return mSprite.rotation; }
	DirectX::SimpleMath::Vector2& GetScale() { return mSprite.GetScale(); }
private:
	virtual void Initialise(D3D& d3d, std::string fileName) override;
private:
	Sprite mSprite;
};