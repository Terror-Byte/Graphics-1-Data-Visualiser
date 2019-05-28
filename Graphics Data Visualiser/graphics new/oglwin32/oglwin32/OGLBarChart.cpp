#include "OGLBarChart.h"
#include <Windows.h>
#include <gl/GL.h>
#include <ctime>
#include <iostream>

OGLBarChart::OGLBarChart(int viewWidth, int viewHeight, Vector2D graphPos) : Graph(viewWidth, viewHeight, graphPos)
{
	// Empties graph vector;
	graphVector.clear();
	rectVector.clear();
	yAxisVector.clear();

	// Set's the coordinates for the top's of the X and Y axis
	SetAxis();

	// Set graphInitialised to false
	graphInitialised = false;
	renderSelected = false;

	// Initialise other variables
	barWidth = 0;
	maxOccurency = 0;
	minOccurency = 0;
	totalOccurency = 0;
	yAxisInterval = 0;
	yAxisLabel->SetText("Frequency");
	selectedRectangle = new OGLRectangle();
}

OGLBarChart::~OGLBarChart()
{
	
}

void OGLBarChart::Render()
{
	if (graphInitialised)
	{
		glScalef(zoomVal, zoomVal, zoomVal);
		SetAxis();
		// Bar Drawing
		if (!barsCreated)
			CreateBars();
		else
			UpdateBars();

		for (int i = 0; i < rectVector.size(); i++)
		{
			rectVector[i]->Render();
		}
		
		// Axis Drawing
		glBegin(GL_LINES);
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

			// Draw notches on the Y axis
			glVertex2f((graphPos.GetX() - 10), y);
			glVertex2f(graphPos.GetX(), y);
		}

		textIndex = 0;
		for (int x = graphPos.GetX() + barWidth; x <= xAxisTop.GetX(); x += barWidth)
		{
			if (textIndex < xAxisVector.size())
			{
				xAxisVector[textIndex]->SetX(x - (barWidth / 2));
				xAxisVector[textIndex]->SetY(graphPos.GetY() - 5);
				textIndex++;
			}
			// Draws the notches on the X axis
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


		glPushMatrix();
		glScalef(5.0f, 5.0f, 5.0f);	

		float graphWidth = xAxisTop.GetX() - graphPos.GetX();
		yAxisLabel->SetX((graphPos.GetX() - 90) / 5.0f);
		yAxisLabel->SetY((graphPos.GetY() + (graphHeight / 2)) / 5.0f);
		xAxisLabel->SetX((graphPos.GetX() + (graphWidth / 2)) / 5.0f);
		xAxisLabel->SetY((graphPos.GetY() - 50) / 5.0f);

		yAxisLabel->Render();
		xAxisLabel->Render();
		glPopMatrix();

		Vector2D* rectPos;
		if (renderSelected)
		{
			glPushMatrix();
			glScalef(3.0f, 3.0f, 3.0f);
			line1->SetX((currentMousePos.GetX() / (zoomVal * 3)) + 2);
			line1->SetY((currentMousePos.GetY() / (zoomVal * 3)) - 2);

			rectPos = new Vector2D(line1->GetX(), line1->GetY());
			selectedRectangle->SetTopLeft(*rectPos);

			rectPos->SetX(line1->GetX() + line1->GetString().size() + 1);
			rectPos->SetY(line1->GetY() - 3);
			selectedRectangle->SetBotRight(*rectPos);

			glColor3f(0.0f, 0.0f, 0.0f);
			selectedRectangle->Render();

			glColor3f(1.0f, 1.0f, 1.0f);
			line1->Render();
			glPopMatrix();
		}
	}
}

bool OGLBarChart::MouseLBUp(int x, int y)
{
	if (mouseClicked)
	{
		mouseClicked = false;
		// std::cout << "Mouseclicked: False" << std::endl;

		glScalef(zoomVal, zoomVal, zoomVal);
		for (int i = 0; i < rectVector.size(); i++)
		{
			rectVector[i]->MouseLBUp(x, y);
			renderSelected = false;
		}
		return true;
	}
	return false;
}

bool OGLBarChart::MouseLBDown(int x, int y)
{
	if (!mouseClicked)
	{
		mouseClicked = true;
		// std::cout << "Mouseclicked: True" << std::endl;

		glScalef(zoomVal, zoomVal, zoomVal);
		for (int i = 0; i < rectVector.size(); i++)
		{
			if (rectVector[i]->MouseLBDown(x, y))
			{
				char occurencyString[200];
				char* occurency;
				occurency = &occurencyString[0];
				itoa(rectVector[i]->GetGraphItem().occurency, occurency, 10);
				std::string text = "Item: " + rectVector[i]->GetGraphItem().data + ", Occurency: " + occurency;

				line1->SetText(text.c_str());
				line1->SetX(x);
				line1->SetY(y);

				//Vector2D* rectPos = new Vector2D(x, y);
				//selectedRectangle->SetTopLeft(*rectPos);
				//rectPos->SetY(y - 5);
				//rectPos->SetX(x + 30);
				//selectedRectangle->SetBotRight(*rectPos);
				renderSelected = true;
			}
		}
		return true;
	}
	return false;
}

