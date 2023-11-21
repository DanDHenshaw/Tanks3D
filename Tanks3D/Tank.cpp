#include "Tank.h"

#include <cmath>

using namespace DirectX;
using namespace DirectX::SimpleMath;

void Tank::Update(float dTime)
{
	GameObject3D::Update(dTime);
}

void Tank::Initialise(D3D& d3d, std::string fileName)
{

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
			rot.y -= ROT_SPEED;
		}
		else if (input.IsPressed(rotRight))
		{
			rot.y += ROT_SPEED;
		}

		float angle = GetRotation().y + PI / 2;
		if (input.IsPressed(movUP))
		{
			pos.x -= MOV_SPEED * cos(angle);
			pos.z += MOV_SPEED * sin(angle);
		}
		else if (input.IsPressed(movDOWN))
		{
			pos.x += MOV_SPEED * cos(angle);
			pos.z -= MOV_SPEED * sin(angle);
		}

		GetPosition() += pos * dTime;
		GetRotation() += rot * dTime;
	}
}
