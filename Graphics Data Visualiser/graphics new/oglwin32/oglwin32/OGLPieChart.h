#pragma once

#include "OGLPieSegment.h"
#include "OGLRectangle.h"
#include "GUIText.h"
#include "Graph.h"
#include <math.h>
#include <Windows.h>
#include <gl/GL.h>

#define PI 3.14159265

class OGLPieChart : public Graph
{
	public:
							OGLPieChart(int viewWidth, int viewHeight, Vector2D graphPos);
							~OGLPieChart();

		void				Render();
		bool				MouseLBUp(int x, int y);
		bool				MouseLBDown(int x, int y);
		void				GraphInitialise();
		void				CentreGraph();
		void				ClearGraph();
					
	private:
		std::vector<OGLPieSegment*>	segVector;
		std::vector<OGLRectangle*> rectVector;
		std::vector<GUIText*>	textVector;
		OGLRectangle*			selectedRectangle;
		
		float					segmentInterval;
		float					maxValues;
		float					radius;
		bool					graphInitialised = false;
		bool					segmentsCreated = false;

		void					CreateSegments();
		void					UpdateSegments();
};