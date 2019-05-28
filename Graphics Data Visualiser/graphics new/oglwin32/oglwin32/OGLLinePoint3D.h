#pragma once

#include "Listener.h"
#include "Renderable.h"
#include "Vector3D.h"
#include "GraphItems.h"

class OGLLinePoint3D : public Listener, public Renderable
{
	public:
						OGLLinePoint3D(float x, float y, float z, graphItem3E itemIn, float zVal);
						~OGLLinePoint3D();

		void			Render();
		bool			MouseMove(int x, int y);
		bool			MouseLBUp(int x, int y);
		bool			MouseLBDown(int x, int y);
		Vector3D inline GetPoint() { return point; };
		void     inline SetPoint(Vector3D pointIn) { point = pointIn; };

	private:
		Vector3D		point;
		graphItem3E		graphData;
		bool			selected;
		float			zoomVal;
};