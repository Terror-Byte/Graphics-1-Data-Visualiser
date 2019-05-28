#include "OGLRectangle.h"
#include <Windows.h>
#include <gl/GL.h>
#include <iostream>
#include <ctime>

OGLRectangle::OGLRectangle()
{
	//init the OGLRectangle to a fixed size
	m_topleft.SetX(-50.0f);
	m_topleft.SetY(50.0f);

	m_bottomright.SetX(50.0f);
	m_bottomright.SetY(-50.0f);

	selected = false;
	segmentLinked = false;
}

OGLRectangle::OGLRectangle(float topLeftX, float topLeftY, float botRightX, float botRightY, OGLPieSegment* pieSeg, float zVal)
{
	m_topleft = { topLeftX, topLeftY };
	m_bottomright = { botRightX, botRightY };

	pieSegment = pieSeg;
	graphData = pieSegment->GetGraphItem();
	zoomVal = zVal;

	selected = false;
	segmentLinked = true;
}

OGLRectangle::OGLRectangle(float topLeftX, float topLeftY, float botRightX, float botRightY, graphItem itemIn, float zVal)
{
	m_topleft.SetX(topLeftX);
	m_topleft.SetY(topLeftY);

	m_bottomright.SetX(botRightX);
	m_bottomright.SetY(botRightY);

	graphData = itemIn;
	pieSegment = NULL;
	zoomVal = zVal;

	selected = false;
	segmentLinked = false;
}

OGLRectangle::~OGLRectangle()
{

}

void OGLRectangle::Render()
{
	float tlx = m_topleft.GetX();
	float tly = m_topleft.GetY();
	float brx = m_bottomright.GetX();
	float bry = m_bottomright.GetY();

	glBegin(GL_TRIANGLE_STRIP);
	if (!segmentLinked)
	{
		if (!selected)
			glColor3f(graphData.colour.red, graphData.colour.green, graphData.colour.blue);
		else if (selected)
			glColor3f(1.0f, 1.0f, 1.0f);
	}
	else if (segmentLinked)
	{
		if (!pieSegment->GetSelected())
			glColor3f(graphData.colour.red, graphData.colour.green, graphData.colour.blue);
		else if (pieSegment->GetSelected())
			glColor3f(1.0f, 1.0f, 1.0f);
	}
	

	glVertex2f(tlx, bry);
	glVertex2f(brx, bry);
	glVertex2f(tlx, tly);
	glVertex2f(brx, tly);

	glEnd();
}

bool OGLRectangle::MouseMove( int x, int y )
{	
	return true;
}

bool OGLRectangle::MouseLBUp( int x, int y )
{
	if (selected)
	{
		selected = false;
	}
	return true;
}
bool OGLRectangle::MouseLBDown( int x, int y )
{
	if (x >= (m_topleft.GetX() * zoomVal) && x <= (m_bottomright.GetX()* zoomVal) && y <= (m_topleft.GetY() * zoomVal) && y >= (m_bottomright.GetY() * zoomVal) && !selected && !segmentLinked)
	{
		std::cout << graphData.data << ": " << graphData.occurency << std::endl;
		selected = true;
		return true;
	}
	return false;
}

graphItem OGLRectangle::GetGraphItem()
{
	return graphData;
}