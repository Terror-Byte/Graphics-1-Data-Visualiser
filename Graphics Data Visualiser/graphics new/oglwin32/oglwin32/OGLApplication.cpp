#include "OGLApplication.h"
#include "OGLWindow.h"
#include "Resource.h"
#include <Windowsx.h>
#include <commdlg.h>
#include <iostream>
#include "GLFont.h"
#include "GUIText.h"
#include "FileReader.h"

OPENFILENAME ofn;
wchar_t szFile[200];
FileReader* fReader = NULL;
char delim = NULL;

HWND graphDialog = 0;
HWND graphCombo = 0;
HWND graphItemCombo1 = 0;
HWND graphItemCombo2 = 0;
HWND graphItemCombo3 = 0;

OGLBarChart* barChart = NULL;
OGLPieChart* pieChart = NULL;
OGLLineChart* lineChart = NULL;
OGLScatter2D* scatter2D = NULL;
OGLScatter3D* scatter3D = NULL;
ListHead* initialHead = NULL;

OGLApplication* OGLApplication::s_oglapp = NULL;

OGLApplication::OGLApplication()
{
	m_appwnd = NULL;
	m_hInst = 0;
	m_terminate = FALSE;
	delim = ',';
}

OGLApplication::~OGLApplication()
{
	if ( m_appwnd )
		delete m_appwnd;
}

BOOL OGLApplication::MyRegisterClass(HINSTANCE hinst)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= this->WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hinst;
	wcex.hIcon			= LoadIcon(hinst, MAKEINTRESOURCE(IDI_OGLWIN32));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_OGLWIN32);
	wcex.lpszClassName	= L"OGLWindow";
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	graphDialog = CreateDialog(hinst, MAKEINTRESOURCE(IDD_DLGGRAPH), 0, (DLGPROC)DialogProc);
	graphCombo = GetDlgItem(graphDialog, IDC_COMBOGRAPH);
	graphItemCombo1 = GetDlgItem(graphDialog, IDC_COMBOE1);
	graphItemCombo2 = GetDlgItem(graphDialog, IDC_COMBOE2);
	graphItemCombo3 = GetDlgItem(graphDialog, IDC_COMBOE3);

	SendMessage(graphCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Bar Chart"));
	SendMessage(graphCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Pie Chart"));
	SendMessage(graphCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"Line Chart"));
	SendMessage(graphCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"2D Scatter Plot"));
	SendMessage(graphCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"3D Scatter Plot"));
	SendMessage(graphCombo, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	
	if ( !RegisterClassEx ( &wcex ) )
		return FALSE;

	return TRUE;
}

OGLApplication* OGLApplication::CreateApplication(HINSTANCE hinst)
{
	if ( ! s_oglapp )
	{
		s_oglapp = new OGLApplication();

		s_oglapp->m_hInst = hinst;
		s_oglapp->MyRegisterClass(hinst);

		//Now create an OGLWindow for this application
		s_oglapp->CreateApplicationWindow(1024,768);
	}

	return s_oglapp;
}

void OGLApplication::DestroyApplication()
{
	if ( s_oglapp )
		delete s_oglapp;
}

OGLApplication* OGLApplication::GetApplication()
{
	return s_oglapp;
}

void OGLApplication::CreateApplicationWindow( int width, int height )
{
	if ( !m_appwnd  )
	{
		m_appwnd = new OGLWindow ( m_hInst, width, height );
		m_appwnd->SetVisible(TRUE);

		barChart = m_appwnd->GetBarChart();
		pieChart = m_appwnd->GetPieChart();
		lineChart = m_appwnd->GetLineChart();
		scatter2D = m_appwnd->GetScatter2D();
		scatter3D = m_appwnd->GetScatter3D();
	}
}

int OGLApplication::Run()
{
	MSG msg;

	while ( !m_terminate )
	{
		if ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			//peek for windows message
			if ( msg.message == WM_QUIT )
			{
				Kill();
			}
			else
			{
				TranslateMessage ( &msg );
				DispatchMessage ( &msg );
			}
		}
		else
		{
			//get the OGLWindow to render stuff;
			m_appwnd->Render();
		}
	}

	return (int) msg.wParam;
}

void OGLApplication::Kill()
{
	m_terminate = TRUE;
}

