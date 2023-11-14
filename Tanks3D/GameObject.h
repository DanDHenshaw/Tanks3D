#pragma once

#include "D3D.h"

#include "Model.h"
#include "Sprite.h"

class GameObject
{
public:
	virtual void Update() = 0;
	virtual void Render() = 0;

	DirectX::SimpleMath::Vector3& GetPosition() { return mPosition; }
private:
	virtual void Initialise(D3D& d3d, std::string fileName) = 0;
private:
	DirectX::SimpleMath::Vector3 mPosition;
};

class GameObject3D : public GameObject
{
public:
	GameObject3D(D3D& d3d, std::string name, std::string fileName);

	void Update() override;
	void Render() override;

	DirectX::SimpleMath::Vector3& GetRotation() { return mRotation; }
	DirectX::SimpleMath::Vector3& GetScale() { return mScale; }
private:
	virtual void Initialise(D3D& d3d, std::string fileName) override;
private:
	const std::string mName;
	Model mModel;

	DirectX::SimpleMath::Vector3 mRotation, mScale;
};

class GameObject2D : public GameObject
{
public:
	GameObject2D(D3D& d3d, std::string fileName);

	void Update() override;
	void Render() override;

	float& GetRotation() { return mRotation; }
	DirectX::SimpleMath::Vector2& GetScale() { return mScale; }
private:
	virtual void Initialise(D3D& d3d, std::string fileName) override;
private:
	Sprite mSprite;

	float mRotation = 0;
	DirectX::SimpleMath::Vector2 mScale;
};