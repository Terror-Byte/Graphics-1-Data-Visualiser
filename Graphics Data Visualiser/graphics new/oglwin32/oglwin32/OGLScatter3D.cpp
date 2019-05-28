#include "OGLScatter3D.h"
#include <Windows.h>
#include <gl/GL.h>

OGLLinePoint3D* testPoint; // REMOVE AFTER

OGLScatter3D::OGLScatter3D(int viewWidth, int viewHeight, Vector3D graphPos)
{
	xRotation = 22.5;
	yRotation = 45;
	vWidth = viewWidth;
	vHeight = viewHeight;
	zoomVal = 0.5f;
	currentMousePos.SetX(0);
	currentMousePos.SetX(0);
	// graphPos3D = CoordConvertVec3D(graphPos);
	graphPos3D = graphPos;

	xAxisLabel = new GUIText();
	yAxisLabel = new GUIText();
	zAxisLabel = new GUIText();

	xAxisVector.clear(); 
	yAxisVector.clear();
	zAxisVector.clear();
	pointVector.clear();
	SetAxis();

}

OGLScatter3D::~OGLScatter3D()
{

}

void OGLScatter3D::Render()
{
	if (graphInitialised)
	{	
		glRotatef(xRotation, 1.0f, 0.0f, 0.0f);
		glRotatef(-yRotation, 0.0f, 1.0f, 0.0f);
		glScalef(zoomVal, zoomVal, zoomVal);
		SetAxis();

		if (!pointsCreated)
			CreatePoints();
		else
			UpdatePoints();

		glBegin(GL_LINES);
		glColor3f(1.0, 0.0, 0.0); // X axis RED
		glVertex3f(graphPos3D.GetX(), graphPos3D.GetY(), graphPos3D.GetZ());
		glVertex3f(xAxisTop.GetX(), xAxisTop.GetY(), xAxisTop.GetZ());
		
		glColor3f(0.0, 1.0, 0.0); // Y axis GREEN
		glVertex3f(graphPos3D.GetX(), graphPos3D.GetY(), graphPos3D.GetZ());
		glVertex3f(yAxisTop.GetX(), yAxisTop.GetY(), yAxisTop.GetZ());

		glColor3f(0.0, 0.0, 1.0); // Z axis BLUE
		glVertex3f(graphPos3D.GetX(), graphPos3D.GetY(), graphPos3D.GetZ());
		glVertex3f(zAxisTop.GetX(), zAxisTop.GetY(), zAxisTop.GetZ());

		int textIndex = 0;
		for (float x = graphPos3D.GetX() + xAxisInterval; x <= xAxisTop.GetX(); x += xAxisInterval)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			if (textIndex < xAxisVector.size())
			{
				xAxisVector[textIndex]->SetX(x);
				xAxisVector[textIndex]->SetY(graphPos3D.GetY());
				xAxisVector[textIndex]->SetZ(graphPos3D.GetZ() - 15);
			}
			textIndex++;

			// Draws notches on X axis
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(x, graphPos3D.GetY(), graphPos3D.GetZ());
			glVertex3f(x, graphPos3D.GetY(), graphPos3D.GetZ() - 10);
		}

		textIndex = 0;	
		for (float y = graphPos3D.GetY() + yAxisInterval; y <= yAxisTop.GetY(); y += yAxisInterval)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			if (textIndex < yAxisVector.size())
			{
				yAxisVector[textIndex]->SetX(graphPos3D.GetX() - 40);
				yAxisVector[textIndex]->SetY(y + 2);
				yAxisVector[textIndex]->SetZ(graphPos3D.GetZ());
			}
			textIndex++;

			// Draws notches on X axis
			glColor3f(0.0, 1.0, 0.0);
			glVertex3f(graphPos3D.GetX(), y, graphPos3D.GetZ());
			glVertex3f(graphPos3D.GetX() - 10, y, graphPos3D.GetZ());
		}

		textIndex = 0;
		for (float z = graphPos3D.GetZ() + zAxisInterval; z <= zAxisTop.GetZ(); z += zAxisInterval)
		{
			glColor3f(1.0f, 1.0f, 1.0f);
			if (textIndex < zAxisVector.size())
			{
				zAxisVector[textIndex]->SetX(graphPos3D.GetX() - 40);
				zAxisVector[textIndex]->SetY(graphPos3D.GetY());
				zAxisVector[textIndex]->SetZ(z);
			}
			textIndex++;

			// Draws notches on X axis
			glColor3f(0.0, 0.0, 1.0);
			glVertex3f(graphPos3D.GetX(), graphPos3D.GetY(), z);
			glVertex3f(graphPos3D.GetX() - 10, graphPos3D.GetY(), z);
		}

		glEnd();
		// Draw Points
		for (int i = 0; i < pointVector.size(); i++)
		{
			pointVector[i]->Render();
		}

		glColor3f(1.0f, 1.0f, 1.0f);

		for (int i = 0; i < xAxisVector.size(); i++)
		{
			xAxisVector[i]->Render();
		}

		for (int i = 0; i < yAxisVector.size(); i++)
		{
			yAxisVector[i]->Render();
		}

		for (int i = 0; i < zAxisVector.size(); i++)
		{
			zAxisVector[i]->Render();
		}

	}
}

