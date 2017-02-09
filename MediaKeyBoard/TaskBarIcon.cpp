#include "stdafx.h"
#include "TaskBarIcon.h"

void TaskBarIcon::AddTaskBarIcon(HWND hWnd,HICON hIcon)
{
	NOTIFYICONDATA tnid;
	tnid.cbSize=sizeof(NOTIFYICONDATA);
	tnid.hWnd=hWnd;
	tnid.uID=NULL;
	tnid.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP;
	tnid.uCallbackMessage=WM_ICON_NOTIFY;
	tnid.hIcon=hIcon;
	lstrcpyn(tnid.szTip,"¶àÃ½Ìå¼üÅÌÄ£ÄâÆ÷",sizeof(tnid.szTip));
	Shell_NotifyIconA(NIM_ADD,&tnid);
}

void TaskBarIcon::DeleteTaskBar(HWND hWnd)
{
	NOTIFYICONDATA tnid;
	tnid.cbSize=sizeof(NOTIFYICONDATA);
	tnid.hWnd=hWnd;
	tnid.uID=NULL;
	Shell_NotifyIconA(NIM_DELETE,&tnid);
}

void TaskBarIcon::ShowInfo(HWND hWnd,TCHAR *title,TCHAR *info)
{
	NOTIFYICONDATA tnid;
	tnid.cbSize=sizeof(NOTIFYICONDATA);
	tnid.hWnd=hWnd;
	tnid.uID=NULL;
	tnid.uFlags=NIF_INFO;
	tnid.dwInfoFlags=NIF_MESSAGE;
	lstrcpyn(tnid.szInfoTitle,title,sizeof(tnid.szInfoTitle));
	lstrcpyn(tnid.szInfo,info,sizeof(tnid.szInfo));
	Shell_NotifyIcon(NIM_MODIFY,&tnid);

}