LRESULT CALLBACK OGLApplication::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	int wmId, wmEvent, zDelta;

	switch ( msg )
	{
		case WM_SIZE:
			s_oglapp->GetApplicationWindow()->Resize( LOWORD(lparam), HIWORD(lparam) );
			break;

		case WM_CLOSE:
			s_oglapp->GetApplicationWindow()->DestroyOGLWindow();
			break;

		case WM_MOUSEMOVE:
			//inform the cursor position to OGLWindow
			s_oglapp->GetApplicationWindow()->MouseMove( GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) );
			break;

		case WM_LBUTTONUP:
			s_oglapp->GetApplicationWindow()->MouseLBUp( GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) );
			break;

		case WM_LBUTTONDOWN:
			s_oglapp->GetApplicationWindow()->MouseLBDown( GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) );
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_KEYDOWN:
			if (wparam == 82)
					s_oglapp->ResetPos();

			//switch (wparam)
			//{
			//	case 82:
			//		s_oglapp->ResetPos();
			//		break;
			//	case 37: // Left
			//		
			//		break;
			//	case 38: // Up
			//		break;
			//	case 39: // Right
			//		break;
			//	case 40: // Down
			//		break;
			//}
			break;

		case WM_MOUSEWHEEL:
			zDelta = GET_WHEEL_DELTA_WPARAM(wparam);
			if (barChart != NULL)
			{
				if (zDelta > 0)
				{
					barChart->SetZoom(barChart->GetZoom() + 0.1);
					pieChart->SetZoom(pieChart->GetZoom() + 0.1);
					lineChart->SetZoom(lineChart->GetZoom() + 0.1);
					scatter2D->SetZoom(scatter2D->GetZoom() + 0.1);
					scatter3D->SetZoom(scatter3D->GetZoom() + 0.1);
				}
				else if (zDelta < 0 && barChart->GetZoom() > 0.1)
				{
					barChart->SetZoom(barChart->GetZoom() - 0.1);
					pieChart->SetZoom(pieChart->GetZoom() - 0.1);
					lineChart->SetZoom(lineChart->GetZoom() - 0.1);
					scatter2D->SetZoom(scatter2D->GetZoom() - 0.1);
					scatter3D->SetZoom(scatter3D->GetZoom() - 0.1);
				}
			}
			break;

		case WM_COMMAND:
			switch (LOWORD(wparam)){
				case IDM_EXIT:
					SendMessage(hwnd, WM_CLOSE, 0, 0);
					break;
				case IDM_ABOUT:
					std::cout << "You pressed dat about button yo" << std::endl;
					break;
				case IDM_FILE_OPEN:	
					GetOpenFileName(&ofn);
					fReader = new FileReader(szFile, delim);

					if (fReader->Read())
					{			
						std::cout << "READ SUCCESSFUL" << std::endl;
						fReader->PrintEntry(-1);
						initialHead = fReader->GetInitialHead();
						s_oglapp->FillComboBox(initialHead, graphItemCombo1);
						s_oglapp->FillComboBox(initialHead, graphItemCombo2);

						barChart->SetCanRender(FALSE);
						pieChart->SetCanRender(FALSE);
						lineChart->SetCanRender(FALSE);
						scatter2D->SetCanRender(FALSE);
						scatter3D->SetCanRender(FALSE);
						ShowWindow(graphDialog, 4);
						//REMOVE
						// Add methods to clear the variables inside the various charts						
						// Bar Chart
						//barChart->ParseData(initialHead, 2);
						//barChart->GraphInitialise();
						//barChart->SetCanRender(TRUE);*/
						// Pie Chart
						//pieChart->ParseData(initialHead, 3);
						//pieChart->GraphInitialise();
						//pieChart->SetCanRender(TRUE);
						// Scatter 2D
					    //scatter2D->ParseData2E(initialHead, 1, 2);
						//scatter2D->GraphInitialise();
						//scatter2D->SetCanRender(TRUE);
						// Scatter 3D
						//scatter3D->ParseData3E(initialHead, 1, 2, 3);
						//scatter3D->SetProjection();
						//scatter3D->GraphInitialise();
						//scatter3D->SetCanRender(TRUE);
						// Line Chart
						//lineChart->ParseData(initialHead, 2);
						//lineChart->GraphInitialise();
						//lineChart->SetCanRender(TRUE);
						//ShowWindow(graphDialog, 4);
					}
					else
					{
						std::cout << "READ FAIL" << std::endl;
					}
					break;
				case IDM_COMMA:					
					delim = ',';
					break;
				case IDM_SEMICOLON:
					delim = ';';
					break;
			}
			break;

		default:
			return DefWindowProc( hwnd, msg, wparam, lparam );
	}

	return 0;
}

