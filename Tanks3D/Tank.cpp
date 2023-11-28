#include "Tank.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Tank::Update(float dTime)
{
	GameObject3D::Update(dTime);
}

void Tank::Initialise(unsigned short up, unsigned short down, unsigned short left, unsigned short right)
{
	movUP = up;
	movDOWN = down;
	rotLEFT = left;
	rotRIGHT = right;

	acceleration = 0.0f;
	accel_ot = 1.0f;

	max_fwd_speed = 1.0f;
	max_bwd_speed = 0.5f;
	rot_speed = 1.5f;
}

void Tank::Input(MouseAndKeys& input, float dTime)
{
	// Checks if any of the input keys are pressed
	bool keypressed = input.IsPressed(movUP) || input.IsPressed(movDOWN) ||
		input.IsPressed(rotRIGHT) || input.IsPressed(rotLEFT);
	
	if (keypressed)
	{
		Vector3 pos(0, 0, 0);
		Vector3 rot(0, 0, 0);

		if (input.IsPressed(rotLEFT))
		{
			// Remove rotation speed from the tanks rotation
			rot.y -= rot_speed;
		}
		else if (input.IsPressed(rotRIGHT))
		{
			// Add rotation speed from the tanks rotation
			rot.y += rot_speed;
		}

		// Gets the rotation + 90 degrees to account for the models rotation
		float angle = GetRotation().y + PI / 2;
		if (input.IsPressed(movUP))
		{
			// Increase acceleration by accel_ot * dTime
			acceleration += accel_ot * dTime;
			// Don't increase the acceleration if the speed is faster than the max forward speed
			if (acceleration >= max_fwd_speed) acceleration = max_fwd_speed;

			// Remove acceleration * cos(angle) from the position x 
			pos.x -= acceleration * cos(angle);
			// Add acceleration * sin(angle) from the position z
			pos.z += acceleration * sin(angle);
		}
		else if (input.IsPressed(movDOWN))
		{
			// Increase acceleration by accel_ot * dTime
			acceleration += accel_ot * dTime;
			// Don't increase the acceleration if the speed is faster than the max backward speed
			if (acceleration >= max_bwd_speed) acceleration = max_bwd_speed;

			// Add acceleration * cos(angle) from the position x 
			pos.x += acceleration * cos(angle);
			// Remove acceleration * sin(angle) from the position z
			pos.z -= acceleration * sin(angle);
		} else
		{
			// If no input then set acceleration to 0
			acceleration = 0.0f;
		}

		// Set position and rotation
		GetPosition() += pos * dTime;
		GetRotation() += rot * dTime;
	}
	else {
		// If no input then set acceleration to 0
		acceleration = 0.0f;
	}
}
