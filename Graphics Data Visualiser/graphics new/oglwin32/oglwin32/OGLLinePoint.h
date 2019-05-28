#pragma once

#include "Listener.h"
#include "Renderable.h"
#include "Vector2D.h"
#include "GraphItems.h"

class OGLLinePoint : public Listener, public Renderable
{
	public:
						OGLLinePoint(float x, float y, graphItem itemIn, float zVal);
						OGLLinePoint(float x, float y, graphItem2E itemIn, float zVal);
						~OGLLinePoint();

		void			Render();
		bool			MouseMove(int x, int y);
		bool			MouseLBUp(int x, int y);
		bool			MouseLBDown(int x, int y);

		Vector2D inline GetPoint() { return point; };
		void	 inline	SetPoint(Vector2D pointIn) { point = pointIn; };
		graphItem inline GetGraphItem() { return graphData; };
		graphItem2E	inline	GetGraphItem2E() { return graphData2E; };
		void	inline	SetZoomVal(float zoomIn) { zoomVal = zoomIn; };

		// bool	 inline	GetSelected() { return selected; };
		//void	 inline	SetSelected(bool inSelected) { selected = inSelected; };

	private:
		Vector2D		point;
		graphItem		graphData;
		graphItem2E		graphData2E;
		bool			selected;
		float			zoomVal;
		bool			twoElements;
};