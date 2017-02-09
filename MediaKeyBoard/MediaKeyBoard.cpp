// MediaKeyBoard.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MediaKeyBoard.h"
#include "TaskBarIcon.h"
#include <ShellAPI.h>
#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

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

// 此代码模块中包含的函数的前向声明:
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

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MEDIAKEYBOARD, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MEDIAKEYBOARD));

	// 主消息循环:
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
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
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
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中
	// 使窗口居中显示
   short x=GetSystemMetrics(SM_CXSCREEN)/2-160;
   short y=GetSystemMetrics(SM_CYSCREEN)/2-135;
   hWnd = CreateWindow(szWindowClass, "多媒体键盘模拟器", WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_MINIMIZEBOX,
      x, y, 320,270, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
	// 注册系统热键
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
   AppendMenu(hMenu,MF_STRING|MF_OWNERDRAW,1006,"关于");
   hFont=CreateFont(12,6,0,0,0,0,0,0,0,0,0,0,0,"宋体");

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
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
		"多媒体键盘模拟器，软件作者：翟士丹",
		"相应快捷键如下(NUMPAD代表小数字键盘)：",
		"CTRL+ALT+NUMPAD8：增加音量；",
		"CTRL+ALT+NUMPAD2：减小音量；",
		"CTRL+ALT+NUMPAD4：上一首；",
		"CTRL+ALT+NUMPAD6：下一首；",
		"CTRL+ALT+NUMPAD5：播放/暂停；",
		"CTRL+ALT+NUMPAD7：停止播放；",
		"CTRL+ALT+NUMPAD9：静音；",
		"CTRL+ALT+NUMPAD1：我的电脑；",
		"CTRL+ALT+NUMPAD3：Internet浏览器",
		"CTRL+ALT+NUMPAD0：锁屏",
		"CTRL+ALT+ENTER：  运行对话框",
	};
	int x;

	switch (message)
	{
	case WM_CREATE:
		hMenu=LoadMenu(NULL,MAKEINTRESOURCE(IDR_MENU1));
		hMenu=GetSubMenu(hMenu,0);
		ModifyMenu(hMenu,0,MF_BYPOSITION|MF_OWNERDRAW,ID_TASKBAR_ABOUT,"显示");
		ModifyMenu(hMenu,2,MF_BYPOSITION|MF_OWNERDRAW,ID_TASKBAR_EXIT,"显示");

// 		SetMenuItemInfo(hMenu,0,true,)
		SetTimer(hWnd,1,400,NULL);
		btn=CreateWindow("button","确定",WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,262,220,50,35,hWnd,(HMENU)1007,NULL,NULL);
		// 创建无标题栏窗口
		lStyle=GetWindowLong(hWnd,GWL_STYLE);
		SetWindowLong(hWnd,GWL_STYLE,lStyle&~WS_CAPTION);
		SetWindowPos(hWnd,NULL,0,0,0,0,SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOSIZE|SWP_NOZORDER|SWP_FRAMECHANGED);

		break;
	case WM_LBUTTONDOWN:
		// 实现无标题栏的拖动
		GetCursorPos(&p);
		PostMessage(hWnd,WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(p.x,p.y));
		break;
	case WM_TIMER:
		SendMessage(hWnd,WM_SIZE,SIZE_MINIMIZED,NULL);
		TaskBarIcon::ShowInfo(hWnd,"提示信息","欢迎使用，多媒体键盘模拟器");
		KillTimer(hWnd,1);
		break;
	case WM_SYSCOMMAND:
		switch(wParam)
		{
		case 1006:
			MessageBox(hWnd,"多媒体键盘模拟器，软件作者：翟士丹\r\n针对台式电脑的标准键盘而设计。","关于软件",64);
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
			keybd_event(VK_LWIN, 0, 0, 0); //按下LWIN
			keybd_event('R', 0, 0, 0); //按下R
			Sleep(200);
			keybd_event(VK_LWIN, 0, 2, 0);  //释放LWIN
			keybd_event('R', 0, 2, 0);        //释放R 
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
			DrawShadeRect(((LPDRAWITEMSTRUCT)lParam)->hDC,((LPDRAWITEMSTRUCT)lParam)->rcItem,RGB(255,0,0),RGB(0,0,255),"确定");
			break;
		}
		switch (((LPDRAWITEMSTRUCT)lParam)->itemID)
		{	
		case 1006:
			DrawShadeRect(((LPDRAWITEMSTRUCT)lParam)->hDC,((LPDRAWITEMSTRUCT)lParam)->rcItem,RGB(255,0,0),RGB(0,0,255),"关于");
			break;
		case ID_TASKBAR_ABOUT:
			DrawShadeRect(((LPDRAWITEMSTRUCT)lParam)->hDC,((LPDRAWITEMSTRUCT)lParam)->rcItem,RGB(255,0,0),RGB(0,0,255),"显示主窗口");
			break;
		case ID_TASKBAR_AUTORUN:
			DrawShadeRect(((LPDRAWITEMSTRUCT)lParam)->hDC,((LPDRAWITEMSTRUCT)lParam)->rcItem,RGB(255,0,0),RGB(0,0,255),"随Windows启动");
			break;
		case ID_TASKBAR_EXIT:
			DrawShadeRect(((LPDRAWITEMSTRUCT)lParam)->hDC,((LPDRAWITEMSTRUCT)lParam)->rcItem,RGB(255,0,0),RGB(0,0,255),"退出程序");
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
		MessageBox(0,"打开注册表失败！","错误信息",64);
		return;
	}
	// 先判断自启动信息是否存在
	if (ERROR_SUCCESS==RegQueryValueEx(hKey,"MediaKeyBoard",NULL,&type,(BYTE*)temp,&size))
	{
		if (0==lstrcmp(file,temp))
		{
			if (ERROR_SUCCESS!=RegDeleteValue(hKey,"MediaKeyBoard"))
			{
				MessageBox(0,"在删除自启动信息时，失败！","错误信息",64);
				return;
			}
		}
		else
		{	
			if (ERROR_SUCCESS!=RegSetValueEx(hKey,"MediaKeyBoard",NULL,REG_SZ,(BYTE*)(&file),lstrlen(file)+1))
			{
				MessageBox(0,"向注册表写入自启动信息时，失败！","错误信息",64);
				return;
			}
		}
	}
	else
	{
		if (ERROR_SUCCESS!=RegSetValueEx(hKey,"MediaKeyBoard",NULL,REG_SZ,(BYTE*)(&file),lstrlen(file)+1))
		{
			MessageBox(0,"向注册表写入自启动信息时，失败！顺便说一句俺真的爱魏亚红！","错误信息",64);
			return;
		}
	}
	RegCloseKey(hKey);
}
// 根据注册表的信息，设置菜单的状态。
void SetMenuState()
{
	TCHAR file[100];
	DWORD size=100,type=REG_SZ;
	TCHAR temp[100];
	HKEY hKey;	

	GetModuleFileName(NULL,file,100);

	if (ERROR_SUCCESS!=RegOpenKeyEx(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",NULL,KEY_ALL_ACCESS,&hKey))
	{			
		MessageBox(0,"打开注册表失败！","错误信息",64);
		return;
	}
	// 先判断自启动信息是否存在
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

// 渐变背景
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
// 	case -2:// -2:顶层菜单条   
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
// 	case -1://-1:弹出菜单  
// 		strText= "\t   "+strText;
// 		if(uState&ODS_SELECTED)
// 			DrawBestRect(pDC,rcItem,0xff00ff,0xfffff,TRUE);
// 		else
// 			DrawTop(pDC,rcItem,TRUE);
// 		//DrawTop(pDC,rcItem,FALSE);
// 		DrawText(pDC,rcText,strText);
// 		break;
// 	case 0://0:分隔条  
// 		rcText.top =rcText.Height()/2+rcText.top ;
// 		rcText.bottom =rcText.top +2;
// 		rcText.left +=2;
// 		rcText.right -=2;
// 		pDC->Draw3dRect(rcText,RGB(64,0,128),RGB(255,255,255));
// 		break;
// 	default://>0:一般的菜单			
// 		if(uState&ODS_SELECTED)//被选择，
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
// 		if(uState&ODS_CHECKED)//是被check过的,
// 		{
// 
// 			DrawText(pDC,rcText,"※√"+strText);//当前状态是check!
// 			break;
// 		}
// 		else
// 		{
// 			DrawText(pDC,rcText,"       "+strText);//未选取
// 			break;
// 		}
// 
// 		break;
// 	}
// }
