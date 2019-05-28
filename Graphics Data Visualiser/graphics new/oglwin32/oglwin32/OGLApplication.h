#pragma once

#include <Windows.h>
#include "ListHead.h"
#include "ListNode.h"

class OGLWindow;

//Define OGLApplication as a singleton
class OGLApplication
{
	private:
		OGLWindow*					m_appwnd;
		
		BOOL						m_terminate;
		static OGLApplication*		s_oglapp;

									OGLApplication();
		virtual						~OGLApplication();
		void						CreateApplicationWindow(int width, int height );
		BOOL						MyRegisterClass(HINSTANCE hinst);

		int							zoomNo = 0;
		void						FillComboBox(ListHead* head, HWND comboBox);
		void						ResetComboBoxes();
		void						ClearGraphs();
		void						ResetPos();

	public:

		HINSTANCE					m_hInst;

		static OGLApplication*		CreateApplication(HINSTANCE hInst);
		static void					DestroyApplication();
		static OGLApplication*		GetApplication();

		
		int							Run();
		void						Kill();
		

		inline OGLWindow*			GetApplicationWindow()
		{
			return m_appwnd;
		}

		static	LRESULT CALLBACK	WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		static  DLGPROC	CALLBACK	DialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};