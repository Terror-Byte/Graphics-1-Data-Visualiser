#include "OGLLinePoint.h"
#include <Windows.h>
#include <gl/GL.h>
#include <iostream>

OGLLinePoint::OGLLinePoint(float x, float y, graphItem itemIn, float zVal)
{
	point.SetX(x);
	point.SetY(y);
	graphData = itemIn;
	zoomVal = zVal;
	selected = false;
	twoElements = false;
}

OGLLinePoint::OGLLinePoint(float x, float y, graphItem2E itemIn, float zVal)
{
	point.SetX(x);
	point.SetY(y);
	graphData2E = itemIn;
	zoomVal = zVal;
	selected = false;
	twoElements = true;
}

void OGLLinePoint::Render()
{
	if (!twoElements && !selected)
	{
		glColor3f(graphData.colour.red, graphData.colour.green, graphData.colour.blue);
	}
	else if (twoElements && !selected)
	{
		glColor3f(graphData2E.colour.red, graphData2E.colour.green, graphData2E.colour.blue);
	}
	else if (selected)
	{
		glColor3f(1.0, 1.0, 1.0);
	}
	glBegin(GL_TRIANGLE_STRIP);
	glVertex2f(point.GetX() - 2, point.GetY());
	glVertex2f(point.GetX(), point.GetY() - 2);
	glVertex2f(point.GetX(), point.GetY() + 2);
	glVertex2f(point.GetX() + 2, point.GetY());
	glEnd();
}

bool OGLLinePoint::MouseMove(int x, int y)
{
	return true;
}

bool OGLLinePoint::MouseLBUp(int x, int y)
{
	if (selected)
	{
		selected = false;
	}
	return true;
}

bool OGLLinePoint::MouseLBDown(int x, int y)
{
	if (x <= (point.GetX() + 2) * zoomVal && x >= (point.GetX() - 2) * zoomVal && y <= (point.GetY() + 2) * zoomVal && y >= (point.GetY() - 2) * zoomVal && !selected)
	{
		selected = true;
		if (!twoElements)
		{
			std::cout << graphData.data << ": " << graphData.occurency << std::endl;
		}
		else
		{
			std::cout << graphData2E.data1 << " " << graphData2E.data2 << ": " << graphData2E.occurency << std::endl;
		}
		return true;
	}
	return false;
}