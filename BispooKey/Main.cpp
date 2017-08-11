#pragma comment (linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment (lib, "Comctl32")

#include "RunOnce.h"
#include <CommCtrl.h>
#include "resource.h"

BOOL SetDlgItemInt32(HWND hwnd, UINT uValue);
unsigned int BispooKeygenme(const char* szName, int nLen);
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HFONT g_hFont = 0;
HINSTANCE g_hInstance = 0;
HWND g_hWndPassword = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
	HWND hWnd = 0;
	INITCOMMONCONTROLSEX iccx;
	LOGFONT lf;
	LPCSTR szClassName = TEXT("BPKG");
	WNDCLASSEX wcx;
	MSG msg;
	RunOnce* runOnce = 0;

	SecureZeroMemory(&iccx, sizeof(INITCOMMONCONTROLSEX));
	iccx.dwICC =  ICC_STANDARD_CLASSES;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);

	if (InitCommonControlsEx(&iccx) == 0)
		return 1;

	SecureZeroMemory(&lf, sizeof(LOGFONT));
	lf.lfHeight = -11;
	strcpy_s(lf.lfFaceName, TEXT("Microsoft Sans Serif"));
	lf.lfWeight = FW_NORMAL;

	if ((g_hFont = CreateFontIndirect(&lf)) == 0)
		return 1;

	SecureZeroMemory(&wcx, sizeof(WNDCLASSEX));
	wcx.cbSize = sizeof(WNDCLASSEX);
	wcx.style = CS_HREDRAW | CS_VREDRAW;
	wcx.lpfnWndProc = WindowProc;
	wcx.hInstance = hInstance;
	wcx.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcx.hbrBackground = reinterpret_cast<HBRUSH> (COLOR_BTNFACE + 1);
	wcx.lpszClassName = szClassName;

	if (RegisterClassEx(&wcx) == 0)
		return 1;

	g_hInstance = hInstance;

	hWnd = CreateWindowEx(
		NULL,
		szClassName,
		TEXT("Bispoo #1 v1.0 Keygen"),
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		251,
		124,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hWnd == 0)
		return 1;

	runOnce = new RunOnce(hWnd);
	if (runOnce->PerformCheck("BispooKey.tmp") == 0)
	{
		delete runOnce;
		runOnce = 0;
		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	if (!UpdateWindow(hWnd))
	{
		delete runOnce;
		runOnce = 0;
		return 1;
	}


	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return static_cast<int> (msg.wParam);
}

BOOL SetDlgItemInt32(HWND hwnd, UINT uValue)
{
	TCHAR pszBuffer[35] = {0};
	int nRet = 0;

	if (uValue)
	{
		wsprintf(pszBuffer, "%i", uValue);
		if (pszBuffer[0] == '-') //will always happen in algorithm
			pszBuffer[0] = '6';
	}

	nRet = SendMessage(hwnd, WM_SETTEXT, NULL, reinterpret_cast<LPARAM> (pszBuffer));

	if (nRet == 0 || nRet == LB_ERRSPACE ||
		nRet == CB_ERRSPACE || nRet == CB_ERR)
		return FALSE;

	return TRUE;
}

unsigned int BispooKeygenme(const char* szName, int nLen)
{
	unsigned int uiDigit32 = 0;
	unsigned int uiName32 = 0;
	unsigned int uiTotal = 0;
	char* szBuffer = new char[nLen + 1];

	SecureZeroMemory(reinterpret_cast<PVOID> (szBuffer), nLen + 1);

	for (int i = 1; i <= nLen; i++)
	{
		//calculate 32-bit digit
		uiDigit32 = szName[i - 1];
		uiDigit32 *= i;
		uiDigit32 += i;
		uiDigit32 <<= 0x0BA; //shift bits left
		uiDigit32 >>= 0x0BE; //shift bits right
		uiDigit32 ^= i; //bitwise xor
		uiDigit32 += 0x32;

		//add to future operand b
		uiTotal += uiDigit32;

		szBuffer[i - 1] = static_cast<char> (uiDigit32); //for future reversing
	}

	//change endian
	for (int i = nLen; i >= 0; i--)
	{
		uiName32 *= 0x100;		//push byte left
		uiName32 |= szBuffer[i];  //and include new byte
	}

	return ~(uiName32 * uiTotal);
}

BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM)
{
	SendMessage(hwnd, WM_SETFONT, reinterpret_cast<WPARAM> (g_hFont), MAKELPARAM(TRUE, NULL));
	return TRUE;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HWND hChild = 0;
	int nLen = 0;
	char* szUsername = 0;

	switch (uMsg)
	{
	case WM_CREATE:
		CreateWindowEx(NULL, WC_STATIC, TEXT("Password"),
			WS_CHILD | WS_VISIBLE,
			12, 48, 53, 13,
			hwnd, reinterpret_cast<HMENU> (1), g_hInstance, NULL);

		g_hWndPassword = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL,
			ES_AUTOHSCROLL | ES_CENTER | WS_CHILD | ES_READONLY | WS_VISIBLE,
			12, 64, 221, 20,
			hwnd, reinterpret_cast<HMENU> (2), g_hInstance, NULL);

		CreateWindowEx(NULL, WC_STATIC, TEXT("Username"),
			WS_CHILD | WS_VISIBLE,
			12, 9, 55, 13,
			hwnd, reinterpret_cast<HMENU> (3), g_hInstance, NULL);

		hChild = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, TEXT("RedMage1993"),
			ES_AUTOHSCROLL | ES_CENTER | WS_CHILD | WS_VISIBLE,
			12, 25, 221, 20,
			hwnd, reinterpret_cast<HMENU> (4), g_hInstance, NULL);
		SendMessage(hChild, EM_SETLIMITTEXT, static_cast<WPARAM> (34), NULL);

		if (!EnumChildWindows(hwnd, EnumChildProc, 0))
			DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		switch (HIWORD(wParam))
		{
		case EN_CHANGE:
			switch (LOWORD(wParam))
			{
			case 4: //username textbox
				if ((SetDlgItemInt32(g_hWndPassword, 0)) == 0)
					DestroyWindow(hwnd);

				if ((nLen = SendMessage(reinterpret_cast<HWND> (lParam), WM_GETTEXTLENGTH, 0, 0)) < 4)
					break;

				szUsername = new char[nLen + 1];
				SecureZeroMemory(reinterpret_cast<PVOID> (szUsername), nLen + 1);

				if (SendMessage(reinterpret_cast<HWND> (lParam), WM_GETTEXT, static_cast<WPARAM> (nLen + 1),
					reinterpret_cast<LPARAM> (szUsername)) == 0)
					DestroyWindow(hwnd);

				if ((SetDlgItemInt32(g_hWndPassword, BispooKeygenme(szUsername, nLen))) == 0)
					DestroyWindow(hwnd);

				delete [] szUsername;
				szUsername = 0;
				break;
			}
			break;

		}
		break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return 0;
}