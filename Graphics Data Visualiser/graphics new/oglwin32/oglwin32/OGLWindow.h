#pragma once

#include <Windows.h>
#include "OGLRectangle.h"
#include "OGLBarChart.h"
#include "OGLPieChart.h"
#include "OGLLineChart.h"
#include "OGLScatter2D.h"
#include "OGLScatter3D.h"
#include "GUIText.h"
#include "GraphItems.h"

class OGLWindow
{
	private:
		HWND			m_hwnd;				//handle to a window
		HDC				m_hdc;				//handle to a device context
		HGLRC			m_hglrc;			//handle to a gl rendering context

		int				m_width;
		int				m_height;
		
		//This is not an ideal place to hold geometry data
		OGLBarChart*	barChart;
		OGLPieChart*	pieChart;
		OGLLineChart*	lineChart;
		OGLScatter2D*	scatter2D;
		OGLScatter3D*	scatter3D;

		// TEMP
		GUIText*		myText;

		int				XCoordConvert(int input);
		int				YCoordConvert(int input);

	protected:

		HGLRC			CreateOGLContext (HDC hdc);
		BOOL			DestroyOGLContext();
		BOOL			InitWindow(HINSTANCE hInstance, int width, int height);
		void			InitOGLState();

	public:
						OGLWindow();
						OGLWindow(HINSTANCE hInstance, int width, int height);
						~OGLWindow();

		void			Render();
		void			Resize( int width, int height );
		void			SetVisible( BOOL visible );
		void			DestroyOGLWindow();

		OGLBarChart*  GetBarChart();
		OGLPieChart*  GetPieChart();
		OGLLineChart* GetLineChart();
		OGLScatter2D* GetScatter2D();
		OGLScatter3D* GetScatter3D();

		BOOL			MouseLBDown ( int x, int y );
		BOOL			MouseLBUp ( int x, int y );
		BOOL			MouseMove ( int x, int y );
};