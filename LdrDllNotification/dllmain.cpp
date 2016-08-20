// dllmain.cpp : Defines the entry point for the DLL application.
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include "ntapi.h"


_LdrRegisterDllNotification LdrRegisterDllNotification = NULL;
_LdrUnregisterDllNotification LdrUnregisterDllNotifcation = NULL;
PVOID Cookie = NULL;
FILE *fp = NULL;

BOOL GetNtFunctions()
{
	HMODULE hNtDll;
	if (!(hNtDll = GetModuleHandle(TEXT("ntdll.dll"))))
	{
		return FALSE;
	}
	LdrRegisterDllNotification = (_LdrRegisterDllNotification)GetProcAddress(hNtDll, "LdrRegisterDllNotification");
	LdrUnregisterDllNotifcation = (_LdrUnregisterDllNotification)GetProcAddress(hNtDll, "LdrUnregisterDllNotification");
	if (!LdrRegisterDllNotification || !LdrUnregisterDllNotifcation)
		return FALSE;

	return TRUE;
}

void CALLBACK MyDllNotification(ULONG Reason, PLDR_DLL_NOTIFICATION_DATA NotificationData,
	PVOID Context)
{
	//Check for the reason
	switch (Reason)
	{
	//LDR_DLL_NOTIFICATION_REASON_LOADED
	case LDR_DLL_NOTIFICATION_REASON_LOADED:
		break;
	//LDR_DLL_NOTIFICATION_REASON_UNLOADED
	case LDR_DLL_NOTIFICATION_REASON_UNLOADED:
	{

		wchar_t message[256] = { 0 };
		swprintf(message, L"DLL was unloaded event for %wZ\n", (*NotificationData->Unloaded.FullDllName));
		//MessageBoxW(0, message, L"Event", MB_OK);
		fwprintf_s(fp, message);
	}
	default:
		return;
	}
}


BOOL Startup()
{
	NTSTATUS ret;
	_wfopen_s(&fp, L"C:\\Log\\logger.txt", L"wb");
	if (!GetNtFunctions())
		return FALSE;
	if (fp)
	{
		if (LdrRegisterDllNotification) 
		{
			ret = LdrRegisterDllNotification(0, &MyDllNotification, NULL, &Cookie);
			if (ret != STATUS_SUCCESS)
				return FALSE;
			else
				return TRUE;
		}
	}
	return FALSE;
}

void Cleanup()
{
	if (LdrUnregisterDllNotifcation)
		if (Cookie != NULL)
			LdrUnregisterDllNotifcation(Cookie);
	return;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		if (!Startup())
			break;

	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
	{
		Cleanup();
		break;

	}
	}
	return TRUE;
}

