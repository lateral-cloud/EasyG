#pragma once

#include "EasyGHead.h"
#include "EasyTypes.h"

_EASYG_BEGIN

class EasyHandle
{
public:
	static HWND hConsole;
	static HMODULE hModule;
	static HINSTANCE& hInstance;
	static PTSZ screenSize;
	static HDC screenHDC;

	static HMODULE WINAPI GetModuleHandle(LPCTSTR lpModuleName = nullptr) noexcept { return ::GetModuleHandle(lpModuleName); }
	static int WINAPI GetModuleHandleEx(DWORD dwFlags, LPCTSTR lpModuleName, HMODULE* phModule) noexcept { return ::GetModuleHandleEx(dwFlags,lpModuleName,phModule); }
	static DWORD WINAPI GetModuleFileName(LPTSTR lpFilename, DWORD nSize) noexcept { return ::GetModuleFileName(hModule, lpFilename, nSize); }
	static DWORD WINAPI GetModuleFileName(HMODULE hMod, LPTSTR lpFilename, DWORD nSize) noexcept { return ::GetModuleFileName(hMod, lpFilename, nSize); }

	static HWND WINAPI GetConsoleWindow() noexcept { return ::GetConsoleWindow(); }
	static void WINAPI ShowConsole(DWORD style = SW_SHOW) noexcept { ::ShowWindow(hConsole, style); }
	static void WINAPI HideConsole() noexcept { EasyHandle::ShowConsole(SW_HIDE); }

	static DWORD SetWindowStyle(HWND hWnd, DWORD lNewStyle) noexcept { return (DWORD)::SetWindowLongPtr(hWnd, GWL_STYLE, lNewStyle); }
	static DWORD SetWindowExStyle(HWND hWnd, DWORD lNewExStyle) noexcept { return (DWORD)::SetWindowLongPtr(hWnd, GWL_EXSTYLE, lNewExStyle); }
	static DWORD GetWindowStyle(HWND hWnd) noexcept { return (DWORD)::GetWindowLongPtr(hWnd, GWL_STYLE); }
	static DWORD GetWindowExStyle(HWND hWnd) noexcept { return (DWORD)::GetWindowLongPtr(hWnd, GWL_EXSTYLE); }
	static void SetWindowPos(HWND hWnd, int x, int y) noexcept { ::SetWindowPos(hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE); }
	static void SetWindowPos(HWND hWnd, const POINT& pt) noexcept { EasyHandle::SetWindowPos(hWnd, pt.x, pt.y); }
	static void SetWindowPosRel(HWND hWnd, int dx, int dy) noexcept { POINT pos = EasyHandle::GetWindowPos(hWnd); EasyHandle::SetWindowPos(hWnd, pos.x + dx, pos.y + dy); }
	static void SetWindowPosRel(HWND hWnd, const SIZE& size) noexcept { EasyHandle::SetWindowPosRel(hWnd, size.cx, size.cy); }
	static POINT GetWindowPos(HWND hWnd) noexcept { RECT rct; ::GetWindowRect(hWnd, &rct); return { rct.left, rct.top }; }
	static void SetWindowSize(HWND hWnd, int Width, int Height) noexcept { ::SetWindowPos(hWnd, HWND_TOP, 0, 0, Width, Height, SWP_NOMOVE); }
	static void SetWindowSize(HWND hWnd, const SIZE& size) noexcept { EasyHandle::SetWindowSize(hWnd, size.cx, size.cy); }
	static SIZE GetWindowSize(HWND hWnd) noexcept { RECT rct; ::GetWindowRect(hWnd, &rct); return { rct.right - rct.left, rct.bottom - rct.top }; }
	static void SetWindowPtsz(HWND hWnd, const PTSZ& ptsz) noexcept { EasyHandle::SetWindowPos(hWnd, ptsz.point); EasyHandle::SetWindowSize(hWnd, ptsz.size); }
	static PTSZ GetWindowPtsz(HWND hWnd) noexcept { return EasyHandle::GetWindowRect(hWnd); }
	static void SetWindowRect(HWND hWnd, const RECT& rect) noexcept { EasyHandle::SetWindowPtsz(hWnd, rect); }
	static RECT GetWindowRect(HWND hWnd) noexcept { RECT rct; ::GetWindowRect(hWnd, &rct); return rct; }

