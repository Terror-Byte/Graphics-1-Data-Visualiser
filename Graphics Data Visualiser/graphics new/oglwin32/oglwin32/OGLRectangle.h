#pragma once

#include "Renderable.h"
#include "Listener.h"
#include "Vector2D.h"
#include "GraphItems.h"
#include "OGLPieSegment.h"

class OGLRectangle : public Renderable, public Listener
{
	public:
						OGLRectangle();
						OGLRectangle(float topLeftX, float topLeftY, float botRightX, float botRightY, OGLPieSegment* pieSeg, float zVal);
						OGLRectangle(float topLeftX, float topLeftY, float botRightX, float botRightY, graphItem itemIn, float zVal);
						~OGLRectangle();

		void			Render();
		bool			MouseMove( int x, int y );
		bool			MouseLBUp( int x, int y );
		bool			MouseLBDown( int x, int y );

		graphItem		GetGraphItem();
		Vector2D inline	GetTopLeft() { return m_topleft; };
		Vector2D inline GetBotRight() { return m_bottomright; };

		void	inline	SetTopLeft(Vector2D inVector) { m_topleft = inVector; };
		void	inline	SetBotRight(Vector2D inVector) { m_bottomright = inVector; };
		void	inline	SetZoom(float zoom) { zoomVal = zoom; };

	private:
		Vector2D		m_topleft;
		Vector2D		m_bottomright;
		graphItem		graphData;
		OGLPieSegment*  pieSegment;
		bool			selected;
		bool			segmentLinked;
		float			zoomVal;
		RGB				colour;
};