bool OGLScatter3D::MouseMove(int x, int y)
{
	deltaMousePos.SetX(x - currentMousePos.GetX());
	deltaMousePos.SetY(y - currentMousePos.GetY());
	currentMousePos.SetX(x);
	currentMousePos.SetY(y);

	if (mouseClicked)
	{
		xRotation -= deltaMousePos.GetY();
		yRotation -= deltaMousePos.GetX();		
	}
	return true;
}

bool OGLScatter3D::MouseLBUp(int x, int y)
{
	if (mouseClicked)
	{
		mouseClicked = false;
		return true;
	}
	return false;
}

bool OGLScatter3D::MouseLBDown(int x, int y)
{
	if (!mouseClicked)
	{
		mouseClicked = true;
		return true;
	}
	return false;
}

// Remove?
//void OGLScatter3D::SetProjection()
//{
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glFrustum(-0.5*vWidth, 0.5*vWidth, -0.5*vHeight, 0.5*vHeight, -1000, 1000);
//
//	glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//}

void OGLScatter3D::SetAxis()
{
	float avg = (vWidth + vHeight) / 2;

	xAxisTop.SetX((graphPos3D.GetX() + (avg - 200)));
	xAxisTop.SetY(graphPos3D.GetY());
	xAxisTop.SetZ(graphPos3D.GetZ());

	yAxisTop.SetX(graphPos3D.GetX());
	yAxisTop.SetY((graphPos3D.GetY() + (avg - 200)));
	yAxisTop.SetZ(graphPos3D.GetZ());

	zAxisTop.SetX(graphPos3D.GetX());
	zAxisTop.SetY(graphPos3D.GetY());
	zAxisTop.SetZ((graphPos3D.GetZ() + (avg - 200)));
}

