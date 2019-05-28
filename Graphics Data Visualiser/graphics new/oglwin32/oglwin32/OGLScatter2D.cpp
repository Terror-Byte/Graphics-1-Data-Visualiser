#include "OGLScatter2D.h"
#include <Windows.h>
#include <gl/GL.h>

OGLScatter2D::OGLScatter2D(int viewWidth, int viewHeight, Vector2D graphPos) : Graph(viewWidth, viewHeight, graphPos)
{
	yTextVector.clear();
	xTextVector.clear();
	pointVector.clear();
	SetAxis();
}

OGLScatter2D::~OGLScatter2D()
{

}

void OGLScatter2D::Render()
{
	if (graphInitialised)
	{
		glScalef(zoomVal, zoomVal, zoomVal);
		SetAxis();
		if (!pointsCreated)
			CreatePoints();
		else
			UpdatePoints();

		glBegin(GL_LINES);
		// Draws Axis
		glColor3f(0.9f, 0.9f, 0.9f);
		glVertex2f(graphPos.GetX(), graphPos.GetY());
		glVertex2f(xAxisTop.GetX(), xAxisTop.GetY());
		glVertex2f(graphPos.GetX(), graphPos.GetY());
		glVertex2f(yAxisTop.GetX(), yAxisTop.GetY());

		int textIndex = 0;
		for (float x = graphPos.GetX() + xAxisInterval; x <= xAxisTop.GetX(); x += xAxisInterval)
		{
			if (textIndex < xTextVector.size())
			{
				xTextVector[textIndex]->SetX(x);
				xTextVector[textIndex]->SetY(graphPos.GetY() - 10);
			}
			textIndex++;
			// Draws notches on X axis
			glVertex2f(x, (graphPos.GetY() - 10));
			glVertex2f(x, graphPos.GetY());
		}

		textIndex = 0;
		for (float y = graphPos.GetY() + yAxisInterval; y <= yAxisTop.GetY(); y += yAxisInterval)
		{
			if (textIndex < yTextVector.size())
			{
				yTextVector[textIndex]->SetX(graphPos.GetX() - 15);
				yTextVector[textIndex]->SetY(y);				
			}
			textIndex++;
			// Draws notches on Y axis
			glVertex2f((graphPos.GetX() - 10), y);
			glVertex2f(graphPos.GetX(), y);
		}

		glEnd();

		for (int i = 0; i < pointVector.size(); i++)
		{
			pointVector[i]->Render();
		}

		glColor3f(1.0f, 1.0f, 1.0f);

		for (int i = 0; i < yTextVector.size(); i++)
		{
			yTextVector[i]->Render();
		}

		for (int i = 0; i < xTextVector.size(); i++)
		{
			xTextVector[i]->Render();
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

bool OGLScatter2D::MouseLBUp(int x, int y)
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

bool OGLScatter2D::MouseLBDown(int x, int y)
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
				itoa(pointVector[i]->GetGraphItem2E().occurency, occurency, 10);
				std::string text = "Item: " + pointVector[i]->GetGraphItem2E().data1 + " and " +  pointVector[i]->GetGraphItem2E().data2 + ", Occurency: " + occurency;

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

void OGLScatter2D::SetAxis()
{
	xAxisTop.SetX((graphPos.GetX() + (vWidth - 200)));
	xAxisTop.SetY(graphPos.GetY());

	yAxisTop.SetX(graphPos.GetX());
	yAxisTop.SetY((graphPos.GetY() + (vHeight - 200)));
}

void OGLScatter2D::GraphInitialise()
{
	graphHeight = yAxisTop.GetY() - graphPos.GetY();
	graphWidth = xAxisTop.GetX() - graphPos.GetX();

	for (int i = 0; i < graph2EVector.size(); i++)
	{
		// Populating nameVectorX
		if (nameVectorX.empty())
		{
			nameVectorX.push_back(graph2EVector[i].data1);
		}
		else
		{
			bool match = false;

			for (int j = 0; j < nameVectorX.size(); j++)
			{
				if (graph2EVector[i].data1 == nameVectorX[j])
				{
					match = true;
				}
			}
			if (!match)
			{
				nameVectorX.push_back(graph2EVector[i].data1);
			}
		}

		// Populating nameVectorY
		if (nameVectorY.empty())
		{
			nameVectorY.push_back(graph2EVector[i].data2);
		}
		else
		{
			bool match = false;

			for (int j = 0; j < nameVectorY.size(); j++)
			{
				if (graph2EVector[i].data2 == nameVectorY[j])
				{
					match = true;
				}
			}
			if (!match)
			{
				nameVectorY.push_back(graph2EVector[i].data2);
			}
		}
	}

	// Create intervals for x and y axis'
	xAxisInterval = graphWidth / nameVectorX.size();
	yAxisInterval = graphHeight / nameVectorY.size();

	GUIText* intervalText;
	int i = 0;
	for (float y = graphPos.GetY() + yAxisInterval; y <= yAxisTop.GetY(); y += yAxisInterval)
	{
		if (i < nameVectorY.size())
		{
			intervalText = new GUIText((char*)nameVectorY[i].c_str(), graphPos.GetX() - 30, y);
			i++;
		}
		yTextVector.push_back(intervalText);
	}

	i = 0;
	for (float x = graphPos.GetX(); x <= xAxisTop.GetX(); x += xAxisInterval)
	{
		if (i < nameVectorX.size())
		{
			intervalText = new GUIText((char*)nameVectorX[i].c_str(), x, graphPos.GetY() - 10);
			i++;
		}
		xTextVector.push_back(intervalText);
	}

	graphInitialised = true;
}

void OGLScatter2D::CreatePoints()
{
	OGLLinePoint* newPoint;
	pointVector.clear();
	float xCoord;
	float yCoord;

	for (int i = 0; i < graph2EVector.size(); i++)
	{
		for (int x = 0; x < nameVectorX.size(); x++)
		{
			if (graph2EVector[i].data1 == nameVectorX[x])
			{
				xCoord = graphPos.GetX() + xAxisInterval + (xAxisInterval * x);
			}
		}
		for (int y = 0; y < nameVectorY.size(); y++)
		{
			if (graph2EVector[i].data2 == nameVectorY[y])
			{
				yCoord = graphPos.GetY() + yAxisInterval + (yAxisInterval * y);
			}
		}
		newPoint = new OGLLinePoint(xCoord, yCoord, graph2EVector[i], zoomVal);
		pointVector.push_back(newPoint);
	}
	pointsCreated = true;
}

void OGLScatter2D::UpdatePoints()
{
	float xCoord;
	float yCoord;
	Vector2D newPos;

	for (int i = 0; i < graph2EVector.size(); i++)
	{
		for (int x = 0; x < nameVectorX.size(); x++)
		{
			if (graph2EVector[i].data1 == nameVectorX[x])
			{
				xCoord = graphPos.GetX() + xAxisInterval + (xAxisInterval * x);
			}
		}
		for (int y = 0; y < nameVectorY.size(); y++)
		{
			if (graph2EVector[i].data2 == nameVectorY[y])
			{
				yCoord = graphPos.GetY() + yAxisInterval + (yAxisInterval * y);
			}
		}
		newPos.SetX(xCoord);
		newPos.SetY(yCoord);
		pointVector[i]->SetPoint(newPos);
		pointVector[i]->SetZoomVal(zoomVal);
	}
	//for (int i = 0; i < pointVector.size(); i++)
	//{

	//}
}

void OGLScatter2D::CentreGraph()
{
	graphPos.SetX(CoordConvertX(100));
	graphPos.SetY(CoordConvertY(vHeight - 100));
	SetAxis();
}

void OGLScatter2D::ParseData2E(ListHead* initialHead, int index1, int index2)
{
	SetAxisLabels(initialHead, index1, index2);
	ListHead* currentHead = initialHead->nextHead;
	ListNode* currentNode;
	std::string data1;
	std::string data2;
	bool data1found = false;
	bool data2found = false;

	while (currentHead != NULL)
	{
		// Find first data node
		currentNode = currentHead->nextNode;
		while (currentNode != NULL || !data1found)
		{
			if (currentNode->GetPos() == index1)
			{
				data1 = currentNode->GetString();
				data1found = true;
			}
			currentNode = currentNode->nextNode;
		}

		// Find second data node
		currentNode = currentHead->nextNode;
		while (currentNode != NULL || !data2found)
		{
			if (currentNode->GetPos() == index2)
			{
				data2 = currentNode->GetString();
				data2found = true;
			}
			currentNode = currentNode->nextNode;
		}

		// If graph vector is empty, or entry is not already in graph vector, create new entry. If same entry found, entry occurency incremented by 1
		if (graph2EVector.empty())
		{
			graphItem2E newItem;
			newItem.colour.red = 3.0 / (rand() % 100);
			newItem.colour.green = 3.0 / (rand() % 100);
			newItem.colour.blue = 3.0 / (rand() % 100);

			newItem.data1 = data1;
			newItem.data2 = data2;
			newItem.occurency = 1;

			graph2EVector.push_back(newItem);
		}
		else
		{
			bool match = false;
			int matchIndex = 0;
			for (int i = 0; i < graph2EVector.size(); i++)
			{
				if (graph2EVector[i].data1 == data1 && graph2EVector[i].data2 == data2)
				{
					match = true;
					matchIndex = i;
					break;
				}
			}
			if (match)
			{
				graph2EVector[matchIndex].occurency++;
			}
			else
			{
				graphItem2E newItem;
				newItem.colour.red = 3.0 / (rand() % 100);
				newItem.colour.green = 3.0 / (rand() % 100);
				newItem.colour.blue = 3.0 / (rand() % 100);

				newItem.data1 = data1;
				newItem.data2 = data2;
				newItem.occurency = 1;

				graph2EVector.push_back(newItem);
			}
		}
		currentHead = currentHead->nextHead;
	}
}

void OGLScatter2D::ClearGraph()
{
	graphVector.clear();
	graph2EVector.clear();
	pointVector.clear();
	xTextVector.clear();
	yTextVector.clear();
	nameVectorX.clear();
	nameVectorY.clear();

	maxHeight = 0;
	xAxisInterval = 0;
	yAxisInterval = 0;
	graphInitialised = false;
	pointsCreated = false;
}

void OGLScatter2D::SetAxisLabels(ListHead* initialHead, int index1, int index2)
{
	ListNode* currentNode;

	currentNode = initialHead->nextNode;
	while (currentNode != NULL)
	{
		if (currentNode->GetPos() == index1)
		{
			xAxisLabel->SetText(currentNode->GetString().c_str());
			break;
		}
		currentNode = currentNode->nextNode;
	}

	currentNode = initialHead->nextNode;
	while (currentNode != NULL)
	{
		if (currentNode->GetPos() == index2)
		{
			yAxisLabel->SetText(currentNode->GetString().c_str());
			break;
		}
		currentNode = currentNode->nextNode;
	}
}