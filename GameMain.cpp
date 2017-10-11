#pragma once

#include"GridMgr.h"


CGridMgr g_MagicBall;
////////////////////////////////////////////////////////////////////////////////
//                                   WinProc                                  //
////////////////////////////////////////////////////////////////////////////////

DWORD m_dLastClickTime = 0;
DWORD m_nClickStatus = 0;

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) //回调函数 消息处理函数
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		exit(0);

	case WM_CHAR:
		break;
	case WM_LBUTTONDOWN:
		{
			// 双击操作
			if(  timeGetTime() - m_dLastClickTime < 300)
			{
				m_nClickStatus++;

				if(m_nClickStatus == 1)
				{
					break;
				}
			}
			else
			{
				m_dLastClickTime =  timeGetTime();
				m_nClickStatus = 0;
			}
			// end
			g_MagicBall.OnButtonDown();
		}
		break;
	case WM_RBUTTONDOWN:
	{
		g_MagicBall.OnRButtonDown();
	}
		break;
	case WM_MOUSEMOVE:
		{
			g_MagicBall.OnMouseMove();
		}
		break;
	case WM_SIZE:
		 {
			g_MagicBall.RepeatWindow();
		 }
		break;
	case WM_KEYDOWN:
		//组合键
		if (::GetKeyState(VK_CONTROL) < 0 && ::GetKeyState(VK_F1) < 0)
		{
			break;
		}
		//end
		switch(wParam)
		{
		case VK_ESCAPE:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
		return 0;

	}
	return DefWindowProc(hWnd,message,wParam,lParam);
}

////////////////////////////////////////////////////////////////////////////////
//              InitApp - Create the window and the objects                   //
////////////////////////////////////////////////////////////////////////////////

BOOL InitApp(HINSTANCE hInst, int nCmdShow)
{
	HWND hWnd;
	WNDCLASS WndClass;

	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInst;
	WndClass.hIcon = LoadIcon(0, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(0, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = 0;
	WndClass.lpszClassName = "MagicBall";
	//WndClass.style = CS_HREDRAW|CS_VREDRAW;
	RegisterClass(&WndClass);

	hWnd = CreateWindowEx(
		WS_EX_DLGMODALFRAME,
		"MagicBall",
		"Astar",
		WS_OVERLAPPEDWINDOW,
		300,100,
		620,
		540,
		NULL,
		NULL,
		hInst,
		NULL);
	//hWnd = CreateWindow("MagicBall", "MagicBall",
	//	WS_OVERLAPPEDWINDOW,
	//	0, 0,
	//	600,
	//	550,
	//	0, 0, GetModuleHandle(0), 0);

	if ( !hWnd ) return FALSE;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	g_MagicBall.InitGame(hWnd);
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//                                     WinMain                                //
////////////////////////////////////////////////////////////////////////////////

int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
				   LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	

	if ( !InitApp(hInst, nCmdShow) ) return FALSE;

	////////////////////////////////
	while(true)// Mainloop
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
			{
				break;
			}
			TranslateMessage(&msg); //消息转换
			DispatchMessage(&msg); //消息转换
		}
		else
		{

			//////////////////////////////////////////////////////////
			//g_MagicBall.RendGrid();
			//g_MagicBall.RendActItem();
			/////////////////////////////////////////////////////////

		}
	}
	return 0;
}