#include "Tank.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Tank::Update(float dTime)
{
	GameObject3D::Update(dTime);
}

void Tank::Initialise()
{
	acceleration = 0.0f;
	accel_ot = 1.0f;

	max_fwd_speed = 1.0f;
	max_bwd_speed = 0.5f;
	rot_speed = 1.5f;
}

void Tank::Input(MouseAndKeys input, float dTime, unsigned short movUP, unsigned short movDOWN, unsigned short rotLEFT, unsigned short rotRight)
{
	bool keypressed = input.IsPressed(movUP) || input.IsPressed(movDOWN) ||
		input.IsPressed(rotRight) || input.IsPressed(rotLEFT);

	if (keypressed)
	{
		Vector3 pos(0, 0, 0);
		Vector3 rot(0, 0, 0);

		if (input.IsPressed(rotLEFT))
		{
			rot.y -= rot_speed;
		}
		else if (input.IsPressed(rotRight))
		{
			rot.y += rot_speed;
		}

		float angle = GetRotation().y + PI / 2;
		if (input.IsPressed(movUP))
		{
			acceleration += accel_ot * dTime;
			if (acceleration >= max_fwd_speed) acceleration = max_fwd_speed;

			pos.x -= acceleration * cos(angle);
			pos.z += acceleration * sin(angle);
		}
		else if (input.IsPressed(movDOWN))
		{
			acceleration += accel_ot * dTime;
			if (acceleration >= max_bwd_speed) acceleration = max_bwd_speed;

			pos.x += acceleration * cos(angle);
			pos.z -= acceleration * sin(angle);
		} else
		{
			acceleration = 0.0f;
		}

		GetPosition() += pos * dTime;
		GetRotation() += rot * dTime;
	}
	else {
		acceleration = 0.0f;
	}
}
