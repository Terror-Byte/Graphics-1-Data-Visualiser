#pragma once
#include <vector>
#include "Renderable.h"
#include "Listener.h"
#include "Vector2D.h"
#include "ListHead.h"
#include "ListNode.h"
#include "Vector2D.h"
#include "GraphItems.h"
#include "GUIText.h"


class Graph : public Renderable, public Listener
{
	public:
							Graph();
							Graph(int viewWidth, int viewHeight, Vector2D graphPosIn);
							~Graph();

		// Listener Methods
		void 				Render();
		bool				MouseMove(int x, int y);
		bool				MouseLBUp(int x, int y);
		bool				MouseLBDown(int x, int y);

		// Gets/Sets
		void	inline		SetZoom(float zoomIn) { zoomVal = zoomIn; };
		float	inline		GetZoom() { return zoomVal; };

		void				SetGraphPos();

		void	inline		SetCanRender(bool canRenderIn) { canRender = canRenderIn; };
		bool	inline		GetCanRender() { return canRender; };

		void	inline		SetXAxisLabel(std::string input) { xAxisLabel->SetText(input.c_str()); };
		void	inline		SetYAxisLabel(std::string input) { yAxisLabel->SetText(input.c_str()); };

		// Misc Methods
		void				ParseData(ListHead* initialHead, int index);

	private:
		
	protected:
		std::vector <graphItem>		graphVector;
		bool				canRender = false;
		Vector2D			graphPos;
		int					vWidth;
		int					vHeight;
		float				zoomVal;
		GUIText*			line1;
		GUIText*			xAxisLabel;
		GUIText*			yAxisLabel;
		GUIText*			zAxisLabel;
		bool				renderSelected;

		// Rotation values, only really used for 3D scatter plot
		float				xRotation;
		float				yRotation;

		// Mouse position/click Variables
		bool				mouseClicked = false;
		Vector2D			currentMousePos;
		Vector2D			deltaMousePos;

		// Coordinate Conversion Methods
		Vector2D				CoordConvertVec2D(Vector2D input);
		float		inline		CoordConvertX(float input) { return (input - (vWidth / 2)); };
		float		inline		CoordConvertY(float input) { return (-input + (vHeight / 2)); };
};