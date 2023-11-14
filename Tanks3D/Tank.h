#pragma once
#include "GameObject.h"

class Tank : public	GameObject3D
{
public:
	void Update(float dTime) override;

private:
	void Initialise(D3D& d3d, std::string fileName) override;
};

