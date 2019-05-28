#include "OGLLinePoint3D.h"
#include <Windows.h>
#include <gl/GL.h>

OGLLinePoint3D::OGLLinePoint3D(float x, float y, float z, graphItem3E itemIn, float zVal)
{
	point.SetX(x);
	point.SetY(y);
	point.SetZ(z);
	graphData = itemIn;
	zoomVal = zVal;
}

OGLLinePoint3D::~OGLLinePoint3D()
{

}

void OGLLinePoint3D::Render()
{
	glBegin(GL_QUADS);
	glColor3f(graphData.colour.red, graphData.colour.green, graphData.colour.blue);

	// Front Side
	glVertex3f(point.GetX() + 2, point.GetY() + 2, point.GetZ() + 2);
	glVertex3f(point.GetX() - 2, point.GetY() + 2, point.GetZ() + 2);
	glVertex3f(point.GetX() - 2, point.GetY() - 2, point.GetZ() + 2);
	glVertex3f(point.GetX() + 2, point.GetY() - 2, point.GetZ() + 2);

	// Back Side
	glVertex3f(point.GetX() + 2, point.GetY() + 2, point.GetZ() - 2);
	glVertex3f(point.GetX() - 2, point.GetY() + 2, point.GetZ() - 2);
	glVertex3f(point.GetX() - 2, point.GetY() - 2, point.GetZ() - 2);
	glVertex3f(point.GetX() + 2, point.GetY() - 2, point.GetZ() - 2);

	// Left Side
	glVertex3f(point.GetX() - 2, point.GetY() + 2, point.GetZ() + 2);
	glVertex3f(point.GetX() - 2, point.GetY() + 2, point.GetZ() - 2);
	glVertex3f(point.GetX() - 2, point.GetY() - 2, point.GetZ() + 2);
	glVertex3f(point.GetX() - 2, point.GetY() - 2, point.GetZ() - 2);

	// Right Side
	glVertex3f(point.GetX() + 2, point.GetY() + 2, point.GetZ() + 2);
	glVertex3f(point.GetX() + 2, point.GetY() + 2, point.GetZ() - 2);
	glVertex3f(point.GetX() + 2, point.GetY() - 2, point.GetZ() + 2);
	glVertex3f(point.GetX() + 2, point.GetY() - 2, point.GetZ() - 2);

	// Top Side
	glVertex3f(point.GetX() + 2, point.GetY() + 2, point.GetZ() + 2);
	glVertex3f(point.GetX() + 2, point.GetY() + 2, point.GetZ() - 2);
	glVertex3f(point.GetX() - 2, point.GetY() + 2, point.GetZ() + 2);
	glVertex3f(point.GetX() - 2, point.GetY() + 2, point.GetZ() - 2);

	// Bottom Side
	glVertex3f(point.GetX() + 2, point.GetY() - 2, point.GetZ() + 2);
	glVertex3f(point.GetX() + 2, point.GetY() - 2, point.GetZ() - 2);
	glVertex3f(point.GetX() - 2, point.GetY() - 2, point.GetZ() + 2);
	glVertex3f(point.GetX() - 2, point.GetY() - 2, point.GetZ() - 2);

	glEnd();
}

bool OGLLinePoint3D::MouseMove(int x, int y)
{
	return true;
}

bool OGLLinePoint3D::MouseLBUp(int x, int y)
{
	return true;
}

bool OGLLinePoint3D::MouseLBDown(int x, int y)
{
	return true;
}