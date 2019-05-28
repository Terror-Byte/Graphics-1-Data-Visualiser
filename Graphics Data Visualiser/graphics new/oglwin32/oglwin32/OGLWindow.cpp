#include "OGLWindow.h"
#include "Resource.h"
#include <gl/GL.h>
#include <iostream>

OGLWindow::OGLWindow()
{

}

OGLWindow::~OGLWindow()
{
	//DestroyOGLContext();
	
	//Clean up the renderable
	delete barChart;
	delete pieChart;
	delete lineChart;
	delete scatter2D;
	delete scatter3D;
}

OGLWindow::OGLWindow(HINSTANCE hInstance, int width, int height)
{
	InitWindow(hInstance, width, height);

	Vector2D* graphOrigin = new Vector2D(100, height - 100);
	Vector2D* pieChartOrigin = new Vector2D(width / 2, height / 2);
	//Vector3D* scatterOrigin = new Vector3D(100, height - 100, 50);
	Vector3D* scatterOrigin = new Vector3D(0, -(height/4), 0);

	barChart = new OGLBarChart(width, height, *graphOrigin);
	pieChart = new OGLPieChart(width, height, *pieChartOrigin);
	lineChart = new OGLLineChart(width, height, *graphOrigin);
	scatter2D = new OGLScatter2D(width, height, *graphOrigin);
	scatter3D = new OGLScatter3D(width, height, *scatterOrigin);
}

HGLRC OGLWindow::CreateOGLContext(HDC hdc)
{
	unsigned int pixelformat;
	HGLRC hglrc;

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Support Window
		PFD_SUPPORT_OPENGL |						// Format Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request RGBA Format
		32,											// Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits mask
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// Z-Buffer depth
		0,											// Stencil Buffer
		0,											// Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(pixelformat=ChoosePixelFormat(hdc,&pfd)))
	{
		return 0;
	}

	if(!SetPixelFormat(hdc,pixelformat,&pfd))
	{
		return 0;
	}

	if (!(hglrc=wglCreateContext(hdc)))
	{
		return 0;
	}

	if(!wglMakeCurrent(hdc,hglrc))
	{
		return 0;
	}

	return hglrc;
}

void OGLWindow::DestroyOGLWindow()
{
	DestroyOGLContext();

	DestroyWindow( m_hwnd );

	m_hwnd = NULL;
	m_hdc = NULL;
}

BOOL OGLWindow::DestroyOGLContext()
{
	if ( m_hglrc )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( m_hglrc );
		m_hglrc = NULL;
	}
	
	if ( !ReleaseDC ( m_hwnd, m_hdc ) )
		return FALSE;

	return TRUE;
}

BOOL OGLWindow::InitWindow(HINSTANCE hInstance, int width, int height)
{
	m_hwnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		L"OGLWindow", L"OGLWindow", WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if ( ! m_hwnd )
		return FALSE;

	m_hdc = GetDC( m_hwnd );

	if ( !(m_hglrc = CreateOGLContext( m_hdc )) )
		return FALSE;
	
	m_width = width;
	m_height = height;

	return TRUE;
}

void OGLWindow::SetVisible ( BOOL visible )
{
	ShowWindow ( m_hwnd, visible? SW_SHOW : SW_HIDE );
}

void OGLWindow::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//INBETWEEN THIS
	glLoadIdentity();

	if (barChart != NULL && barChart->GetCanRender())
	{
		barChart->Render();
	}
	
	if (pieChart != NULL && pieChart->GetCanRender())
	{
		pieChart->Render();
	}

	if (lineChart != NULL && lineChart->GetCanRender())
	{
		lineChart->Render();
	}

	if (scatter2D != NULL && scatter2D->GetCanRender())
	{
		scatter2D->Render();
	}

	if (scatter3D != NULL && scatter3D->GetCanRender())
	{
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		scatter3D->Render();
		glDisable(GL_DEPTH_TEST);
	}

	glEnd(); 
	glFlush();
	//AND THIS

	SwapBuffers(m_hdc);
	return;
}

void OGLWindow::Resize( int width, int height )
{
	m_width = width;
	m_height = height;

	glViewport( 0, 0, width, height );
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glOrtho( -0.5*width, 0.5*width, -0.5*height, 0.5*height, -1000.0, 1000.0);
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	return;
}

void OGLWindow::InitOGLState()
{
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}

BOOL OGLWindow::MouseLBDown ( int x, int y )
{
	barChart->MouseLBDown(XCoordConvert(x), YCoordConvert(y));
	pieChart->MouseLBDown(XCoordConvert(x), YCoordConvert(y));
	lineChart->MouseLBDown(XCoordConvert(x), YCoordConvert(y));
	scatter2D->MouseLBDown(XCoordConvert(x), YCoordConvert(y));
	scatter3D->MouseLBDown(XCoordConvert(x), YCoordConvert(y));

	return TRUE;
}

BOOL OGLWindow::MouseLBUp ( int x, int y )
{
	barChart->MouseLBUp(XCoordConvert(x), YCoordConvert(y));
	pieChart->MouseLBUp(XCoordConvert(x), YCoordConvert(y));
	lineChart->MouseLBUp(XCoordConvert(x), YCoordConvert(y));
	scatter2D->MouseLBUp(XCoordConvert(x), YCoordConvert(y));
	scatter3D->MouseLBUp(XCoordConvert(x), YCoordConvert(y));

	return TRUE;
}

BOOL OGLWindow::MouseMove ( int x, int y )
{
	barChart->MouseMove(XCoordConvert(x), YCoordConvert(y));
	pieChart->MouseMove(XCoordConvert(x), YCoordConvert(y));
	lineChart->MouseMove(XCoordConvert(x), YCoordConvert(y));
	scatter2D->MouseMove(XCoordConvert(x), YCoordConvert(y));
	scatter3D->MouseMove(XCoordConvert(x), YCoordConvert(y));

	return TRUE;
}

OGLBarChart* OGLWindow::GetBarChart()
{
	return barChart;
}

OGLPieChart* OGLWindow::GetPieChart()
{
	return pieChart;
}

OGLLineChart* OGLWindow::GetLineChart()
{
	return lineChart;
}

OGLScatter2D* OGLWindow::GetScatter2D()
{
	return scatter2D;
}

OGLScatter3D* OGLWindow::GetScatter3D()
{
	return scatter3D;
}

int OGLWindow::XCoordConvert(int input)
{
	return (input - (m_width / 2));
}

int OGLWindow::YCoordConvert(int input)
{
	return (-input + (m_height / 2));
}