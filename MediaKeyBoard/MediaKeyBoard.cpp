// MediaKeyBoard.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "MediaKeyBoard.h"
#include "TaskBarIcon.h"
#include <ShellAPI.h>
#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

const int VUp=1;
const int VDown=2;
const int Pre=3;
const int Next=4;
const int PlayPause=5;
const int Stop=6;
const int Internet=7;
const int Resource=8;
const int Mute=9;
const int Lock=10;
const int WinRun=11;

bool showed;
HICON hIcon;
HMENU hMenu;
HFONT hFont;

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
void SetAutoRun();
void SetMenuState();
void DrawShadeRect(HDC hDC,RECT rect,COLORREF cr1,COLORREF cr2,TCHAR *text);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MEDIAKEYBOARD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MEDIAKEYBOARD));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MEDIAKEYBOARD));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MEDIAKEYBOARD));

	hIcon=LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_MEDIAKEYBOARD));
	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����
	// ʹ���ھ�����ʾ
   short x=GetSystemMetrics(SM_CXSCREEN)/2-160;
   short y=GetSystemMetrics(SM_CYSCREEN)/2-135;
   hWnd = CreateWindow(szWindowClass, "��ý�����ģ����", WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
      x, y, 320,270, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
	// ע��ϵͳ�ȼ�
   RegisterHotKey(hWnd,VUp,MOD_ALT|MOD_CONTROL,VK_NUMPAD8);
   RegisterHotKey(hWnd,VDown,MOD_ALT|MOD_CONTROL,VK_NUMPAD2);
   RegisterHotKey(hWnd,Pre,MOD_ALT|MOD_CONTROL,VK_NUMPAD4);
   RegisterHotKey(hWnd,Next,MOD_CONTROL|MOD_ALT,VK_NUMPAD6);
   RegisterHotKey(hWnd,PlayPause,MOD_ALT|MOD_CONTROL,VK_NUMPAD5 );
   RegisterHotKey(hWnd,Stop,MOD_ALT|MOD_CONTROL,VK_NUMPAD7);
   RegisterHotKey(hWnd,Mute,MOD_ALT|MOD_CONTROL,VK_NUMPAD9);
   RegisterHotKey(hWnd,Internet,MOD_ALT|MOD_CONTROL,VK_NUMPAD3);
   RegisterHotKey(hWnd,Resource,MOD_ALT|MOD_CONTROL,VK_NUMPAD1);
   RegisterHotKey(hWnd,Lock,MOD_ALT|MOD_CONTROL,VK_NUMPAD0);
   RegisterHotKey(hWnd,WinRun,MOD_ALT|MOD_CONTROL,VK_RETURN);

   HMENU hMenu=GetSystemMenu(hWnd,false);
   EnableMenuItem(hMenu,6,MF_BYPOSITION|MF_DISABLED);
   AppendMenu(hMenu,MF_SEPARATOR,NULL,"");
   AppendMenu(hMenu,MF_STRING|MF_OWNERDRAW,1006,"����");
   hFont=CreateFont(12,6,0,0,0,0,0,0,0,0,0,0,0,"����");

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	POINT p;
	HDC hdc;
	RECT rect;
	static HWND btn;
	static LONG lStyle;
	HICON hIcon = ::LoadIcon(hInst,MAKEINTRESOURCE(IDI_MEDIAKEYBOARD));
	
	static char *show[]=
	{
		"��ý�����ģ������������ߣ���ʿ��",
		"��Ӧ��ݼ�����(NUMPAD����С���ּ���)��",
		"CTRL+ALT+NUMPAD8������������",
		"CTRL+ALT+NUMPAD2����С������",
		"CTRL+ALT+NUMPAD4����һ�ף�",
		"CTRL+ALT+NUMPAD6����һ�ף�",
		"CTRL+ALT+NUMPAD5������/��ͣ��",
		"CTRL+ALT+NUMPAD7��ֹͣ���ţ�",
		"CTRL+ALT+NUMPAD9��������",
		"CTRL+ALT+NUMPAD1���ҵĵ��ԣ�",
		"CTRL+ALT+NUMPAD3��Internet�����",
		"CTRL+ALT+NUMPAD0������",
		"CTRL+ALT+ENTER��  ���жԻ���",
	};
	int x;

	switch (message)
	{
	case WM_CREATE:
		hMenu=LoadMenu(NULL,MAKEINTRESOURCE(IDR_MENU1));
		hMenu=GetSubMenu(hMenu,0);
		ModifyMenu(hMenu,0,MF_BYPOSITION|MF_OWNERDRAW,ID_TASKBAR_ABOUT,"��ʾ");
		ModifyMenu(hMenu,2,MF_BYPOSITION|MF_OWNERDRAW,ID_TASKBAR_EXIT,"��ʾ");

// 		SetMenuItemInfo(hMenu,0,true,)
		SetTimer(hWnd,1,400,NULL);
		btn=CreateWindow("button","ȷ��",WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,262,220,50,35,hWnd,(HMENU)1007,NULL,NULL);
		// �����ޱ���������
		lStyle=GetWindowLong(hWnd,GWL_STYLE);
		SetWindowLong(hWnd,GWL_STYLE,lStyle&~WS_CAPTION);
		SetWindowPos(hWnd,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);

		break;
	case WM_LBUTTONDOWN:
		// ʵ���ޱ��������϶�
		GetCursorPos(&p);
		PostMessage(hWnd,WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(p.x,p.y));
		break;
	case WM_TIMER:
		SendMessage(hWnd,WM_SIZE,SIZE_MINIMIZED,NULL);
		TaskBarIcon::ShowInfo(hWnd,"��ʾ��Ϣ","��ӭʹ�ã���ý�����ģ����");
		KillTimer(hWnd,1);
		break;
	case WM_SYSCOMMAND:
		switch(wParam)
		{
		case 1006:
			MessageBox(hWnd,"��ý�����ģ������������ߣ���ʿ��\r\n���̨ʽ���Եı�׼���̶���ơ�","�������",64);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	case WM_COMMAND:
		switch(wParam)
		{
		case 1007:
			SendMessage(hWnd,WM_SIZE,SIZE_MINIMIZED,NULL);
			break;
		case ID_TASKBAR_AUTORUN:
			SetAutoRun();
			break;
		case ID_TASKBAR_ABOUT:
			ShowWindow(hWnd,SW_NORMAL);
			SetForegroundWindow(hWnd);
			break;
		case ID_TASKBAR_EXIT:
			TaskBarIcon::DeleteTaskBar(hWnd);
			exit(0);
			break;
		}
		break;
	case WM_CTLCOLORBTN:
		return SetTextColor((HDC)wParam,RGB(255,0,0));
		break;
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		GetClientRect(hWnd,&rect);
		DrawShadeRect(hdc,rect,RGB(255,0,100),RGB(0,100,255),"");
		SetBkMode(hdc,TRANSPARENT);
		SelectObject(hdc,hFont);
		for (x=0;x<13;x++)
		{
			if (x%2==0)
			{		
				SetTextColor(hdc,RGB(255,255,255));
			}
			else
			{
				SetTextColor(hdc,RGB(0,0,255));
			}
			TextOut(hdc,10,10+x*20,show[x],lstrlen(show[x]));
		}
		EndPaint(hWnd,&ps);
	case WM_ICON_NOTIFY:
		if (lParam==WM_LBUTTONDOWN)
		{
			ShowWindow(hWnd,SW_NORMAL);
			SetForegroundWindow(hWnd);
		}
		if (lParam==WM_RBUTTONDOWN)
		{
			GetCursorPos(&p);
			SetForegroundWindow(hWnd);
			SetMenuState();
			TrackPopupMenu(hMenu,TPM_RIGHTBUTTON,p.x,p.y,0,hWnd,0);
		}
		break;
	case WM_SIZE:
		if (wParam==SIZE_MINIMIZED)
		{
			TaskBarIcon::AddTaskBarIcon(hWnd,hIcon);
			ShowWindow(hWnd,SW_HIDE);
		}
		if (wParam==SIZE_RESTORED)
		{
			TaskBarIcon::DeleteTaskBar(hWnd);
			ShowWindow(hWnd,SW_NORMAL);
			SetForegroundWindow(hWnd);
		}
		break;
	case WM_HOTKEY:
		switch(wParam)
		{
		case VUp:
			keybd_event(VK_VOLUME_UP,NULL,NULL,NULL);
			break;
		case VDown:
			keybd_event(VK_VOLUME_DOWN,NULL,NULL,NULL);
			break;
		case Pre:
			keybd_event(VK_MEDIA_PREV_TRACK,NULL,NULL,NULL);
			break;
		case Next:
			keybd_event(VK_MEDIA_NEXT_TRACK,NULL,NULL,NULL);
			break;
		case PlayPause:
			keybd_event(VK_MEDIA_PLAY_PAUSE,NULL,NULL,NULL);
			break;
		case Stop:
			keybd_event(VK_MEDIA_STOP,NULL,NULL,NULL);
			break;
		case Mute:
			keybd_event(VK_VOLUME_MUTE,NULL,NULL,NULL);
			break;
		case Internet:
			keybd_event(VK_BROWSER_HOME,NULL,NULL,NULL);
			break;
		case Resource:
			ShellExecuteA(hWnd,"open","::{20D04FE0-3AEA-1069-A2D8-08002B30309D}",NULL,NULL,SW_MAXIMIZE);
			break;
		case Lock:
			ShellExecute(0,"open","rundll32.exe","USER32,LockWorkStation","",SW_HIDE);;
			break;
		case WinRun:
			Sleep(200);
			keybd_event(VK_LWIN, 0, 0, 0); //����LWIN
			keybd_event('R', 0, 0, 0); //����R
			Sleep(200);
			keybd_event(VK_LWIN, 0, 2, 0);  //�ͷ�LWIN
			keybd_event('R', 0, 2, 0);        //�ͷ�R 
			break;
		}
		break;
	case WM_DESTROY:
		TaskBarIcon::DeleteTaskBar(hWnd);
		PostQuitMessage(0);
		break;
	case WM_DRAWITEM:
		switch(((LPDRAWITEMSTRUCT)lParam)->CtlID)
		{
		case 1007:		
			DrawShadeRect(((LPDRAWITEMSTRUCT)lParam)->hDC,((LPDRAWITEMSTRUCT)lParam)->rcItem,RGB(255,0,0),RGB(0,0,255),"ȷ��");
			break;
		}
		switch (((LPDRAWITEMSTRUCT)lParam)->itemID)
		{	
		case 1006:
			DrawShadeRect(((LPDRAWITEMSTRUCT)lParam)->hDC,((LPDRAWITEMSTRUCT)lParam)->rcItem,RGB(255,0,0),RGB(0,0,255),"����");
			break;
		case ID_TASKBAR_ABOUT:
			DrawShadeRect(((LPDRAWITEMSTRUCT)lParam)->hDC,((LPDRAWITEMSTRUCT)lParam)->rcItem,RGB(255,0,0),RGB(0,0,255),"��ʾ������");
			break;
		case ID_TASKBAR_AUTORUN:
			DrawShadeRect(((LPDRAWITEMSTRUCT)lParam)->hDC,((LPDRAWITEMSTRUCT)lParam)->rcItem,RGB(255,0,0),RGB(0,0,255),"��Windows����");
			break;
		case ID_TASKBAR_EXIT:
			DrawShadeRect(((LPDRAWITEMSTRUCT)lParam)->hDC,((LPDRAWITEMSTRUCT)lParam)->rcItem,RGB(255,0,0),RGB(0,0,255),"�˳�����");
			break;
		}
// 		FillRect(((LPDRAWITEMSTRUCT)lParam)->hDC,&((LPDRAWITEMSTRUCT)lParam)->rcItem,CreateSolidBrush(RGB(255,0,0)));
		if (((LPDRAWITEMSTRUCT)lParam)->itemState & ODS_SELECTED)
		{
			InvertRect(((LPDRAWITEMSTRUCT)lParam)->hDC,&((LPDRAWITEMSTRUCT)lParam)->rcItem);
		}
		return (INT_PTR)TRUE;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void SetAutoRun()
{
	TCHAR file[100];
	DWORD size=100,type=REG_SZ;
	TCHAR temp[100];
	HKEY hKey;	
	GetModuleFileName(NULL,file,100);

	if (ERROR_SUCCESS!=RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",NULL,KEY_ALL_ACCESS,&hKey))
	{			
		MessageBox(0,"��ע���ʧ�ܣ�","������Ϣ",64);
		return;
	}
	// ���ж���������Ϣ�Ƿ����
	if (ERROR_SUCCESS==RegQueryValueEx(hKey,"MediaKeyBoard",NULL,&type,(BYTE*)temp,&size))
	{
		if (0==lstrcmp(file,temp))
		{
			if (ERROR_SUCCESS!=RegDeleteValue(hKey,"MediaKeyBoard"))
			{
				MessageBox(0,"��ɾ����������Ϣʱ��ʧ�ܣ�","������Ϣ",64);
				return;
			}
		}
		else
		{	
			if (ERROR_SUCCESS!=RegSetValueEx(hKey,"MediaKeyBoard",NULL,REG_SZ,(BYTE*)(&file),lstrlen(file)+1))
			{
				MessageBox(0,"��ע���д����������Ϣʱ��ʧ�ܣ�","������Ϣ",64);
				return;
			}
		}
	}
	else
	{
		if (ERROR_SUCCESS!=RegSetValueEx(hKey,"MediaKeyBoard",NULL,REG_SZ,(BYTE*)(&file),lstrlen(file)+1))
		{
			MessageBox(0,"��ע���д����������Ϣʱ��ʧ�ܣ�˳��˵һ�䰳��İ�κ�Ǻ죡","������Ϣ",64);
			return;
		}
	}
	RegCloseKey(hKey);
}
// ����ע������Ϣ�����ò˵���״̬��
void SetMenuState()
{
	TCHAR file[100];
	DWORD size=100,type=REG_SZ;
	TCHAR temp[100];
	HKEY hKey;	

	GetModuleFileName(NULL,file,100);

	if (ERROR_SUCCESS!=RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",NULL,KEY_ALL_ACCESS,&hKey))
	{			
		MessageBox(0,"��ע���ʧ�ܣ�","������Ϣ",64);
		return;
	}
	// ���ж���������Ϣ�Ƿ����
	if (ERROR_SUCCESS==RegQueryValueEx(hKey,"MediaKeyBoard",NULL,&type,(BYTE*)temp,&size))
	{
		if (0==lstrcmp(file,temp))
		{
			CheckMenuItem(hMenu,1,MF_BYPOSITION|MF_CHECKED);
		}
		else
		{
			CheckMenuItem(hMenu,1,MF_BYPOSITION|MF_UNCHECKED);
		}
	}
	else
	{
		CheckMenuItem(hMenu,1,MF_BYPOSITION|MF_UNCHECKED);
	}
	RegCloseKey(hKey);
}

// ���䱳��
void DrawShadeRect(HDC hDC,RECT rect,COLORREF cr1,COLORREF cr2,TCHAR *text)
{
	int r1=GetRValue(cr1);
	int g1=GetGValue(cr1);
	int b1=GetBValue(cr1);
	int r2=GetRValue(cr2);
	int g2=GetGValue(cr2);
	int b2=GetBValue(cr2);
	float dr = ((float)(r2 - r1))/(float)(rect.right-rect.left);
	float dg = ((float)(g2 - g1))/(float)(rect.right-rect.left);
	float db = ((float)(b2 - b1))/(float)(rect.right-rect.left);
	for(int i = rect.left; i < rect.right; i ++)
	{
		int r = r1 + (int)(dr*((float)(i - rect.left)));
		int g = g1 + (int)(dg*((float)(i - rect.left)));
		int b = b1 + (int)(db*((float)(i - rect.left)));
		HPEN hPen=CreatePen(PS_SOLID,1,RGB(r,g,b));
		SelectObject(hDC,hPen);
		MoveToEx(hDC,i,rect.top,NULL);
		LineTo(hDC,i,rect.bottom);
	}
	SetBkMode(hDC,TRANSPARENT);
	SetTextColor(hDC,RGB(255,255,255));
	DrawText(hDC,text,lstrlen(text),&rect,DT_CENTER|DT_VCENTER|DT_INTERNAL|DT_SINGLELINE);
}

// void DrawItem(LPDRAWITEMSTRUCT  lpDIS)
// {	
// 	HDC dc=
// 	CDC* pDC = CDC::FromHandle(lpDIS->hDC);	// get dc
// 	VERIFY(pDC);pDC->SetBkMode(TRANSPARENT);// get position
// 	CRect rcItem = lpDIS->rcItem;// get states
// 	UINT uState = lpDIS->itemState;	// the data of menu item is NULL then return
// 	if(lpDIS->itemData == NULL)	return;
// 	CString strText     = ((CMenuItemContext*)(lpDIS->itemData))->strText;
// 	UINT nMenuID        = ((CMenuItemContext*)(lpDIS->itemData))->nMenuID;
// 	CRect rcIcon(rcItem);
// 	rcIcon.right = rcIcon.left + Public_Area;
// 	CRect rcText(rcItem);
// 	rcText.left  = rcIcon.right;
// 	switch(nMenuID)
// 	{
// 	case -2:// -2:����˵���   
// 		if(uState&ODS_SELECTED)
// 		{
// 			DrawTop(pDC,rcItem,TRUE);
// 		}
// 		else
// 		{
// 			DrawTop(pDC,rcItem,FALSE);
// 		}
// 		DrawText(pDC,rcItem,strText);
// 		break;
// 	case -1://-1:�����˵�  
// 		strText= "\t   "+strText;
// 		if(uState&ODS_SELECTED)
// 			DrawBestRect(pDC,rcItem,0xff00ff,0xfffff,TRUE);
// 		else
// 			DrawTop(pDC,rcItem,TRUE);
// 		//DrawTop(pDC,rcItem,FALSE);
// 		DrawText(pDC,rcText,strText);
// 		break;
// 	case 0://0:�ָ���  
// 		rcText.top =rcText.Height()/2+rcText.top ;
// 		rcText.bottom =rcText.top +2;
// 		rcText.left +=2;
// 		rcText.right -=2;
// 		pDC->Draw3dRect(rcText,RGB(64,0,128),RGB(255,255,255));
// 		break;
// 	default://>0:һ��Ĳ˵�			
// 		if(uState&ODS_SELECTED)//��ѡ��
// 		{
// 			DrawBGColor(pDC,rcItem,TRUE);
// 		}
// 		else
// 		{
// 			DrawBGColor(pDC,rcItem,FALSE);//
// 		}
// 		int nIndex = GetIconIndexByID(nMenuID);
// 		if(uState&ODS_GRAYED)
// 		{
// 			if(nIndex >= 0) 
// 			{
// 				DrawIcon(pDC,rcItem,nIndex,TRUE);
// 			}
// 			DrawGray(pDC,rcText,strText);
// 			break;
// 		}
// 		else
// 		{
// 			if(nIndex >= 0) 
// 			{
// 				DrawIcon(pDC,rcItem,nIndex,FALSE);
// 			}
// 		}
// 		if(uState&ODS_CHECKED)//�Ǳ�check����,
// 		{
// 
// 			DrawText(pDC,rcText,"����"+strText);//��ǰ״̬��check!
// 			break;
// 		}
// 		else
// 		{
// 			DrawText(pDC,rcText,"       "+strText);//δѡȡ
// 			break;
// 		}
// 
// 		break;
// 	}
// }
