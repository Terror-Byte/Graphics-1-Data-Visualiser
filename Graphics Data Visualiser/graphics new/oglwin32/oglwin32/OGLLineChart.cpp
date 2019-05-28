#include "OGLLineChart.h"
#include <Windows.h>
#include <gl/GL.h>

OGLLineChart::OGLLineChart(int viewWidth, int viewHeight, Vector2D graphPos) : Graph(viewWidth, viewHeight, graphPos)
{
	graphVector.clear();
	pointVector.clear();
	SetAxis();
	yAxisLabel->SetText("Frequency");
}

OGLLineChart::~OGLLineChart()
{
	
}

void OGLLineChart::Render()
{
	if (graphInitialised)
	{
		glScalef(zoomVal, zoomVal, zoomVal);
		SetAxis();
		if (!pointsCreated)
			CreatePoints();
		if (pointsCreated)
			UpdatePoints();

		glBegin(GL_LINES);	
		glColor3f(0.0, 0.0, 0.7);
		Vector2D point1;
		Vector2D point2;
		for (int i = 0; i < pointVector.size(); i++)
		{
			if (i != pointVector.size() - 1)
			{
				point1 = pointVector[i]->GetPoint();
				point2 = pointVector[i + 1]->GetPoint();
				glVertex2f(point1.GetX(), point1.GetY());
				glVertex2f(point2.GetX(), point2.GetY());
			}
		}

		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex2f(graphPos.GetX(), graphPos.GetY());
		glVertex2f(xAxisTop.GetX(), xAxisTop.GetY());
		glVertex2f(graphPos.GetX(), graphPos.GetY());
		glVertex2f(yAxisTop.GetX(), yAxisTop.GetY());

		int textIndex = 0;
		for (float y = graphPos.GetY(); y <= yAxisTop.GetY(); y += yAxisInterval)
		{
			if (textIndex < yAxisVector.size())
			{
				yAxisVector[textIndex]->SetX(graphPos.GetX() - 15);
				yAxisVector[textIndex]->SetY(y);
				textIndex++;
			}

			// Draws notches on Y axis
			glVertex2f((graphPos.GetX() - 10), y);
			glVertex2f(graphPos.GetX(), y);
		}

		textIndex = 0;
		for (float x = graphPos.GetX() + xAxisInterval; x <= xAxisTop.GetX(); x += xAxisInterval)
		{
			if (textIndex < xAxisVector.size())
			{
				xAxisVector[textIndex]->SetX(x);
				xAxisVector[textIndex]->SetY(graphPos.GetY() - 5);
				textIndex++;
			}
			// Draws notches on X axis
			glVertex2f(x, (graphPos.GetY() - 10));
			glVertex2f(x, graphPos.GetY());
		}

		glEnd();

		// Loops through text vector and renders them
		for (int i = 0; i < yAxisVector.size(); i++)
		{
			yAxisVector[i]->Render();
		}

		for (int i = 0; i < xAxisVector.size(); i++)
		{
			xAxisVector[i]->Render();
		}

		for (int i = 0; i < pointVector.size(); i++)
		{
			pointVector[i]->Render();
		}

		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glScalef(5.0f, 5.0f, 5.0f);

		yAxisLabel->SetX((graphPos.GetX() - 90) / 5.0f);
		yAxisLabel->SetY((graphPos.GetY() + (graphHeight / 2)) / 5.0f);
		xAxisLabel->SetX((graphPos.GetX() + (graphWidth / 2)) / 5.0f);
		xAxisLabel->SetY((graphPos.GetY() - 50) / 5.0f);

		yAxisLabel->Render();
		xAxisLabel->Render();
		glPopMatrix();

		if (renderSelected)
		{
			glPushMatrix();
			glScalef(3.0f, 3.0f, 3.0f);

			line1->SetX((currentMousePos.GetX() / (zoomVal * 3)) + 2);
			line1->SetY((currentMousePos.GetY() / (zoomVal * 3)) - 2);

			glColor3f(1.0f, 1.0f, 1.0f);
			line1->Render();
			glPopMatrix();
		}
	}
}

bool OGLLineChart::MouseLBUp(int x, int y)
{
	if (mouseClicked)
	{
		mouseClicked = false;

		for (int i = 0; i < pointVector.size(); i++)
		{
			pointVector[i]->MouseLBUp(x, y);
			renderSelected = false;
		}
		return true;		
	}
	return false;
}

