#pragma once
#include <string>
#include "OGLRectangle.h"
#include "Vector2D.h"
#include "GraphItems.h"
#include "GUIText.h"
#include "Graph.h"

class OGLBarChart : public Graph
{
	public:
		// Constructor and Destructor
		OGLBarChart(int viewWidth, int viewHeight, Vector2D graphPos);
		~OGLBarChart();

		// Override Methods
		void 				Render();
		bool				MouseLBUp(int x, int y);
		bool				MouseLBDown(int x, int y);

		// Various Methods
		void				SetAxis();
		void				GraphInitialise();
		void				CreateBars();
		void				UpdateBars();
		void				CentreGraph();
		void				ClearGraph();

	private:
		// Variables and containers
		std::vector <OGLRectangle*>	rectVector;
		std::vector<GUIText*>		yAxisVector;
		std::vector<GUIText*>		xAxisVector;
		Vector2D			xAxisTop;
		Vector2D			yAxisTop;
		OGLRectangle*		selectedRectangle;

		float				barWidth;
		float				maxOccurency;
		float				minOccurency;
		float				totalOccurency;
		float				graphHeight;
		float				yAxisInterval;
		float				singleInterval;
		bool				graphInitialised;
		bool				barsCreated = false;
};