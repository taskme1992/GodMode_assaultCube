
#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <stdlib.h>

//Global Vars
HANDLE hProc = NULL;
DWORD pID;

boolean attachProc(char* procName)
{
    PROCESSENTRY32 procEntry32;

    procEntry32.dwSize = sizeof(PROCESSENTRY32);

    auto hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcSnap == INVALID_HANDLE_VALUE)
    {
        std::cout << "Failed to take snapshot of processes" << std::endl;
        return false;
    }

    while (Process32Next(hProcSnap, &procEntry32))
    {
        std::cout << procEntry32.szExeFile << std::endl;

        if (!strcmp(procName, procEntry32.szExeFile))
        {
            std::cout << "Found process " << procEntry32.szExeFile << " with process ID " << procEntry32.th32ProcessID << std::endl;
            hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
            pID = procEntry32.th32ProcessID;

            if (hProc == NULL)
            {
                std::cout << "Failed getting handle to process." << std::endl;
            }

            CloseHandle(hProcSnap);
            return true;

        }
    }

    std::cout << "Couldn't find " << procName << " in the process snapshot" << std::endl;
    CloseHandle(hProcSnap);
    return false;

}

template <class dataType>
void wpm(dataType valToWrite, DWORD addressToWrite)
{
    WriteProcessMemory(hProc, (PVOID)addressToWrite, &valToWrite, sizeof(dataType), 0);
}

template <class dataType>
dataType rmp(DWORD addressToRead)
{
    dataType rmpBuffer;

    ReadProcessMemory(hProc, (PVOID)addressToRead, &rmpBuffer, sizeof(dataType), 0);

    return rmpBuffer;
}


int main()
{
    DWORD memoryAddress = 0x00882CA4;

    attachProc((char*)"ac_client.exe");

    DWORD base = 0x0017E0A8;
    DWORD off = 0xEC;

    //int new = rpm<DWORD>(baseAddress + offs::dwEntityList + (index * 0x10))
    //int test = rmp<int>(memoryAddress);
    std::cout << (0x004000000 + base) + off;

    while (true)
    {
        int iSecret = rand() % 10 + 1;
        wpm<int>(400, memoryAddress);

    }

}