bool OGLLineChart::MouseLBDown(int x, int y)
{
	if (!mouseClicked)
	{
		mouseClicked = true;
		for (int i = 0; i < pointVector.size(); i++)
		{
			if (pointVector[i]->MouseLBDown(x, y))
			{
				char occurencyString[200];
				char* occurency;
				occurency = &occurencyString[0];
				itoa(pointVector[i]->GetGraphItem().occurency, occurency, 10);
				std::string text = "Item: " + pointVector[i]->GetGraphItem().data + ", Occurency: " + occurency;

				line1->SetText(text.c_str());
				line1->SetX(x);
				line1->SetY(y);

				renderSelected = true;
			}
		}
		return true;
	}
	return false;
}

void OGLLineChart::SetAxis()
{
	xAxisTop.SetX((graphPos.GetX() + (vWidth - 200)));
	xAxisTop.SetY(graphPos.GetY());

	yAxisTop.SetX(graphPos.GetX());
	yAxisTop.SetY((graphPos.GetY() + (vHeight - 200)));
}

void OGLLineChart::GraphInitialise()
{
	graphHeight = 0;
	graphWidth = 0;
	totalOccurency = 0;

	for (int i = 0; i < graphVector.size(); i++)
	{
		totalOccurency += graphVector[i].occurency;
		if (maxOccurency == 0 || maxOccurency < graphVector[i].occurency)
			maxOccurency = graphVector[i].occurency;
	}

	graphHeight = yAxisTop.GetY() - graphPos.GetY();
	graphWidth = xAxisTop.GetX() - graphPos.GetX();

	xAxisInterval = graphWidth / (graphVector.size() - 1);

	float a = 0;
	float b = graphHeight;
	float A = 0;
	float B = maxOccurency;
	double  x = 1;
	// (a + ((x - a) * (b - a)))/ (B - A)
	singleInterval = (a + ((x - a) * (b - a))) / (B - A);
	x = maxOccurency / 20;
	yAxisInterval = (a + ((x - a) * (b - a))) / (B - A);

	// Initialises Y axis labels
	yAxisVector.clear();
	GUIText* intervalText;
	char interval[100];
	int intervalNo = 0;
	for (float y = graphPos.GetY(); y <= yAxisTop.GetY(); y += yAxisInterval)
	{
		if (y == graphPos.GetY())
		{
			intervalText = new GUIText("0", graphPos.GetX() - 55, y);
		}
		else if (y != graphPos.GetY())
		{
			itoa(((yAxisInterval * intervalNo) / singleInterval), interval, 10);		
			intervalText = new GUIText(interval, graphPos.GetX() - 15, y);
		}
		intervalNo++;
		yAxisVector.push_back(intervalText);
	}

	xAxisVector.clear();
	intervalNo = 1;
	for (float x = graphPos.GetX(); x < xAxisTop.GetX(); x += xAxisInterval)
	{
		if (intervalNo < graphVector.size())
		{
			intervalText = new GUIText((char*)graphVector[intervalNo].data.c_str(), x, graphPos.GetY() - 5);
			xAxisVector.push_back(intervalText);
		}
		intervalNo++;
	}
	graphInitialised = true;
}

void OGLLineChart::CreatePoints()
{
	OGLLinePoint* newPoint;
	pointVector.clear();

	xAxisLabel->SetText(graphVector[0].data.c_str());

	int i = 1; // Point Vector Index
	for (float x = graphPos.GetX() + xAxisInterval; x <= xAxisTop.GetX(); x += xAxisInterval)
	{
		if (i < graphVector.size())
		{
			float y = graphPos.GetY() + (graphVector[i].occurency * singleInterval);

			newPoint = new OGLLinePoint(x, y, graphVector[i], zoomVal);
			pointVector.push_back(newPoint);		
			i++;
		}
	}
	pointsCreated = true;
}

void OGLLineChart::UpdatePoints()
{
	int graphVectorIndex = 1;
	int pointVectorIndex = 0;

	for (float x = graphPos.GetX() + xAxisInterval; x <= xAxisTop.GetX(); x += xAxisInterval)
	{
		if (graphVectorIndex < graphVector.size())
		{
			int y = graphPos.GetY() + (graphVector[graphVectorIndex].occurency * singleInterval);
			Vector2D* newPos = new Vector2D(x, y);
			pointVector[pointVectorIndex]->SetPoint(*newPos);
			pointVector[pointVectorIndex]->SetZoomVal(zoomVal);
		}
		graphVectorIndex++;
		pointVectorIndex++;
	}
}

void OGLLineChart::CentreGraph()
{
	graphPos.SetX(CoordConvertX(100));
	graphPos.SetY(CoordConvertY(vHeight - 100));
	SetAxis();
}

void OGLLineChart::ClearGraph()
{
	graphVector.clear();
	pointVector.clear();
	xAxisVector.clear();
	yAxisVector.clear();

	maxOccurency = 0;
	xAxisInterval = 0;
	yAxisInterval = 0;
	singleInterval = 0;
	totalOccurency = 0;
	graphInitialised = false;
	pointsCreated = false;
}

