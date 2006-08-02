/*
 *  ReactOS kernel
 *  Copyright (C) 2003 ReactOS Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
/* $Id$
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS winlogon
 * FILE:            subsys/system/winlogon/winlogon.h
 * PURPOSE:         Winlogon
 * PROGRAMMER:
 */

#ifndef __WINLOGON_MAIN_H__
#define __WINLOGON_MAIN_H__

#define WIN32_NO_STATUS
#include <windows.h>
#include <userenv.h>
#include <winwlx.h>
#include <rtlfuncs.h>
#include <exfuncs.h>
#include <setypes.h>
#include <ntsecapi.h>

#include <reactos/winlogon.h>

#include "setup.h"
#include "resource.h"

typedef BOOL (WINAPI * PFWLXNEGOTIATE)  (DWORD, DWORD *);
typedef BOOL (WINAPI * PFWLXINITIALIZE) (LPWSTR, HANDLE, PVOID, PVOID, PVOID *);
typedef VOID (WINAPI * PFWLXDISPLAYSASNOTICE) (PVOID);
typedef int  (WINAPI * PFWLXLOGGEDOUTSAS) (PVOID, DWORD, PLUID, PSID, PDWORD,
                                           PHANDLE, PWLX_MPR_NOTIFY_INFO,
                                           PVOID *);
typedef BOOL (WINAPI * PFWLXACTIVATEUSERSHELL) (PVOID, PWSTR, PWSTR, PVOID);
typedef int  (WINAPI * PFWLXLOGGEDONSAS) (PVOID, DWORD, PVOID);
typedef VOID (WINAPI * PFWLXDISPLAYLOCKEDNOTICE) (PVOID);
typedef int  (WINAPI * PFWLXWKSTALOCKEDSAS) (PVOID, DWORD);
typedef BOOL (WINAPI * PFWLXISLOCKOK) (PVOID);
typedef BOOL (WINAPI * PFWLXISLOGOFFOK) (PVOID);
typedef VOID (WINAPI * PFWLXLOGOFF) (PVOID);
typedef VOID (WINAPI * PFWLXSHUTDOWN) (PVOID, DWORD);

/* version 1.1 */

typedef BOOL (WINAPI * PFWLXSCREENSAVERNOTIFY) (PVOID, BOOL *);
typedef BOOL (WINAPI * PFWLXSTARTAPPLICATION) (PVOID, PWSTR, PVOID, PWSTR);

/* version 1.3 */

typedef BOOL (WINAPI * PFWLXNETWORKPROVIDERLOAD) (PVOID, PWLX_MPR_NOTIFY_INFO);
typedef BOOL (WINAPI * PFWLXDISPLAYSTATUSMESSAGE) (PVOID, HDESK, DWORD, PWSTR, PWSTR);
typedef BOOL (WINAPI * PFWLXGETSTATUSMESSAGE) (PVOID, DWORD *, PWSTR, DWORD);
typedef BOOL (WINAPI * PFWLXREMOVESTATUSMESSAGE) (PVOID);