void OGLBarChart::SetAxis()
{
	xAxisTop.SetX((graphPos.GetX() + (vWidth - 200)));
	xAxisTop.SetY(graphPos.GetY());

	yAxisTop.SetX(graphPos.GetX());
	yAxisTop.SetY((graphPos.GetY() + (vHeight - 200)));
}

void OGLBarChart::GraphInitialise()
{
	barWidth = (xAxisTop.GetX() - graphPos.GetX()) / (graphVector.size() - 1);
	maxOccurency = 0;
	graphHeight = 0;

	for (int i = 1; i < graphVector.size(); i++)
	{
		totalOccurency += graphVector[i].occurency;
		if (maxOccurency == 0 || maxOccurency < graphVector[i].occurency)
			maxOccurency = graphVector[i].occurency;
	}

	graphHeight = yAxisTop.GetY() - graphPos.GetY();

	float a = 0;
	float b = graphHeight;
	float A = 0;
	float B = maxOccurency;
	float x = 1;
	// (a + ((x - a) * (b - a)))/ (B - A)
	singleInterval = (a + ((x - a) * (b - a))) / (B - A);
	x = maxOccurency / 20;
	yAxisInterval = (a + ((x - a) * (b - a))) / (B - A);

	// Initalises the Y axis labels
	yAxisVector.clear();
	GUIText* intervalText;
	char interval[100];
	int intervalNo = 0;
	for (float y = graphPos.GetY(); y <= yAxisTop.GetY(); y += yAxisInterval)
	{
		if (y == graphPos.GetY())
		{
			intervalText = new GUIText("0", graphPos.GetX() - 55, y );
		}
		else if (y != graphPos.GetY())
		{
			itoa(((yAxisInterval * intervalNo) / singleInterval), interval, 10);
			intervalText = new GUIText(interval, graphPos.GetX() - 15, y);
		}
		intervalNo++;
		yAxisVector.push_back(intervalText);
	}

	intervalNo = 1;
	for (float x = graphPos.GetX() + (barWidth / 2); x < xAxisTop.GetX(); x += barWidth)
	{
		if (intervalNo < graphVector.size())
		{
			intervalText = new GUIText((char*)graphVector[intervalNo].data.c_str(), x, graphPos.GetY() - 5);
			xAxisVector.push_back(intervalText);
		}
		intervalNo++;
	}

	// Sets graphinitalised to true, enabled the graph to be rendered
	graphInitialised = true;
}

void OGLBarChart::CreateBars()
{
	OGLRectangle* newRectangle;
	rectVector.clear();

	xAxisLabel->SetText(graphVector[0].data.c_str());

	int i = 1; // Bar vector index
	for (float x = graphPos.GetX(); x < xAxisTop.GetX(); x += barWidth)
	{
		if (i < graphVector.size())
		{
			float y = graphPos.GetY() + (graphVector[i].occurency * singleInterval);

			newRectangle = new OGLRectangle(x, y, (x + barWidth), graphPos.GetY(), graphVector[i], zoomVal);
			rectVector.push_back(newRectangle);
			i++;
		}
	}
	barsCreated = true;
}

void OGLBarChart::UpdateBars()
{
	int i = 0; // Bar vector index
	for (float x = graphPos.GetX(); x < xAxisTop.GetX(); x += barWidth)
	{
		if (i < rectVector.size())
		{
			float y = graphPos.GetY() + (graphVector[i + 1].occurency * singleInterval);
			Vector2D topLeft = { x, y };
			Vector2D botRight = { (x + barWidth), graphPos.GetY() };
			rectVector[i]->SetTopLeft(topLeft);
			rectVector[i]->SetBotRight(botRight);
			rectVector[i]->SetZoom(zoomVal);		
		}
		i++;
	}
}

void OGLBarChart::CentreGraph()
{
	graphPos.SetX(CoordConvertX(100));
	graphPos.SetY(CoordConvertY(vHeight - 100));
	SetAxis();
}

void OGLBarChart::ClearGraph()
{
	graphVector.clear();
	rectVector.clear();
	yAxisVector.clear();
	xAxisVector.clear();

	barWidth = 0;
	maxOccurency = 0;
	minOccurency = 0;
	totalOccurency = 0;
	yAxisInterval = 0;
	singleInterval = 0;
	graphInitialised = 0;
	barsCreated = false;
}
