#include "stdafx.h"
#include "MediaKeyBoard.h"
#include <ShellAPI.h>
#define WM_ICON_NOTIFY WM_USER+50

class TaskBarIcon
{
public:
	static void AddTaskBarIcon(HWND hWnd,HICON hIcon);
	static void DeleteTaskBar(HWND hWnd);
	static void ShowInfo(HWND hWnd,TCHAR *title,TCHAR *info);
};