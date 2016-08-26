#include <Windows.h>
#include <tchar.h>
#include <cstdlib>

#include "../Common/ntapi.h"

#pragma comment(lib, "user32")

_LdrRegisterDllNotification LdrRegisterDllNotification = NULL;
_LdrUnregisterDllNotification LdrUnregisterDllNotifcation = NULL;
PVOID Cookie = NULL;
FILE *fp = NULL;


BOOL GetNtFunctions()
{
    HMODULE hNtDll;
    if(!(hNtDll = GetModuleHandle(TEXT("ntdll.dll"))))
    {
        return FALSE;
    }
    LdrRegisterDllNotification = (_LdrRegisterDllNotification)GetProcAddress(hNtDll, "LdrRegisterDllNotification");
    LdrUnregisterDllNotifcation = (_LdrUnregisterDllNotification)GetProcAddress(hNtDll, "LdrUnregisterDllNotification");
    if(!LdrRegisterDllNotification || !LdrUnregisterDllNotifcation)
        return FALSE;

    return TRUE;
}


void CALLBACK MyDllNotification(ULONG Reason, PLDR_DLL_NOTIFICATION_DATA NotificationData,
    PVOID Context)
{
    //Check for the reason
    switch(Reason)
    {
        //LDR_DLL_NOTIFICATION_REASON_LOADED
    case LDR_DLL_NOTIFICATION_REASON_LOADED:
        break;
        //LDR_DLL_NOTIFICATION_REASON_UNLOADED
    case LDR_DLL_NOTIFICATION_REASON_UNLOADED:
    {

        wchar_t message[256] = {0};
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
    if(!GetNtFunctions())
        return FALSE;
    if(fp)
    {
        if(LdrRegisterDllNotification)
        {
            ret = LdrRegisterDllNotification(0, &MyDllNotification, NULL, &Cookie);
            if(ret != STATUS_SUCCESS)
                return FALSE;
            else
                return TRUE;
        }
    }
    return FALSE;
}

void Cleanup()
{
    if(LdrUnregisterDllNotifcation)
        if(Cookie != NULL)
            LdrUnregisterDllNotifcation(Cookie);
    return;
}

int main(int argc, char** argv)
{
    wchar_t input;
    if(!Startup())
    {
        wprintf_s(L"Could not get the address LdrRegisterDllNOtifications\n");
        return 0;
    }
    wprintf_s(L"Press 'q' to quit\n");
    while(1)
    {

    }

}


