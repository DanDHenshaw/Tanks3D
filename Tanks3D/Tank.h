#pragma once
#include "GameObject.h"

#include "Input.h"

#include "Collision.h"

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
	void Input(MouseAndKeys& input, float dTime, bool canUp = true, bool canDown = true);

	/*
	* up, down, left, right - keycodes for movement
	*/
	void Initialise(unsigned short up = VK_W, unsigned short down = VK_S, unsigned short left = VK_A, unsigned short right = VK_D);

public:
	Collisions::BoundingSphere GetBoundingSphere() const {
		return collision;
	}

	DirectX::SimpleMath::Vector3 GetForwardPoint() const {
		return fwdPoint;
	}
	DirectX::SimpleMath::Vector3 GetBackwardPoint() const {
		return bwdPoint;
	}

private:
	// Movement variables
	unsigned short movUP = VK_W, movDOWN = VK_S, rotLEFT = VK_A, rotRIGHT = VK_D;

	float acceleration;
	// acceleration increase over time
	float accel_ot;

	// max speed for forward movement
	float max_fwd_speed;
	// max speed for backwards movement
	float max_bwd_speed;
	// rotation speed;
	float rot_speed;

private:
	Collisions::BoundingSphere collision;
	DirectX::SimpleMath::Vector3 fwdPoint;
	DirectX::SimpleMath::Vector3 bwdPoint;
};

