#include "Graph.h"

Graph::Graph()
{

}

Graph::Graph(int viewWidth, int viewHeight, Vector2D graphPosIn)
{
	vWidth = viewWidth;
	vHeight = viewHeight;
	graphPos = CoordConvertVec2D(graphPosIn);
	currentMousePos.SetX(0);
	currentMousePos.SetX(0);
	zoomVal = 1.0f;
	line1 = new GUIText();
	xAxisLabel = new GUIText();
	yAxisLabel = new GUIText();
	renderSelected = false;
}

Graph::~Graph()
{

}

void Graph::Render()
{

}

bool Graph::MouseMove(int x, int y)
{
	deltaMousePos.SetX(x - currentMousePos.GetX());
	deltaMousePos.SetY(y - currentMousePos.GetY());
	currentMousePos.SetX(x);
	currentMousePos.SetY(y);

	if (mouseClicked)
	{
		graphPos.SetX(graphPos.GetX() + deltaMousePos.GetX());
		graphPos.SetY(graphPos.GetY() + deltaMousePos.GetY());
	}
	line1->SetX(x);
	line1->SetY(y);
	
	return true;
}

bool Graph::MouseLBUp(int x, int y)
{
	if (mouseClicked)
	{
		mouseClicked = false;
		return true;
	}
	return false;
}

bool Graph::MouseLBDown(int x, int y)
{
	if (!mouseClicked)
	{
		mouseClicked = true;
		return true;
	}
	return false;
}

Vector2D Graph::CoordConvertVec2D(Vector2D input)
{
	input.SetX((input.GetX() - (vWidth / 2)));
	input.SetY((-input.GetY() + (vHeight / 2)));
	return input;
}

void Graph::ParseData(ListHead* initialHead, int index)
{
	ListHead* currentHead = initialHead;
	ListNode* currentNode;

	while (currentHead != NULL)
	{
		currentNode = currentHead->nextNode;
		while (currentNode != NULL)
		{
			if (currentNode->GetPos() == index)
			{
				if (graphVector.empty())
				{
					graphItem newItem;
					/*RGB newColour;
					newColour.red = 3.0 / (rand() % 100);
					newColour.green = 3.0 / (rand() % 100);
					newColour.blue = 3.0 / (rand() % 100);*/
					newItem.colour.red = 3.0 / (rand() % 100);
					newItem.colour.green = 3.0 / (rand() % 100);
					newItem.colour.blue = 3.0 / (rand() % 100);

					newItem.data = currentNode->GetString();
					newItem.occurency = 1;
					// newItem.colour = newColour;
					graphVector.push_back(newItem);
				}
				else
				{
					bool match = false;
					int matchIndex = 0;
					for (int i = 0; i < graphVector.size(); i++)
					{
						if (graphVector[i].data == currentNode->GetString())
						{
							match = true;
							matchIndex = i;
							break;
						}
					}
					if (match)
					{
						graphVector[matchIndex].occurency++;
					}
					else
					{
						graphItem newItem;
						/*RGB newColour;
						newColour.red = 3.0 / (rand() % 100);
						newColour.green = 3.0 / (rand() % 100);
						newColour.blue = 3.0 / (rand() % 100);*/
						newItem.colour.red = 3.0 / (rand() % 100);
						newItem.colour.green = 3.0 / (rand() % 100);
						newItem.colour.blue = 3.0 / (rand() % 100);

						newItem.data = currentNode->GetString();
						newItem.occurency = 1;
						/*newItem.colour = newColour;*/
						graphVector.push_back(newItem);
					}
				}
			}
			currentNode = currentNode->nextNode;
		}
		currentHead = currentHead->nextHead;
	}
}