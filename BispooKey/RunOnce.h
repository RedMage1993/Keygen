//*********************************************************
// Author: Fritz Ammon
// Date: 17 July 2011
// Program: RunOnce.h
// Description: Allows a programmer to limit a user to one
// instance of the application per session.
//*********************************************************

#ifndef RUNONCE_H
#define RUNONCE_H

#pragma comment (lib, "Shlwapi.lib")

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <Shlobj.h>
#include <Shlwapi.h>
#include <Windows.h>

using namespace std;

class RunOnce
{
public:
	RunOnce(HWND hWnd);
	~RunOnce();

	BOOL GetMutexPathFile(LPSTR lpFile = NULL);
	BOOL GetMutexName();
	BOOL MutexNameExists();
	BOOL GenMutexName();
	BOOL SetMutexName();
	BOOL SetPageWindow();
	BOOL BringToFocus();

	BOOL PerformCheck(LPSTR lpFile = NULL);

private:
	TCHAR m_pszMutexPathFile[MAX_PATH];
	TCHAR m_pszMutexName[MAX_PATH];
	HANDLE m_hMutexFile;
	HANDLE m_hMutex;
	HANDLE m_hMapFile;
	HWND m_hWnd;
};

#endif // RUNONCE_H