void OGLScatter3D::GraphInitialise()
{
	float axisLength = xAxisTop.GetX() - graphPos3D.GetX();

	for (int i = 0; i < graph3EVector.size(); i++)
	{
		// Populating nameVectorX
		if (nameVectorX.empty())
		{
			nameVectorX.push_back(graph3EVector[i].data1);
		}
		else
		{
			bool match = false;

			for (int j = 0; j < nameVectorX.size(); j++)
			{
				if (graph3EVector[i].data1 == nameVectorX[j])
				{
					match = true;
				}
			}
			if (!match)
			{
				nameVectorX.push_back(graph3EVector[i].data1);
			}
		}

		// Populating nameVectorY
		if (nameVectorY.empty())
		{
			nameVectorY.push_back(graph3EVector[i].data2);
		}
		else
		{
			bool match = false;

			for (int j = 0; j < nameVectorY.size(); j++)
			{
				if (graph3EVector[i].data2 == nameVectorY[j])
				{
					match = true;
				}
			}
			if (!match)
			{
				nameVectorY.push_back(graph3EVector[i].data2);
			}
		}

		// Populating nameVectorZ
		if (nameVectorZ.empty())
		{
			nameVectorZ.push_back(graph3EVector[i].data3);
		}
		else
		{
			bool match = false;

			for (int j = 0; j < nameVectorZ.size(); j++)
			{
				if (graph3EVector[i].data3 == nameVectorZ[j])
				{
					match = true;
				}
			}
			if (!match)
			{
				nameVectorZ.push_back(graph3EVector[i].data3);
			}
		}
	}

	// Create intervals for x, y and z axis
	xAxisInterval = axisLength / nameVectorX.size();
	yAxisInterval = axisLength / nameVectorY.size();
	zAxisInterval = axisLength / nameVectorZ.size();

	GUIText* intervalText;
	int i = 0;
	for (float y = graphPos3D.GetY() + yAxisInterval; y <= yAxisTop.GetY(); y += yAxisInterval)
	{
		if (i < nameVectorY.size())
		{
			intervalText = new GUIText((char*)nameVectorY[i].c_str(), graphPos3D.GetX() - 40, y + 5, graphPos3D.GetZ());
			intervalText->SetIs3D(true);
			i++;
		}
		yAxisVector.push_back(intervalText);
	}

	i = 0;
	for (float x = graphPos3D.GetX() + xAxisInterval; x <= xAxisTop.GetX(); x += xAxisInterval)
	{
		if (i < nameVectorX.size())
		{
			intervalText = new GUIText((char*)nameVectorX[i].c_str(), x, graphPos3D.GetY(), graphPos3D.GetZ() - 15);
			intervalText->SetIs3D(true);
			i++;
		}
		xAxisVector.push_back(intervalText);
	}

	i = 0;
	for (float z = graphPos3D.GetZ(); z <= zAxisTop.GetZ(); z += zAxisInterval)
	{
		if (i < nameVectorZ.size())
		{
			intervalText = new GUIText((char*)nameVectorZ[i].c_str(), graphPos3D.GetX() - 30, graphPos3D.GetY(), z);
			intervalText->SetIs3D(true);
			i++;
		}
		zAxisVector.push_back(intervalText);
	}

	graphInitialised = true;
}

void OGLScatter3D::CreatePoints()
{
	OGLLinePoint3D* newPoint;
	pointVector.clear();
	float xCoord;
	float yCoord;
	float zCoord;

	for (int i = 0; i < graph3EVector.size(); i++)
	{
		for (int x = 0; x < nameVectorX.size(); x++)
		{
			if (graph3EVector[i].data1 == nameVectorX[x])
			{
				xCoord = graphPos3D.GetX() + xAxisInterval + (xAxisInterval * x);
			}
		}
		for (int y = 0; y < nameVectorY.size(); y++)
		{
			if (graph3EVector[i].data2 == nameVectorY[y])
			{
				yCoord = graphPos3D.GetY() + yAxisInterval + (yAxisInterval * y);
			}
		}
		for (int z = 0; z < nameVectorZ.size(); z++)
		{
			if (graph3EVector[i].data3 == nameVectorZ[z])
			{
				zCoord = graphPos3D.GetZ() + zAxisInterval + (zAxisInterval * z);
			}
		}
		newPoint = new OGLLinePoint3D(xCoord, yCoord, zCoord, graph3EVector[i], zoomVal);
		pointVector.push_back(newPoint);
	}
	pointsCreated = true;
}

void OGLScatter3D::UpdatePoints()
{
	float xCoord;
	float yCoord;
	float zCoord;
	Vector3D newPos;

	for (int i = 0; i < graph3EVector.size(); i++)
	{
		for (int x = 0; x < nameVectorX.size(); x++)
		{
			if (graph3EVector[i].data1 == nameVectorX[x])
			{
				xCoord = graphPos3D.GetX() + xAxisInterval + (xAxisInterval * x);
			}
		}
		for (int y = 0; y < nameVectorY.size(); y++)
		{
			if (graph3EVector[i].data2 == nameVectorY[y])
			{
				yCoord = graphPos3D.GetY() + yAxisInterval + (yAxisInterval * y);
			}
		}
		for (int z = 0; z < nameVectorZ.size(); z++)
		{
			if (graph3EVector[i].data3 == nameVectorZ[z])
			{
				zCoord = graphPos3D.GetZ() + zAxisInterval + (zAxisInterval * z);
			}
		}
		newPos = { xCoord, yCoord, zCoord };
		pointVector[i]->SetPoint(newPos);
	}
}

