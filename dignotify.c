/*  Copyright (c) 2006 Dimitry Gashinsky. All rights reserved.  */
/*  */
/*  Author: Dimitry Gashinsky <dimitry@gashinsky.com>  */
/*  Id: 5fc0e5d5-9f92-4cb2-9b75-631579649715  */

#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include <winwlx.h>

BOOL RunCommand(HANDLE hToken, _TCHAR szCommand[]);
VOID OutputDebugLastError(_TCHAR szMessage[]);

//Entrance function for the DLL
BOOL WINAPI LibMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
      {
        DisableThreadLibraryCalls (hInstance);	
      }
      break;
    }

  return TRUE;
}

VOID APIENTRY DigLock (PWLX_NOTIFICATION_INFO pInfo)
{
  RunCommand(pInfo->hToken, _T("/C \"dignotify.cmd Lock\""));
}

VOID APIENTRY DigUnlock (PWLX_NOTIFICATION_INFO pInfo)
{
  RunCommand(pInfo->hToken, _T("/C \"dignotify.cmd Unlock\""));
}

VOID APIENTRY DigStartScreenSaver (PWLX_NOTIFICATION_INFO pInfo)
{
  RunCommand(pInfo->hToken, _T("/C \"dignotify.cmd StartScreenSaver\""));
}

VOID APIENTRY DigStopScreenSaver (PWLX_NOTIFICATION_INFO pInfo)
{
  RunCommand(pInfo->hToken, _T("/C \"dignotify.cmd StopScreenSaver\""));
}

VOID APIENTRY DigLogon (PWLX_NOTIFICATION_INFO pInfo)
{
  RunCommand(pInfo->hToken, _T("/C \"dignotify.cmd Logon\""));
}

VOID APIENTRY DigLogoff (PWLX_NOTIFICATION_INFO pInfo)
{
  RunCommand(pInfo->hToken, _T("/C \"dignotify.cmd Logoff\""));
}

VOID APIENTRY DigStartShell (PWLX_NOTIFICATION_INFO pInfo)
{
  RunCommand(pInfo->hToken, _T("/C \"dignotify.cmd StartShell\""));
}

VOID APIENTRY DigStartup (PWLX_NOTIFICATION_INFO pInfo)
{
  RunCommand(pInfo->hToken, _T("/C \"dignotify.cmd Startup\""));
}

VOID APIENTRY DigShutdown (PWLX_NOTIFICATION_INFO pInfo)
{
  RunCommand(pInfo->hToken, _T("/C \"dignotify.cmd Shutdown\""));
}

BOOL RunCommand(HANDLE hToken, _TCHAR szCommand[])
{
  BOOL success = FALSE;
  STARTUPINFO si;
  PROCESS_INFORMATION pi;
  _TCHAR tbuf[MAX_PATH*2];

  if (NULL != hToken)
    {
      si.cb = sizeof(STARTUPINFO); 
      si.lpReserved = NULL; 
      si.lpTitle = NULL; 
      si.lpDesktop = _T("winsta0\\default"); 
      si.dwX = si.dwY = si.dwXSize = si.dwYSize = 0L; 
      si.dwFlags = 0; 
      si.wShowWindow = SW_SHOW; 
      si.lpReserved2 = NULL; 
      si.cbReserved2 = 0;

      success = CreateProcessAsUser(hToken, "cmd.exe", szCommand, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
      if (!success) OutputDebugLastError(_T("dignotify:"));
    }

  _stprintf(tbuf, _T("dignotify: cmd.exe %s returned (%d)\r\n"), szCommand, success);
  OutputDebugString(tbuf);
  return success;
}

VOID OutputDebugLastError(_TCHAR szMessage[]) 
{ 
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                  NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMsgBuf, 0, NULL );

    lpDisplayBuf = LocalAlloc(LMEM_ZEROINIT, strlen(lpMsgBuf)+strlen(szMessage)+40); 
    _stprintf(lpDisplayBuf, "%s failed with error %d: %s", szMessage, dw, lpMsgBuf); 
    OutputDebugString(lpDisplayBuf);

    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}