DLGPROC CALLBACK OGLApplication::DialogProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wparam)){
			case IDOK:		
				barChart->SetCanRender(FALSE);
				pieChart->SetCanRender(FALSE);
				lineChart->SetCanRender(FALSE);
				scatter2D->SetCanRender(FALSE);
				scatter3D->SetCanRender(FALSE);

				switch (SendMessage(graphCombo, CB_GETCURSEL, (WPARAM)0, (LPARAM)0))
				{
					// Scatter 2D
					case 0:
						s_oglapp->ClearGraphs();
						scatter2D->ParseData2E(initialHead, SendMessage(graphItemCombo1, CB_GETCURSEL, (WPARAM)0, (LPARAM)0), SendMessage(graphItemCombo2, CB_GETCURSEL, (WPARAM)0, (LPARAM)0));
						scatter2D->GraphInitialise();
						scatter2D->SetCanRender(TRUE);	
						break;

					// Scatter 3D
					case 1:
						s_oglapp->ClearGraphs();
						scatter3D->ParseData3E(initialHead, SendMessage(graphItemCombo1, CB_GETCURSEL, (WPARAM)0, (LPARAM)0), SendMessage(graphItemCombo2, CB_GETCURSEL, (WPARAM)0, (LPARAM)0), SendMessage(graphItemCombo3, CB_GETCURSEL, (WPARAM)0, (LPARAM)0));
//						scatter3D->SetProjection();
						scatter3D->GraphInitialise();
						scatter3D->SetCanRender(TRUE);
						break;

					// Bar Chart
					case 2:
						s_oglapp->ClearGraphs();
						barChart->ParseData(initialHead, SendMessage(graphItemCombo1, CB_GETCURSEL, (WPARAM)0, (LPARAM)0));
						barChart->GraphInitialise();
						barChart->SetCanRender(TRUE);
						break;

					// Line Chart
					case 3:
						s_oglapp->ClearGraphs();
						lineChart->ParseData(initialHead, SendMessage(graphItemCombo1, CB_GETCURSEL, (WPARAM)0, (LPARAM)0));
						lineChart->GraphInitialise();
						lineChart->SetCanRender(TRUE);
						break;

					// Pie Chart
					case 4:			
						s_oglapp->ClearGraphs();
						pieChart->ParseData(initialHead, SendMessage(graphItemCombo1, CB_GETCURSEL, (WPARAM)0, (LPARAM)0));
						pieChart->GraphInitialise();
						pieChart->SetCanRender(TRUE);
						break;
				}
				break;
			case IDCANCEL:
				std::cout << "You pressed Cancel!" << std::endl;
				break;
			case IDC_COMBOGRAPH:
				switch (SendMessage(graphCombo, CB_GETCURSEL, (WPARAM)0, (LPARAM)0))
				{
					// Scatter 2D
					case 0:
						s_oglapp->ResetComboBoxes();
						s_oglapp->FillComboBox(initialHead, graphItemCombo1);
						s_oglapp->FillComboBox(initialHead, graphItemCombo2);
						break;

					// Scatter 3D
					case 1:
						s_oglapp->ResetComboBoxes();
						s_oglapp->FillComboBox(initialHead, graphItemCombo1);
						s_oglapp->FillComboBox(initialHead, graphItemCombo2);
						s_oglapp->FillComboBox(initialHead, graphItemCombo3);
						break;

					// Bar Chart
					case 2:
						s_oglapp->ResetComboBoxes();
						s_oglapp->FillComboBox(initialHead, graphItemCombo1);
						break;

					// Line Chart
					case 3:
						s_oglapp->ResetComboBoxes();
						s_oglapp->FillComboBox(initialHead, graphItemCombo1);
						break;

					// Pie Chart
					case 4:
						s_oglapp->ResetComboBoxes();
						s_oglapp->FillComboBox(initialHead, graphItemCombo1);
						break;
				}
				break;
			case IDC_COMBOE1:
				break;
			case IDC_COMBOE2:
				break;
			case IDC_COMBOE3:
				break;
		}
	}
	return 0;
}

void OGLApplication::FillComboBox(ListHead* head, HWND comboBox)
{
	ListNode* currentNode = head->nextNode;
	wchar_t stringArray[200];

	while (currentNode != NULL)
	{
		mbstowcs(stringArray, currentNode->GetString().c_str(), 200);
		SendMessage(comboBox, CB_INSERTSTRING, (WPARAM)-1, reinterpret_cast<LPARAM>(stringArray));
		currentNode = currentNode->nextNode;
	}
}

void OGLApplication::ResetComboBoxes()
{
	SendMessage(graphItemCombo1, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	SendMessage(graphItemCombo2, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	SendMessage(graphItemCombo3, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
}

void OGLApplication::ClearGraphs()
{
	s_oglapp->ResetPos();
	scatter2D->ClearGraph();
	scatter3D->ClearGraph();
	barChart->ClearGraph();
	lineChart->ClearGraph();
	pieChart->ClearGraph();
}

void OGLApplication::ResetPos()
{
	barChart->CentreGraph();
	barChart->SetZoom(1.0f);

	pieChart->CentreGraph();
	pieChart->SetZoom(1.0f);

	lineChart->CentreGraph();
	lineChart->SetZoom(1.0f);

	scatter2D->CentreGraph();
	scatter2D->SetZoom(1.0f);

	scatter3D->CentreGraph();
	scatter3D->SetZoom(0.5f);
}