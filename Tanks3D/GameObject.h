#pragma once

#include "D3D.h"

#include "Model.h"
#include "Sprite.h"

// Base gameobject class
class GameObject
{
public:
	virtual ~GameObject() = default;

	virtual void Update(float dTime) = 0;
	virtual void Render(D3D& d3d, float dTime) = 0;

	virtual void Initialise(D3D& d3d, std::string fileName) = 0;
};

// 3D gameobject class
class GameObject3D : public GameObject
{
public:
	/*
	* file constructor
	* name - name of the model
	* filename - path to the model file
	*/
	GameObject3D(D3D& d3d, std::string name, std::string fileName);
	/*
	* mesh constructor
	* mesh - the mesh used to initialise the model
	*/
	GameObject3D(D3D& d3d, Mesh& mesh);

	virtual void Update(float dTime) override;
	virtual void Render(D3D& d3d, float dTime) override;

	Model& GetModel() { return mModel; }

	DirectX::SimpleMath::Vector3& GetPosition() { return mModel.GetPosition(); }
	DirectX::SimpleMath::Vector3& GetRotation() { return mModel.GetRotation(); }
	DirectX::SimpleMath::Vector3& GetScale() { return mModel.GetScale(); }
private:
	virtual void Initialise(D3D& d3d, std::string fileName) override;
private:
	// filepath Name
	const std::string mName;
	Model mModel;
};

// 2D gameobject class
class GameObject2D : public GameObject
{
public:
	/*
	* filename - path to the texture file
	* batch - the spritebatch used to render the sprite
	*/
	GameObject2D(D3D& d3d, std::string fileName, DirectX::SpriteBatch* batch);

	virtual void Update(float dTime) override;
	void Render(D3D& d3d, float dTime) override;

	DirectX::SimpleMath::Vector2& GetPosition() { return mSprite.mPos; }
	float& GetRotation() { return mSprite.rotation; }
	DirectX::SimpleMath::Vector2& GetScale() { return mSprite.GetScale(); }
private:
	virtual void Initialise(D3D& d3d, std::string fileName) override;
private:
	Sprite mSprite;

	DirectX::SpriteBatch* mBatch;
};