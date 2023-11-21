#pragma once
#include "GameObject.h"

#include "Input.h"

class Tank : public	GameObject3D
{
public:
	using GameObject3D::GameObject3D;

	void Update(float dTime) override;

	void Input(MouseAndKeys input, float dTime, unsigned short movUP = VK_W, unsigned short movDOWN = VK_S, unsigned short rotLEFT = VK_A, unsigned short rotRight = VK_D);

	void Initialise();
private:
	float acceleration;
	float accel_ot;

	float max_fwd_speed;
	float max_bwd_speed;
	float rot_speed;
};

