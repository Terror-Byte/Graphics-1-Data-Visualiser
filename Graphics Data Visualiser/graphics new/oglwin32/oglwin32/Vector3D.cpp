#include "Vector3D.h"

// Constructors

Vector3D::Vector3D(void)
{
	Init(0.0f, 0.0f, 0.0f);
}

Vector3D::Vector3D(float x, float y, float z)
{
	Init(x, y, z);
}

Vector3D::Vector3D(const Vector3D& v)
{
	Copy(v);
}

Vector3D::~Vector3D(void)
{

}

// Accessors and Mutators

float Vector3D::GetX() const
{
	return x;
}

float Vector3D::GetY() const
{
	return y;
}

float Vector3D::GetZ() const
{
	return z;
}

void Vector3D::SetX(float xIn)
{
	x = xIn;
}

void Vector3D::SetY(float yIn)
{
	y = yIn;
}

void Vector3D::SetZ(float zIn)
{
	z = zIn;
}

// Private Methods

void Vector3D::Init(float xIn, float yIn, float zIn)
{
	x = xIn;
	y = yIn;
	z = zIn;
}

void Vector3D::Copy(const Vector3D& v)
{
	x = v.GetX();
	y = v.GetY();
	z = v.GetZ();
}
