#pragma once
 
#include <Windows.h>
#include <TlHelp32.h>
 
 
class entry
{
public:
 
	HWND __HWNDwut;
    PROCESSENTRY32 __yes;
	HANDLE __no;
    DWORD __Client;
 
    DWORD FindProcessName(const char *__ProcessName, PROCESSENTRY32 *pEntry)
    {
        PROCESSENTRY32 __ProcessEntry;
        __ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) return 0;        if (!Process32First(hSnapshot, &__ProcessEntry))
        {
            CloseHandle(hSnapshot);
            return 0;
        }
        do {
            if (!_strcmpi(__ProcessEntry.szExeFile, __ProcessName))
            {
                memcpy((void *)pEntry, (void *)&__ProcessEntry, sizeof(PROCESSENTRY32));
                CloseHandle(hSnapshot);
                return __ProcessEntry.th32ProcessID;
            }
        } while (Process32Next(hSnapshot, &__ProcessEntry));
        CloseHandle(hSnapshot);
        return 0;
    }
 
 
    DWORD getThreadByProcess(DWORD __DwordProcess)
    {
        THREADENTRY32 __ThreadEntry;
        __ThreadEntry.dwSize = sizeof(THREADENTRY32);
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) return 0;
 
        if (!Thread32First(hSnapshot, &__ThreadEntry)) { CloseHandle(hSnapshot); return 0; }
 
        do {
            if (__ThreadEntry.th32OwnerProcessID == __DwordProcess)
            {
                CloseHandle(hSnapshot);
                return __ThreadEntry.th32ThreadID;
            }
        } while (Thread32Next(hSnapshot, &__ThreadEntry));
        CloseHandle(hSnapshot);
        return 0;
    }
 
    DWORD GetModuleNamePointer(LPSTR LPSTRModuleName, DWORD __DwordProcessId)
    {
        MODULEENTRY32 lpModuleEntry = { 0 };
        HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, __DwordProcessId);
        if (!hSnapShot)
            return NULL;
        lpModuleEntry.dwSize = sizeof(lpModuleEntry);
        BOOL __RunModule = Module32First(hSnapShot, &lpModuleEntry);
        while (__RunModule)
        {
            if (!strcmp(lpModuleEntry.szModule, LPSTRModuleName))
            {
                CloseHandle(hSnapShot);
                return (DWORD)lpModuleEntry.modBaseAddr;
            }
            __RunModule = Module32Next(hSnapShot, &lpModuleEntry);
        }
        CloseHandle(hSnapShot);
        return NULL;
    }
 
 
    void runSetDebugPrivs()
    {
        HANDLE __no = GetCurrentProcess(), __HandleToken;
        TOKEN_PRIVILEGES priv;
        LUID __LUID;
        OpenProcessToken(__no, TOKEN_ADJUST_PRIVILEGES, &__HandleToken);
        LookupPrivilegeValue(0, "seDebugPrivilege", &__LUID);
        priv.PrivilegeCount = 1;
        priv.Privileges[0].Luid = __LUID;
        priv.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        AdjustTokenPrivileges(__HandleToken, false, &priv, 0, 0, 0);
        CloseHandle(__HandleToken);
        CloseHandle(__no);
    }
 
 
 
    void runProcess()
    {
        runSetDebugPrivs();
        while (!FindProcessName("csgo.exe", &__yes));
        while (!(getThreadByProcess(__yes.th32ProcessID)));
        __no = OpenProcess(PROCESS_ALL_ACCESS, false, __yes.th32ProcessID);
        while (__Client == 0x0) __Client = GetModuleNamePointer("client.dll", __yes.th32ProcessID);
        __HWNDwut = FindWindow(NULL, "Counter-Strike: Global Offsensive");
    }
};
 
extern entry wow;