typedef struct _MSGINAFUNCTIONS
{
	/* Functions always available for a valid GINA */
	PFWLXNEGOTIATE            WlxNegotiate;
	PFWLXINITIALIZE           WlxInitialize;

	/* Functions available if WlxVersion >= WLX_VERSION_1_0 (MS Windows 3.5.0) */
	PFWLXDISPLAYSASNOTICE     WlxDisplaySASNotice;
	PFWLXLOGGEDOUTSAS         WlxLoggedOutSAS;
	PFWLXACTIVATEUSERSHELL    WlxActivateUserShell;
	PFWLXLOGGEDONSAS          WlxLoggedOnSAS;
	PFWLXDISPLAYLOCKEDNOTICE  WlxDisplayLockedNotice;
	PFWLXWKSTALOCKEDSAS       WlxWkstaLockedSAS;
	PFWLXISLOCKOK             WlxIsLockOk;
	PFWLXISLOGOFFOK           WlxIsLogoffOk;
	PFWLXLOGOFF               WlxLogoff;
	PFWLXSHUTDOWN             WlxShutdown;

	/* Functions available if WlxVersion >= WLX_VERSION_1_1 (MS Windows 3.5.1) */
	PFWLXSCREENSAVERNOTIFY    WlxScreenSaverNotify;
	PFWLXSTARTAPPLICATION     WlxStartApplication;

	/* Functions available if WlxVersion >= WLX_VERSION_1_2 (MS Windows NT 4.0) */

	/* Functions available if WlxVersion >= WLX_VERSION_1_3 (MS Windows 2000) */
	PFWLXNETWORKPROVIDERLOAD  WlxNetworkProviderLoad;
	PFWLXDISPLAYSTATUSMESSAGE WlxDisplayStatusMessage;
	PFWLXGETSTATUSMESSAGE     WlxGetStatusMessage;
	PFWLXREMOVESTATUSMESSAGE  WlxRemoveStatusMessage;

	/* Functions available if WlxVersion >= WLX_VERSION_1_4 (MS Windows XP) */
} MSGINAFUNCTIONS, *PMSGINAFUNCTIONS;

typedef struct _MSGINAINSTANCE
{
	HMODULE hDllInstance;
	MSGINAFUNCTIONS Functions;
	PVOID Context;
	DWORD Version;
} MSGINAINSTANCE, *PMSGINAINSTANCE;

/* FIXME: put in an enum */
#define WKSTA_IS_LOGGED_OFF 0
#define WKSTA_IS_LOGGED_ON  1
#define WKSTA_IS_LOCKED     2

typedef struct _WLSESSION
{
  MSGINAINSTANCE MsGina;
  DWORD SASAction;
  DWORD LogonStatus;
  BOOL SuppressStatus;
  BOOL TaskManHotkey;
  HWND SASWindow;
  HWINSTA InteractiveWindowStation;
  LPWSTR InteractiveWindowStationName;
  HDESK ApplicationDesktop;
  HDESK WinlogonDesktop;
  HDESK ScreenSaverDesktop;
  LUID LogonId;
  HANDLE UserToken;

  /* Logon informations */
  DWORD Options;
  WLX_MPR_NOTIFY_INFO MprNotifyInfo;
  WLX_PROFILE_V2_0 Profile;
} WLSESSION, *PWLSESSION;

extern HINSTANCE hAppInstance;
extern PWLSESSION WLSession;

#define WLX_SHUTTINGDOWN(Status) \
  (((Status) == WLX_SAS_ACTION_SHUTDOWN) || \
   ((Status) == WLX_SAS_ACTION_SHUTDOWN_POWER_OFF) || \
   ((Status) == WLX_SAS_ACTION_SHUTDOWN_REBOOT) \
  )

#define WLX_SUSPENDING(Status) \
  (((Status) == WLX_SAS_ACTION_SHUTDOWN_SLEEP) || \
   ((Status) == WLX_SAS_ACTION_SHUTDOWN_SLEEP2) || \
   ((Status) == WLX_SAS_ACTION_SHUTDOWN_HIBERNATE) \
  )

#define RemoveStatusMessage(Session) \
  Session->MsGina.Functions.WlxRemoveStatusMessage(Session->MsGina.Context);
#define DisplaySASNotice(Session) \
  Session->MsGina.Functions.WlxDisplaySASNotice(Session->MsGina.Context);

/* user32 */
BOOL WINAPI
UpdatePerUserSystemParameters(DWORD dwUnknown,
                              DWORD dwReserved);

/* sas.c */
VOID
DispatchSAS(
	IN OUT PWLSESSION Session,
	IN DWORD dwSasType);
BOOL
InitializeSAS(
	IN OUT PWLSESSION Session);

/* wlx.c */
BOOL
GinaInit(
	IN OUT PWLSESSION Session);
BOOL
CreateWindowStationAndDesktops(
	IN OUT PWLSESSION Session);

