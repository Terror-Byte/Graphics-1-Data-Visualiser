#include "OGLPieSegment.h"
#include "GraphItems.h"
#include <Windows.h>
#include <gl/GL.h>
#include <iostream>

OGLPieSegment::OGLPieSegment(Vector2D gPos, float sAngle, float eAngle, float rad, graphItem gItem, float zVal)
{
	graphPos = gPos;
	startAngle = sAngle;
	endAngle = eAngle;
	radius = rad;
	graphData = gItem;
	zoomVal = zVal;

	/*segmentColour.red = 3.0 / (rand() % 100);
	segmentColour.green = 3.0 / (rand() % 100);
	segmentColour.blue = 3.0 / (rand() % 100);*/
}

OGLPieSegment::~OGLPieSegment()
{

}

void OGLPieSegment::Render()
{
	float x;
	float y;

	glBegin(GL_TRIANGLE_FAN);
	if (!selected)
		glColor3f(graphData.colour.red, graphData.colour.green, graphData.colour.blue);
	else if (selected)
		glColor3f(1.0f, 1.0f, 1.0f);

	glVertex2f(graphPos.GetX(), graphPos.GetY());

	for (float i = startAngle; i <= endAngle; i += 0.001)
	{
		x = graphPos.GetX() + (radius * cos(i));
		y = graphPos.GetY() + (radius * sin(i));
		glVertex2f(x, y);
	}
	glEnd();
}

bool OGLPieSegment::MouseMove(int x, int y)
{
	return true;
}

bool OGLPieSegment::MouseLBUp(int x, int y)
{
	if (selected)
	{
		selected = false;
	}
	return true;
}

bool OGLPieSegment::MouseLBDown(int x, int y)
{
	if (!selected)
	{
		//for (float i = startAngle; i <= endAngle; i += 0.001)
		//{
		//	pointX = graphPos.GetX() + (radius * cos(i));
		//	pointY = graphPos.GetY() + (radius * sin(i));
		//	
		//	if (x >= (graphPos.GetX() * zoomVal) && x <= (pointX * zoomVal) && y >= (graphPos.GetY() * zoomVal) && y <= (pointY * zoomVal))
		//	{
		//		selected = true;
		//		std::cout << graphData.data << ": " << graphData.occurency << std::endl;
		//		break;
		//	}
		//}
		float xFromCentre = x - (graphPos.GetX() * zoomVal);
		float yFromCentre = y - (graphPos.GetY() * zoomVal);
		
		if (abs(xFromCentre) <= (radius * zoomVal) && abs(yFromCentre) <= (radius * zoomVal))
		{
			float result = atan2(yFromCentre, xFromCentre);
			if (result < 0)
				result = (2 * PI) + result;
			if (result >= startAngle && result <= endAngle)
			{
				selected = true;
				return true;
			}
		}
	}
	return false;
}