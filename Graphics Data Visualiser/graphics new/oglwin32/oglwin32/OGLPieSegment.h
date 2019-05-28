#pragma once

#include "Listener.h"
#include "Renderable.h"
#include "Vector2D.h"
#include "Graph.h"
#include <math.h>

#define PI 3.14159265

class OGLPieSegment : public Listener, public Renderable
{
	public:
							OGLPieSegment(Vector2D gPos, float sAngle, float eAngle, float rad, graphItem gItem, float zVal);
							~OGLPieSegment();

		void				Render();
		bool				MouseMove(int x, int y);
		bool				MouseLBUp(int x, int y);
		bool				MouseLBDown(int x, int y);
		void	inline		SetGraphPos(Vector2D posIn) { graphPos = posIn; };
		void	inline		SetZoom(float zVal) { zoomVal = zVal; };
		graphItem	inline	GetGraphItem() { return graphData; };
		bool	inline		GetSelected() { return selected; };

	private:
		Vector2D			graphPos;
		float				startAngle;
		float				endAngle;
		float				radius;
		float				zoomVal;
		bool				selected = false;
		graphItem			graphData;
};