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

	virtual DirectX::SimpleMath::Vector3& GetPosition() = 0;
	virtual DirectX::SimpleMath::Vector2& GetPosition2D() = 0;

	virtual DirectX::SimpleMath::Vector3& GetRotation() = 0;
	virtual float& GetRotation2D() = 0;

	virtual DirectX::SimpleMath::Vector3& GetScale() = 0;
	virtual DirectX::SimpleMath::Vector2& GetScale2D() = 0;
private:
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

	virtual DirectX::SimpleMath::Vector3& GetPosition() override { return mModel.GetPosition(); }
	virtual DirectX::SimpleMath::Vector3& GetRotation() override { return mModel.GetRotation(); }
	virtual DirectX::SimpleMath::Vector3& GetScale() override { return mModel.GetScale(); }
private:
	virtual void Initialise(D3D& d3d, std::string fileName) override;

public:
	DirectX::SimpleMath::Vector2& GetPosition2D() override { return DirectX::SimpleMath::Vector2(GetPosition().x, GetPosition().y); }
	float& GetRotation2D() override { return GetRotation().x; }
	DirectX::SimpleMath::Vector2& GetScale2D() override { return DirectX::SimpleMath::Vector2(GetScale().x, GetScale().y); }

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

	virtual DirectX::SimpleMath::Vector2& GetPosition2D() override { return mSprite.mPos; }
	virtual float& GetRotation2D() override { return mSprite.rotation; }
	virtual DirectX::SimpleMath::Vector2& GetScale2D() override { return mSprite.GetScale(); }
private:
	virtual void Initialise(D3D& d3d, std::string fileName) override;

public:
	DirectX::SimpleMath::Vector3& GetPosition() override { return DirectX::SimpleMath::Vector3(GetPosition2D().x, GetPosition2D().y, 0); }
	DirectX::SimpleMath::Vector3& GetRotation() override { return DirectX::SimpleMath::Vector3(0, 0, 0); }
	DirectX::SimpleMath::Vector3& GetScale() override { return DirectX::SimpleMath::Vector3(GetScale2D().x, GetScale2D().y, 0); }

private:
	Sprite mSprite;
};