VOID WINAPI WlxUseCtrlAltDel(HANDLE hWlx);
VOID WINAPI WlxSetContextPointer(HANDLE hWlx, PVOID pWlxContext);
VOID WINAPI WlxSasNotify(HANDLE hWlx, DWORD dwSasType);
BOOL WINAPI WlxSetTimeout(HANDLE hWlx, DWORD Timeout);
int WINAPI WlxAssignShellProtection(HANDLE hWlx, HANDLE hToken, HANDLE hProcess, HANDLE hThread);
int WINAPI WlxMessageBox(HANDLE hWlx, HWND hwndOwner, LPWSTR lpszText, LPWSTR lpszTitle, UINT fuStyle);
int WINAPI WlxDialogBox(HANDLE hWlx, HANDLE hInst, LPWSTR lpszTemplate, HWND hwndOwner, DLGPROC dlgprc);
int WINAPI WlxDialogBoxParam(HANDLE hWlx, HANDLE hInst, LPWSTR lpszTemplate, HWND hwndOwner, DLGPROC dlgprc, LPARAM dwInitParam);
int WINAPI WlxDialogBoxIndirect(HANDLE hWlx, HANDLE hInst, LPCDLGTEMPLATE hDialogTemplate, HWND hwndOwner, DLGPROC dlgprc);
int WINAPI WlxDialogBoxIndirectParam(HANDLE hWlx, HANDLE hInst, LPCDLGTEMPLATE hDialogTemplate, HWND hwndOwner, DLGPROC dlgprc, LPARAM dwInitParam);
int WINAPI WlxSwitchDesktopToUser(HANDLE hWlx);
int WINAPI WlxSwitchDesktopToWinlogon(HANDLE hWlx);
int WINAPI WlxChangePasswordNotify(HANDLE hWlx, PWLX_MPR_NOTIFY_INFO pMprInfo, DWORD dwChangeInfo);
BOOL WINAPI WlxGetSourceDesktop(HANDLE hWlx, PWLX_DESKTOP* ppDesktop);
BOOL WINAPI WlxSetReturnDesktop(HANDLE hWlx, PWLX_DESKTOP pDesktop);
BOOL WINAPI WlxCreateUserDesktop(HANDLE hWlx, HANDLE hToken, DWORD Flags, PWSTR pszDesktopName, PWLX_DESKTOP* ppDesktop);
int WINAPI WlxChangePasswordNotifyEx(HANDLE hWlx, PWLX_MPR_NOTIFY_INFO pMprInfo, DWORD dwChangeInfo, PWSTR ProviderName, PVOID Reserved);
BOOL WINAPI WlxCloseUserDesktop(HANDLE hWlx, PWLX_DESKTOP pDesktop, HANDLE hToken);
BOOL WINAPI WlxSetOption(HANDLE hWlx, DWORD Option, ULONG_PTR Value, ULONG_PTR* OldValue);
BOOL WINAPI WlxGetOption(HANDLE hWlx, DWORD Option, ULONG_PTR* Value);
VOID WINAPI WlxWin31Migrate(HANDLE hWlx);
BOOL WINAPI WlxQueryClientCredentials(PWLX_CLIENT_CREDENTIALS_INFO_V1_0 pCred);
BOOL WINAPI WlxQueryInetConnectorCredentials(PWLX_CLIENT_CREDENTIALS_INFO_V1_0 pCred);
DWORD WINAPI WlxQueryConsoleSwitchCredentials(PWLX_CONSOLESWITCH_CREDENTIALS_INFO_V1_0 pCred);
BOOL WINAPI WlxQueryTsLogonCredentials(PWLX_CLIENT_CREDENTIALS_INFO_V2_0 pCred);
BOOL WINAPI WlxDisconnect(VOID);
DWORD WINAPI WlxQueryTerminalServicesData(HANDLE hWlx, PWLX_TERMINAL_SERVICES_DATA pTSData, WCHAR* UserName, WCHAR* Domain);

#endif /* __WINLOGON_MAIN_H__ */

/* EOF */
