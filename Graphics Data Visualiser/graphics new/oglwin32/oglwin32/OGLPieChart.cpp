#include "OGLPieChart.h"

OGLPieChart::OGLPieChart(int viewWidth, int viewHeight, Vector2D graphPos) : Graph(viewWidth, viewHeight, graphPos)
{
	graphVector.clear();
	segVector.clear();

	maxValues = 0;
	segmentInterval = 0;
	radius = 0;
	renderSelected = false;
	selectedRectangle = new OGLRectangle();
}

OGLPieChart::~OGLPieChart()
{

}

void OGLPieChart::Render()
{
	if (graphInitialised)
	{
		glScalef(zoomVal, zoomVal, zoomVal);
		if (!segmentsCreated)
			CreateSegments();
		else
			UpdateSegments();
		for (int i = 0; i < segVector.size(); i++)
		{
			segVector[i]->Render();
			rectVector[i]->Render();
			glColor3f(1.0f, 1.0f, 1.0f);
			textVector[i]->Render();
		}

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

bool OGLPieChart::MouseLBUp(int x, int y)
{
	if (mouseClicked)
	{
		mouseClicked = false;

		for (int i = 0; i < segVector.size(); i++)
		{
			segVector[i]->MouseLBUp(x, y);
			renderSelected = false;
		}
	}
	return false;
}

bool OGLPieChart::MouseLBDown(int x, int y)
{
	if (!mouseClicked)
	{
		mouseClicked = true;
		for (int i = 0; i < segVector.size(); i++)
		{
			if (segVector[i]->MouseLBDown(x, y))
			{
				char occurencyString[200];
				char* occurency;
				occurency = &occurencyString[0];
				itoa(segVector[i]->GetGraphItem().occurency, occurency, 10);
				std::string text = "Item: " + segVector[i]->GetGraphItem().data + ", Occurency: " + occurency;

				line1->SetText(text.c_str());
				line1->SetX(x);
				line1->SetY(y);

				renderSelected = true;
			}
		}
	}
	return false;
}

void OGLPieChart::GraphInitialise()
{
	maxValues = 0;
	for (int i = 1; i < graphVector.size(); i++)
	{
		maxValues += graphVector[i].occurency;
	}

	segmentInterval = (2 * PI) / maxValues;
	radius = vWidth / 4;

	graphInitialised = true;
}

void OGLPieChart::CreateSegments()
{
	OGLPieSegment* newSegment;
	OGLRectangle* newRect;

	segVector.clear();
	textVector.clear();
	rectVector.clear();

	float startAngle = 0;
	float endAngle = 0;
	float rectX = graphPos.GetX() + radius + 5;
	float rectY = graphPos.GetY() + radius;
	int yInterval = 1;
	int xInterval = 0;

	for (int i = 1; i < graphVector.size(); i++)
	{
		int rectYinterval = 4 * (yInterval - 1);
		if (rectY - rectYinterval < graphPos.GetY() - radius)
		{
			yInterval = 1;
			xInterval += 30;
			rectYinterval = 4 * (yInterval - 1);
		}

		endAngle = startAngle + (graphVector[i].occurency * segmentInterval);

		newSegment = new OGLPieSegment(graphPos, startAngle, endAngle, radius, graphVector[i], zoomVal);
		newRect = new OGLRectangle(rectX + xInterval, rectY - rectYinterval + yInterval, rectX + 8 + xInterval, rectY - rectYinterval - 8 + yInterval, newSegment, zoomVal);

		segVector.push_back(newSegment);
		rectVector.push_back(newRect);
		startAngle = endAngle;
		yInterval++;
	}

	GUIText* segmentText;
	float textX;
	float textY;
	for (int i = 0; i < rectVector.size(); i++)
	{
		textX = rectVector[i]->GetTopLeft().GetX() + 16;
		textY = (rectVector[i]->GetTopLeft().GetY() + rectVector[i]->GetBotRight().GetY()) / 2;

		segmentText = new GUIText((char*)rectVector[i]->GetGraphItem().data.c_str(), textX, textY);
		textVector.push_back(segmentText);
	}
	segmentsCreated = true;
}

void OGLPieChart::UpdateSegments()
{
	for (int i = 0; i < segVector.size(); i++)
	{
		segVector[i]->SetGraphPos(graphPos);
		segVector[i]->SetZoom(zoomVal);
	}

	float rectX = graphPos.GetX() + radius + 10;
	float rectY = graphPos.GetY() + radius;
	Vector2D* topLeft;
	Vector2D* botRight;
	float textX;
	float textY;
	int yInterval = 1;
	int xInterval = 0;

	for (int i = 0; i < rectVector.size(); i++)
	{
		int rectYinterval = 16 * (yInterval - 1);
		if (rectY - rectYinterval < graphPos.GetY() - radius)
		{
			yInterval = 1;
			xInterval += 30;
			rectYinterval = 16 * (yInterval - 1);
		}

		topLeft = new Vector2D(rectX + xInterval, rectY - rectYinterval + yInterval);
		botRight = new Vector2D(rectX + 8 + xInterval, rectY - rectYinterval - 8 + yInterval);

		rectVector[i]->SetTopLeft(*topLeft);
		rectVector[i]->SetBotRight(*botRight);

		textX = rectX + xInterval + 16;
		textY = (topLeft->GetY() + botRight->GetY()) / 2;
		textVector[i]->SetX(textX);
		textVector[i]->SetY(textY);

		yInterval++;
	}
}

void OGLPieChart::CentreGraph()
{
	graphPos.SetX(CoordConvertX(vWidth / 2));
	graphPos.SetY(CoordConvertY(vHeight / 2));
}

void OGLPieChart::ClearGraph()
{
	graphVector.clear();
	segVector.clear();
	textVector.clear();

	segmentInterval = 0;
	maxValues = 0;
	radius = 0;
	graphInitialised = false;
	segmentsCreated = false;
}