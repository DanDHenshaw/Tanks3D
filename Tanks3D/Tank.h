#pragma once
#include "GameObject.h"

#include "Input.h"

class Tank : public	GameObject3D
{
public:
	using GameObject3D::GameObject3D;

	void Update(float dTime) override;

	void Input(MouseAndKeys input, float dTime, unsigned short movUP = VK_W, unsigned short movDOWN = VK_S, unsigned short rotLEFT = VK_A, unsigned short rotRight = VK_D);

private:
	float MOV_SPEED = 2.5f;
	float ROT_SPEED = 5.0f;

private:
	void Initialise(D3D& d3d, std::string fileName) override;
};