	static HACCEL LoadAccelerators(LPCTSTR lpTableName) noexcept { return ::LoadAccelerators(hModule, lpTableName); }
	static HACCEL LoadAccelerators(HINSTANCE hIns, LPCTSTR lpTableName) noexcept { return ::LoadAccelerators(hIns, lpTableName); }
	static HCURSOR LoadCursor(LPCTSTR lpCursorName) noexcept { return ::LoadCursor(hModule, lpCursorName); }
	static HCURSOR LoadCursor(HINSTANCE hIns, LPCTSTR lpCursorName) noexcept { return ::LoadCursor(hIns, lpCursorName); }
	static HCURSOR LoadCursorFromFile(LPCTSTR lpFileName) noexcept { return ::LoadCursorFromFile(lpFileName); }
	static HICON LoadIcon(LPCTSTR lpIconName) noexcept { return ::LoadIcon(hModule, lpIconName); }
	static HICON LoadIcon(HINSTANCE hIns, LPCTSTR lpIconName) noexcept { return ::LoadIcon(hIns, lpIconName); }
	static HMENU LoadMenu(LPCTSTR lpMenuName) noexcept { return ::LoadMenu(hModule, lpMenuName); }
	static HMENU LoadMenu(HINSTANCE hIns, LPCTSTR lpMenuName) noexcept { return ::LoadMenu(hIns, lpMenuName); }
	static int LoadString(UINT uID, LPTSTR lpBuffer, int cchBufferMax) noexcept { return ::LoadString(hModule, uID, lpBuffer, cchBufferMax); }
	static int LoadString(HINSTANCE hIns, UINT uID, LPTSTR lpBuffer, int cchBufferMax) noexcept { return ::LoadString(hIns, uID, lpBuffer, cchBufferMax); }
	static HANDLE LoadImage(LPCTSTR name, UINT type, int cx = 0, int cy = 0, UINT fuLoad = LR_DEFAULTSIZE) noexcept { return ::LoadImage(hModule, name, type, cx, cy, fuLoad); }
	static HANDLE LoadImage(HINSTANCE hIns, LPCTSTR name, UINT type, int cx = 0, int cy = 0, UINT fuLoad = LR_DEFAULTSIZE) noexcept { return ::LoadImage(hIns, name, type, cx, cy, fuLoad); }
	static HBITMAP LoadImage_Bitmap(LPCTSTR name, int cx = 0, int cy = 0, UINT fuLoad = LR_DEFAULTSIZE | LR_CREATEDIBSECTION) noexcept { return (HBITMAP)::LoadImage(hModule, name, IMAGE_BITMAP, cx, cy, fuLoad); }
	static HBITMAP LoadImage_Bitmap(HINSTANCE hIns, LPCTSTR name, int cx = 0, int cy = 0, UINT fuLoad = LR_DEFAULTSIZE | LR_CREATEDIBSECTION) noexcept { return (HBITMAP)::LoadImage(hIns, name, IMAGE_BITMAP, cx, cy, fuLoad); }
	static HCURSOR LoadImage_Cursor(LPCTSTR name, int cx = 0, int cy = 0, UINT fuLoad = LR_DEFAULTSIZE | LR_SHARED) noexcept { return (HCURSOR)::LoadImage(hModule, name, IMAGE_CURSOR, cx, cy, fuLoad); }
	static HCURSOR LoadImage_Cursor(HINSTANCE hIns, LPCTSTR name, int cx = 0, int cy = 0, UINT fuLoad = LR_DEFAULTSIZE | LR_SHARED) noexcept { return (HCURSOR)::LoadImage(hIns, name, IMAGE_CURSOR, cx, cy, fuLoad); }
	static HICON LoadImage_Icon(LPCTSTR name, int cx = 0, int cy = 0, UINT fuLoad = LR_DEFAULTSIZE | LR_SHARED) noexcept { return (HICON)::LoadImage(hModule, name, IMAGE_ICON, cx, cy, fuLoad); }
	static HICON LoadImage_Icon(HINSTANCE hIns, LPCTSTR name, int cx = 0, int cy = 0, UINT fuLoad = LR_DEFAULTSIZE | LR_SHARED) noexcept { return (HICON)::LoadImage(hIns, name, IMAGE_ICON, cx, cy, fuLoad); }
};

HWND EasyHandle::hConsole = EasyHandle::GetConsoleWindow();
HMODULE EasyHandle::hModule = EasyHandle::GetModuleHandle();
HINSTANCE& EasyHandle::hInstance = EasyHandle::hModule;
PTSZ EasyHandle::screenSize{ ::GetSystemMetrics(SM_XVIRTUALSCREEN),::GetSystemMetrics(SM_YVIRTUALSCREEN),
::GetSystemMetrics(SM_CXVIRTUALSCREEN),::GetSystemMetrics(SM_CYVIRTUALSCREEN) };
HDC EasyHandle::screenHDC = ::GetDC(nullptr);

_EASYG_END