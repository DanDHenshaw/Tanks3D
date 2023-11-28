#pragma once
#include "GameObject.h"

#include "Input.h"

// Drivable tank
class Tank : public	GameObject3D
{
public:
	using GameObject3D::GameObject3D;

	void Update(float dTime) override;

	/*
	* tank controller
	* input - handle keys
	* dTime - elapsed time
	*/
	void Input(MouseAndKeys& input, float dTime);

	/*
	* up, down, left, right - keycodes for movement
	*/
	void Initialise(unsigned short up = VK_W, unsigned short down = VK_S, unsigned short left = VK_A, unsigned short right = VK_D);

private:
	// Movement variables
	unsigned short movUP = VK_W, movDOWN = VK_S, rotLEFT = VK_A, rotRIGHT = VK_D;

private:
	float acceleration;
	// acceleration increase over time
	float accel_ot;

	// max speed for forward movement
	float max_fwd_speed;
	// max speed for backwards movement
	float max_bwd_speed;
	// rotation speed;
	float rot_speed;
};