void OGLScatter3D::CentreGraph()
{
	graphPos3D.SetX(0);
	graphPos3D.SetY(-(vHeight / 4));
	graphPos3D.SetZ(0);
	xRotation = 22.5;
	yRotation = 45;
	SetAxis();
}

void OGLScatter3D::ParseData3E(ListHead* initialHead, int index1, int index2, int index3)
{
	SetAxisLabels(initialHead, index1, index2, index3);
	ListHead* currentHead = initialHead->nextHead;
	ListNode* currentNode;
	std::string data1;
	std::string data2;
	std::string data3;
	bool data1found = false;
	bool data2found = false;
	bool data3found = false;

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

		// Find third data node
		currentNode = currentHead->nextNode;
		while (currentNode != NULL || !data3found)
		{
			if (currentNode->GetPos() == index3)
			{
				data3 = currentNode->GetString();
				data3found = true;
			}
			currentNode = currentNode->nextNode;
		}

		// If graph vector is empty, or entry is not already in graph vector, create new entry. If same entry found, entry occurency incremented by 1
		if (graph3EVector.empty())
		{
			graphItem3E newItem;
			newItem.colour.red = 3.0 / (rand() % 100);
			newItem.colour.green = 3.0 / (rand() % 100);
			newItem.colour.blue = 3.0 / (rand() % 100);

			newItem.data1 = data1;
			newItem.data2 = data2;
			newItem.data3 = data3;
			newItem.occurency = 1;

			graph3EVector.push_back(newItem);
		}
		else
		{
			bool match = false; 
			int matchIndex = 0;
			for (int i = 0; i < graph3EVector.size(); i++)
			{
				if (graph3EVector[i].data1 == data1 && graph3EVector[i].data2 == data2 && graph3EVector[i].data3 == data3)
				{
					match = true;
					matchIndex = i;
					break;
				}
			}
			if (match)
			{
				graph3EVector[matchIndex].occurency++;
			}
			else
			{
				graphItem3E newItem;
				newItem.colour.red = 3.0 / (rand() % 100);
				newItem.colour.green = 3.0 / (rand() % 100);
				newItem.colour.blue = 3.0 / (rand() % 100);

				newItem.data1 = data1;
				newItem.data2 = data2;
				newItem.data3 = data3;
				newItem.occurency = 1;

				graph3EVector.push_back(newItem);
			}
		}
		currentHead = currentHead->nextHead;
	}
}

Vector3D OGLScatter3D::CoordConvertVec3D(Vector3D input)
{
	input.SetX(CoordConvertX(input.GetX()));
	input.SetY(CoordConvertY(input.GetY()));
	return input;
}

void OGLScatter3D::ClearGraph()
{
	graphVector.clear();
	graph3EVector.clear();
	pointVector.clear();
	xAxisVector.clear();
	yAxisVector.clear();
	zAxisVector.clear();
	nameVectorX.clear();
	nameVectorY.clear();
	nameVectorZ.clear();

	maxHeight = 0;
	xAxisInterval = 0;
	yAxisInterval = 0;
	zAxisInterval = 0;
	graphInitialised = false;
	pointsCreated = false;
}

void OGLScatter3D::SetAxisLabels(ListHead* initialHead, int index1, int index2, int index3)
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

	currentNode = initialHead->nextNode;
	while (currentNode != NULL)
	{
		if (currentNode->GetPos() == index3)
		{
			zAxisLabel->SetText(currentNode->GetString().c_str());
			break;
		}
		currentNode = currentNode->nextNode;
	}
}