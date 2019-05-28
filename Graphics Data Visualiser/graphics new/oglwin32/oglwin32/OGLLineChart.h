#pragma once

#include "Graph.h"
#include <vector>
#include "GUIText.h"
#include "OGLLinePoint.h"

class OGLLineChart : public Graph
{
	public:
		// Constructor and Destructor
		OGLLineChart(int viewWidth, int viewHeight, Vector2D graphPos);
		~OGLLineChart();

		// Override Methods
		void 				Render();
		bool				MouseLBUp(int x, int y);
		bool				MouseLBDown(int x, int y);

		// Various Methods
		void				SetAxis();
		void				GraphInitialise();
		void				CreatePoints();
		void				CentreGraph();
		void				UpdatePoints();
		void				ClearGraph();
		
	private:
		// add point container
		std::vector<GUIText*>		yAxisVector;
		std::vector<GUIText*>		xAxisVector;
		std::vector<OGLLinePoint*>	pointVector;
		Vector2D			xAxisTop;
		Vector2D			yAxisTop;

		float				maxOccurency;
		float				xAxisInterval;
		float				yAxisInterval;
		float				singleInterval;
		float				totalOccurency;
		float				graphWidth;
		float				graphHeight;
		bool				graphInitialised = false;
		bool				pointsCreated = false;
};
