#pragma once

#include "EasyWindow_def.h"
#include "EasyHandle.h"

_EASYG_BEGIN

void EasyWindowStruct::Init(HWND hparent, const EasyImage2* pcanvas, WNDPROC windowprocess) noexcept
{
	hParent = hparent;
	WndProcFunc = windowprocess;
	Size = pcanvas->ImageSize;
	pCanvas = pcanvas;
	isAlive = true;
}

void EasyWindowStruct::Init(HWND hparent, int cx, int cy, WNDPROC windowprocess) noexcept
{
	hParent = hparent;
	WndProcFunc = windowprocess;
	Size = { cx,cy };
	isAlive = true;
}

bool RegisterWndClass(LPCTSTR lpszClassName, UINT Style = CS_VREDRAW | CS_HREDRAW, HBRUSH bkBrush = (HBRUSH)GetStockObject(BLACK_BRUSH),
	HCURSOR hcursor = LoadCursor(nullptr, IDC_ARROW), HICON hIcon = nullptr, HICON hIconSm = nullptr) noexcept
{
	static WNDCLASSEX WndClassEx{ sizeof(WNDCLASSEX),0,EasyWindowController::WndProc,0,0,EasyHandle::hInstance,
								nullptr,nullptr,nullptr,nullptr,nullptr,nullptr };
	WndClassEx.style = Style;
	WndClassEx.hIcon = hIcon;
	WndClassEx.hIconSm = hIconSm;
	WndClassEx.hCursor = hcursor;
	WndClassEx.hbrBackground = bkBrush;
	WndClassEx.lpszClassName = lpszClassName;
	if (!RegisterClassEx(&WndClassEx))
	{
		std::wstring str = std::to_wstring(GetLastError());
		MessageBox(nullptr, (L"Error registing window class: " + str).c_str(), L"[Error]", MB_OK | MB_ICONERROR);
		return 0;
	}
	return 1;
}

EasyWindowStuVecT EasyWindowController::vecWindowStruct;
EasyWindowMutexT EasyWindowController::EasyWindowMutex;
EasyWindowCVT EasyWindowController::EW_ProcCV;
EasyWindowCVT EasyWindowController::EW_TaskCV;
EasyWindowCVT EasyWindowController::EW_AliveCV;
EasyWindowCVT EasyWindowController::EW_MsgCV;
EasyWindowCVT EasyWindowController::EW_CreateCV;
const UINT EasyWindowController::WM_TRAY = RegisterWindowMessage(L"Tray");
const UINT EasyWindowController::WM_SYSCTRL_CREATE = RegisterWindowMessage(L"SysCtrlCreate");
const UINT EasyWindowController::WM_SYSCTRL_DELETE = RegisterWindowMessage(L"SysCtrlDelete");
const UINT EasyWindowController::WM_USER_REDRAW = RegisterWindowMessage(L"UserRedraw");
const UINT EasyWindowController::WM_TASKBARCREATED = RegisterWindowMessage(L"TaskbarCreated");
UINT EasyWindowController::WndCount = 0;
UINT EasyWindowController::AliveWndCount = 0;
const bool RegisteredDefaultClass = RegisterWndClass(L"EasyWindowClass");

EasyWindowStruct& EasyWindowController::GetWindowStruct(EasyWindowIndex index) noexcept
{
	return vecWindowStruct[index];
}

bool EasyWindowController::isValidWindowIndex(EasyWindowIndex index) noexcept
{
	return 0 <= index && index < vecWindowStruct.size();
}

bool EasyWindowController::isAliveWindowIndex(EasyWindowIndex index) noexcept
{
	return Self::isValidWindowIndex(index) && vecWindowStruct[index].isAlive;
}

EasyWindowIndex EasyWindowController::GetWindowIndex(HWND hWnd) noexcept
{
	if (!hWnd)
		return EWF_NOWINDOW_INDEX;
	const auto size = (EasyWindowIndex)vecWindowStruct.size();
	for (EasyWindowIndex i = 0; i < size; ++i)
	{
		if (vecWindowStruct[i].hWnd == hWnd)
			return i;
	}
	return EWF_NOWINDOW_INDEX;
}

EasyWindowIndex EasyWindowController::RegisterWindowStu(const EasyWindowStruct& wndStu) noexcept
{
	vecWindowStruct.push_back(wndStu);
	return EasyWindowIndex(vecWindowStruct.size() - 1);
}

bool EasyWindowController::isAnyWindow() noexcept
{
	return AliveWndCount;
}

void EasyWindowController::WaitForProcessingIndex(EasyWindowIndex index) noexcept
{
	if (Self::isValidWindowIndex(index))
	{
		EasyWindowLock lock(EasyWindowMutex);
		while (vecWindowStruct[index].isBusyProcessing)
			EW_ProcCV.wait(lock);
	}
}

void EasyWindowController::BeginProcessingIndex(EasyWindowIndex index) noexcept
{
	vecWindowStruct[index].isBusyProcessing = true;
}

void EasyWindowController::EndProcessingIndex(EasyWindowIndex index) noexcept
{
	vecWindowStruct[index].isBusyProcessing = false;
	EW_ProcCV.notify_all();
}

void EasyWindowController::WaitForTaskIndex(EasyWindowIndex index) noexcept
{
	if (index == EWF_NOWINDOW_INDEX)
	{
		EasyWindowLock lock(EasyWindowMutex);
		const auto size = (EasyWindowIndex)(vecWindowStruct.size());
		bool hasTask = true;
		for (EasyWindowIndex i = 0; i < size;)
		{
			if (vecWindowStruct[index].isInTask)
				hasTask = true;
			else
				hasTask = false;
			while (vecWindowStruct[index].isInTask)
				EW_TaskCV.wait(lock);
			if (hasTask)
				i = 0;
			else
				++i;
		}
	}
	else if (Self::isAliveWindowIndex(index))
	{
		EasyWindowLock lock(EasyWindowMutex);
		while (vecWindowStruct[index].isInTask)
			EW_TaskCV.wait(lock);
	}
}

void EasyWindowController::BeginTaskIndex(EasyWindowIndex index) noexcept
{
	Self::WaitForProcessingIndex(index);
	vecWindowStruct[index].isInTask = true;
}

void EasyWindowController::EndTaskIndex(EasyWindowIndex index) noexcept
{
	vecWindowStruct[index].isInTask = false;
	EW_TaskCV.notify_all();
}

void EasyWindowController::WaitForEnd(EasyWindowIndex index) noexcept
{
	if (index == EWF_NOWINDOW_INDEX)
	{
		EasyWindowLock lock(EasyWindowMutex);
		while (Self::isAnyWindow())
			EW_AliveCV.wait(lock);
	}
	else
	{
		EasyWindowLock lock(EasyWindowMutex);
		while (Self::isAliveWindowIndex(index))
			EW_AliveCV.wait(lock);
	}
}

void EasyWindowController::EndWindow(EasyWindowIndex index) noexcept
{
	vecWindowStruct[index].isAlive = false;
	--AliveWndCount;
	EW_AliveCV.notify_all();
}

void EasyWindowController::FreeWindowIndex(EasyWindowIndex index) noexcept
{
	if (!Self::isValidWindowIndex(index))
		return;
	vecWindowStruct[index].pCanvas = nullptr;
	std::vector<EasyMessage>().swap(vecWindowStruct[index].vecMessage);
}

void EasyWindowController::CloseWindowIndex(EasyWindowIndex index) noexcept
{
	if (index == EWF_NOWINDOW_INDEX && AliveWndCount)
	{
		for (auto& wnd : vecWindowStruct)
			if (wnd.isAlive)
				::SendMessage(wnd.hWnd, WM_DESTROY, 1, 0);
	}
	else if (Self::isAliveWindowIndex(index))
		::SendMessage(Self::GetWindowStruct(index).hWnd, WM_DESTROY, 1, 0);
}

void EasyWindowController::SetWndProcIndex(EasyWindowIndex index, WNDPROC WindowProcess) noexcept
{
	if (Self::isAliveWindowIndex(index))
		vecWindowStruct[index].WndProcFunc = WindowProcess;
}

void EasyWindowController::BindWindowCanvasIndex(EasyWindowIndex index, const EasyImage2* canvas) noexcept
{
	if (Self::isAliveWindowIndex(index))
		vecWindowStruct[index].pCanvas = canvas;
}

void EasyWindowController::WaitExit() noexcept
{
	Self::WaitForEnd(EWF_NOWINDOW_INDEX);
	::exit(0);
}

void EasyWindowController::AutoExit() noexcept
{
	std::thread(WaitExit).detach();
}

void EasyWindowController::QuickDraw(EasyWindowIndex index, UINT nSkipPixels) noexcept
{
	if (Self::isAliveWindowIndex(index))
		vecWindowStruct[index].nSkipPixels = nSkipPixels;
}

void EasyWindowController::ShowTray(NOTIFYICONDATA* nid) noexcept
{
	::Shell_NotifyIcon(NIM_ADD, nid);
}

void EasyWindowController::CreateTrayIndex(EasyWindowIndex index, LPCTSTR lpszTrayName, HICON hIcon) noexcept
{
	static int id = 0;
	if (Self::isAliveWindowIndex(index))
	{
		auto& wnd = vecWindowStruct[index];
		wnd.isUseTray = true;
		wnd.nid.cbSize = sizeof(NOTIFYICONDATA);
		wnd.nid.hWnd = wnd.hWnd;
		wnd.nid.uID = id++;
		wnd.nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		wnd.nid.uCallbackMessage = WM_TRAY;
		wnd.nid.hIcon = hIcon;
		::lstrcpy(wnd.nid.szTip, lpszTrayName);
		Self::ShowTray(&wnd.nid);
	}
}

void EasyWindowController::DeleteTrayIndex(EasyWindowIndex index) noexcept
{
	if (index == EWF_NOWINDOW_INDEX || !isValidWindowIndex(index))
		return;
	if (vecWindowStruct[index].isUseTray)
	{
		vecWindowStruct[index].isUseTray = false;
		Shell_NotifyIcon(NIM_DELETE, &vecWindowStruct[index].nid);
	}
}

void EasyWindowController::SetTrayMenuIndex(EasyWindowIndex index, HMENU hMenu) noexcept
{
	if (Self::isAliveWindowIndex(index))
	{
		vecWindowStruct[index].isUseTrayMenu = true;
		vecWindowStruct[index].hTrayMenu = hMenu;
	}
}

void EasyWindowController::SetTrayMenuProcFuncIndex(EasyWindowIndex index, TRAYPROC pFunc) noexcept
{
	if (Self::isAliveWindowIndex(index))
		vecWindowStruct[index].TrayMenuProcFunc = pFunc;
}

EasyWindowMegVecT& EasyWindowController::GetMsgVecIndex(EasyWindowIndex index) noexcept
{
	static EasyWindowMegVecT nullvec;
	if (Self::isAliveWindowIndex(index))
		return vecWindowStruct[index].vecMessage;
	nullvec.clear();
	return nullvec;
}

void EasyWindowController::RemoveMsgIndex(EasyWindowIndex index) noexcept
{
	if (Self::isAliveWindowIndex(index))
	{
		auto& vec = vecWindowStruct[index].vecMessage;
		if (vec.empty())
			return;
		vec.erase(vec.begin());
	}
}

void EasyWindowController::ClearMsgIndex(EasyWindowIndex index, BYTE filter) noexcept
{
	if (Self::isAliveWindowIndex(index))
	{
		auto& vec = vecWindowStruct[index].vecMessage;
		int size = (int)vec.size();
		for (int i = 0; i < size; ++i)
			if (filter & GetMessageType(vec[i].message))
			{
				vec.erase(vec.begin() + i--);
				--size;
			}
	}
}

bool EasyWindowController::isNewMessageIndex(EasyWindowIndex index, BYTE filter) noexcept
{
	if (Self::isAliveWindowIndex(index))
		for (auto& msg : vecWindowStruct[index].vecMessage)
			if (filter & GetMessageType(msg.message))
				return true;
	return false;
}

EasyMessage EasyWindowController::GetNextMessageIndex(EasyWindowIndex index, BYTE filter) noexcept
{
	if (Self::isNewMessageIndex(index, filter))
	{
		auto& vec = vecWindowStruct[index].vecMessage;
		const int size = (int)vec.size();
		for (int i = 0; i < size; ++i)
			if (filter & GetMessageType(vec[i].message))
			{
				vec.erase(vec.begin(), vec.begin() + i);
				return vec[0];
			}
	}
	return {};
}

void EasyWindowController::GetMessageIndex(EasyWindowIndex index, EasyMessage* pmsg, BYTE filter) noexcept
{
	if (Self::isAliveWindowIndex(index))
	{
		EasyWindowLock lock(EasyWindowMutex);
		while (!Self::isNewMessageIndex(index, filter))
			EW_MsgCV.wait(lock);
		if (pmsg)
			*pmsg = Self::GetNextMessageIndex(index, filter);
		Self::RemoveMsgIndex(index);
		return;
	}
	*pmsg = {};
	return;
}

EasyMessage EasyWindowController::GetMessageIndex(EasyWindowIndex index, BYTE filter) noexcept
{
	EasyMessage msg;
	Self::GetMessageIndex(index, &msg, filter);
	return msg;
}

bool EasyWindowController::PeekMessageIndex(EasyWindowIndex index, EasyMessage* pmsg, BYTE filter, bool removemsg) noexcept
{
	if (Self::isNewMessageIndex(index, filter))
	{
		EasyWindowLock lock(EasyWindowMutex);
		while (!Self::isNewMessageIndex(index, filter))
			EW_MsgCV.wait(lock);
		if (pmsg)
			*pmsg = Self::GetNextMessageIndex(index, filter);
		if (removemsg)
			Self::RemoveMsgIndex(index);
		return true;
	}
	return false;
}

void EasyWindowController::RegisterMessageIndex(EasyWindowIndex index, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	EasyMessage emsg;
	emsg.message = msg;
	emsg.wParam = wParam;
	emsg.lParam = lParam;
	switch (GetMessageType(msg))
	{
	case EMT_MOUSE:
		if (emsg.mouse.wheel)
		{
			POINT p = { emsg.mouse.x ,emsg.mouse.y };
			::ScreenToClient(vecWindowStruct[index].hWnd, &p);
			emsg.mouse.x = (short)p.x;
			emsg.mouse.y = (short)p.y;
		}
		break;
	case EMT_KEY:
	case EMT_CHAR:
		PostMessage(EasyHandle::hConsole, msg, wParam, lParam);
		break;
	}
	vecWindowStruct[index].vecMessage.push_back(emsg);
	EW_MsgCV.notify_all();
}

void EasyWindowController::OnSize(EasyWindowIndex index) noexcept
{
	auto& wnd = vecWindowStruct[index];
	RECT wndRct;
	::GetClientRect(wnd.hWnd, &wndRct);
	wnd.Size.cx = wndRct.right - wnd.FrameSize.cx;
	wnd.Size.cy = wndRct.bottom - wnd.FrameSize.cy;
}

void EasyWindowController::OnTray(EasyWindowIndex index, LPARAM lParam) noexcept
{
	if (vecWindowStruct[index].isUseTray)
	{
		auto& wnd = vecWindowStruct[index];
		HWND hWnd = wnd.hWnd;
		POINT ptMouse;
		::GetCursorPos(&ptMouse);
		switch (lParam)
		{
		case WM_LBUTTONDOWN:
			::SetForegroundWindow(hWnd);
			break;
		case WM_RBUTTONDOWN:
			if (wnd.isUseTrayMenu)
			{
				::SetForegroundWindow(hWnd);
				int nMenuId = ::TrackPopupMenu(wnd.hTrayMenu, TPM_RETURNCMD, ptMouse.x, ptMouse.y, 0, hWnd, nullptr);
				if (nMenuId == 0) ::PostMessage(hWnd, WM_LBUTTONDOWN, 0, 0);
				if (wnd.TrayMenuProcFunc)
					wnd.TrayMenuProcFunc(nMenuId);

			}
			break;
		}
	}
}

void EasyWindowController::OnTaskBarCreated(EasyWindowIndex index) noexcept
{
	if (vecWindowStruct[index].isUseTray)
		Self::ShowTray(&vecWindowStruct[index].nid);
}

void EasyWindowController::OnPaint(EasyWindowIndex index) noexcept
{
	const auto& wnd = vecWindowStruct[index];
	if (wnd.pCanvas)
	{
		const auto hdc = ::GetDC(wnd.hWnd);
		if (wnd.bStretch)
			wnd.pCanvas->StretchBlt(hdc, 0, 0, wnd.Size.cx, wnd.Size.cy, 0, 0, 0, 0, SRCCOPY);
		else
			wnd.pCanvas->BitBlt(hdc, NULLPTSZ, NULLPOINT, SRCCOPY);
		::ReleaseDC(wnd.hWnd, hdc);
	}
}

void EasyWindowController::OnPaint(EasyWindowIndex index, HDC hdc) noexcept
{
}

void EasyWindowController::OnMove(EasyWindowIndex index) noexcept {}

void EasyWindowController::OnDestroy(EasyWindowIndex index, WPARAM wParam) noexcept
{
	Self::CloseWindowIndexBase(index);
	if (wParam)
		::DestroyWindow(vecWindowStruct[index].hWnd);
}

void EasyWindowController::CloseWindowIndexBase(EasyWindowIndex index) noexcept
{
	if (!vecWindowStruct[index].isAlive)
		return;
	auto& wnd = vecWindowStruct[index];
	Self::EndWindow(index);
	Self::BeginProcessingIndex(index);
	Self::WaitForTaskIndex(index);
	if (wnd.hParent)
	{
		::EnableWindow(wnd.hParent, true);
		::SetForegroundWindow(wnd.hParent);
	}
	Self::DeleteTrayIndex(index);
	Self::FreeWindowIndex(index);
	Self::EndProcessingIndex(index);
}

HWND EasyWindowController::OnSysCtrlCreate(EasyWindowIndex index, WPARAM wParam, LPARAM lParam) noexcept
{
	CREATESTRUCT* c = (CREATESTRUCT*)lParam;
	HWND hWnd = CreateWindow(c->lpszClass, c->lpszName, c->style, c->x, c->y, c->cx, c->cy, c->hwndParent, c->hMenu, GetModuleHandle(0), c->lpCreateParams);
	vecWindowStruct[index].vecSysCtrl.push_back((EasySysCtrlBase*)wParam);
	return hWnd;
}

LRESULT EasyWindowController::SysCtrlProc(EasyWindowIndex index, UINT msg, WPARAM wParam, LPARAM lParam, bool& bRet) noexcept
{
	auto& wnd = vecWindowStruct[index];
	if (msg == WM_SYSCTRL_CREATE)
	{
		wnd.bHasCtrl = true;
		bRet = true;
		return (LRESULT)OnSysCtrlCreate(index, wParam, lParam);
	}
	else if (msg == WM_SYSCTRL_DELETE)
	{
		const int size = (int)wnd.vecSysCtrl.size();
		const EasySysCtrlBase* pSysCtrlBase = (EasySysCtrlBase*)wParam;
		for (size_t i = 0; i < size; ++i)
		{
			if (wnd.vecSysCtrl[i] == pSysCtrlBase)
				wnd.vecSysCtrl[i] = nullptr;
		}

		bRet = true;
		return 0;
	}
	if (wnd.bHasCtrl)
	{
		bool bCtrlRet = false;
		LRESULT lr = 0;
		for (auto& pCtrl : wnd.vecSysCtrl)
			if (pCtrl)
			{
				/*
				LRESULT lr = pCtrl->UpdateMessage(msg, wParam, lParam, bCtrlRet);
				if (bCtrlRet)
				{
					bRet = true;
					return lr;
				}
				*/
			}
	}
	bRet = false;
	return 0;
}

void EasyWindowController::OnCreate(EasyWindowIndex index, HWND hWnd, LPARAM lParam) noexcept {}

LRESULT CALLBACK EasyWindowController::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	LRESULT resultUserProc = EWINDOW_DEFAULT_PROC;
	EasyWindowIndex index = Self::GetWindowIndex(hWnd);
	if (!isValidWindowIndex(index))
	{
		if (msg == WM_CREATE)
		{
			index = (int)vecWindowStruct.size() - 1;
			Self::OnCreate(index, hWnd, lParam);
			WNDPROC proc = vecWindowStruct[index].WndProcFunc;
			if (proc)
				proc(hWnd, msg, wParam, lParam);
		}
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	Self::RegisterMessageIndex(index, msg, wParam, lParam);
	if (msg == WM_SIZE)
		Self::OnSize(index);
	else if (msg == WM_TRAY)
		Self::OnTray(index, lParam);
	else if (msg == WM_USER_REDRAW)
	{
		Self::OnTray(index, lParam);
		return 0;
	}
	else if (msg == WM_TASKBARCREATED)
		Self::OnTaskBarCreated(index);

	bool bRetSysCtrl = false;
	LRESULT lrSysCtrl = Self::SysCtrlProc(index, msg, wParam, lParam, bRetSysCtrl);
	if (bRetSysCtrl)
		return lrSysCtrl;

	if (!vecWindowStruct[index].WndProcFunc)
		vecWindowStruct[index].WndProcFunc = ::DefWindowProc;
	resultUserProc = vecWindowStruct[index].WndProcFunc(hWnd, msg, wParam, lParam);
	switch (msg)
	{
	case WM_PAINT:
		Self::OnPaint(index);
		::DefWindowProc(hWnd, WM_PAINT, 0, 0);
		break;
	case WM_MOVE:
		Self::OnMove(index);
		break;
	case WM_CLOSE:
		::DestroyWindow(vecWindowStruct[index].hWnd);
		break;
	case WM_DESTROY:
		Self::OnDestroy(index, wParam);
		::PostQuitMessage(0);
		break;
	}
	return resultUserProc;
}

void EasyWindowController::InitWindowBase(const EasyWindowSets& WPS, int* nDoneFlag, HWND* hWnd) noexcept
{
	std::wstring wstrTitle;
	EasyWindowStruct wnd;
	int nFrameW, nFrameH;
	EasyWindowIndex index;
	{
		EasyWindowLock2 lock(EasyWindowMutex);
		if (!WPS.lpszWindowName || WPS.lpszWindowName[0] == L'\0')
		{
			wstrTitle = L"EasyG" _EASYG_VER_STR_;
			if (WndCount)
				wstrTitle += L'(' + std::to_wstring(WndCount + 1) + L')';
		}
		else
			wstrTitle = WPS.lpszWindowName;

		int user_style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WPS.Style;
		if (WPS.Flag & EWF_NOMINIMIZE)
			user_style &= ~WS_MINIMIZEBOX & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;
		if (WPS.Flag & EWF_DBLCLKS)
			user_style |= CS_DBLCLKS;
		if (WPS.Flag & EWF_SHOWCONSOLE)
			EasyHandle::ShowConsole();
		else if (WPS.Flag & EWF_HIDECONSOLE)
			EasyHandle::HideConsole();

		wnd.Init(WPS.hParent, WPS.ptsz.cx, WPS.ptsz.cy, WPS.WndProc);
		vecWindowStruct.push_back(wnd);
		index = (EasyWindowIndex)vecWindowStruct.size();
		for (int i = 0;; i++)
		{
			wnd.hWnd = ::CreateWindowEx(0, _T("EasyWindowClass"), wstrTitle.c_str(), user_style, CW_USEDEFAULT, CW_USEDEFAULT, WPS.ptsz.cx, WPS.ptsz.cy,
				WPS.hParent, nullptr, EasyHandle::hInstance, nullptr);
			if (wnd.hWnd)
			{
				vecWindowStruct[index - 1].hWnd = wnd.hWnd;
				break;
			}
			else if (i == 2)
			{
				std::wstring str = std::to_wstring(GetLastError());
				MessageBox(nullptr, (L"Error creating window: " + str).c_str(), L"[Error]", MB_OK | MB_ICONERROR);
				*nDoneFlag = -1;
				EW_CreateCV.notify_all();
				return;
			}
		}
		if (WPS.Flag & EWF_NOCLOSE)
		{
			HMENU hmenu = ::GetSystemMenu(wnd.hWnd, false);
			::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);
		}

		*hWnd = wnd.hWnd;
		++WndCount;
		++AliveWndCount;
		*nDoneFlag = 1;
		EW_CreateCV.notify_all();
		RECT rcClient, rcWnd;
		::GetClientRect(wnd.hWnd, &rcClient);
		::GetWindowRect(wnd.hWnd, &rcWnd);
		nFrameW = (rcWnd.right - rcWnd.left) - rcClient.right;
		nFrameH = (rcWnd.bottom - rcWnd.top) - rcClient.bottom;
		::SetWindowPos(wnd.hWnd, HWND_TOP, WPS.ptsz.x, WPS.ptsz.y, WPS.ptsz.cx + nFrameW, WPS.ptsz.cy + nFrameH, WPS.bPrePos ? 0 : SWP_NOMOVE);
		::ShowWindow(wnd.hWnd, SW_SHOWNORMAL);
		::UpdateWindow(wnd.hWnd);
	}
	MSG Msg;
	while (GetMessage(&Msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
}

HWND EasyWindowController::InitWindow(const EasyWindowSets& WPS) noexcept
{
	int nDoneFlag = 0;
	HWND hWnd;
	if (WPS.hParent)
		::EnableWindow(WPS.hParent, false);
	std::thread(Self::InitWindowBase, WPS, &nDoneFlag, &hWnd).detach();
	EasyWindowLock lock(EasyWindowMutex);
	while (nDoneFlag == 0)
		EW_CreateCV.wait(lock);
	if (nDoneFlag == -1)
	{
		if (WPS.hParent)
			::EnableWindow(WPS.hParent, true);
		return nullptr;
	}
	return hWnd;
}

void EasyWindowController::ShowConsole(UINT style) noexcept
{
	::ShowWindow(EasyHandle::hConsole, style);
}

void EasyWindowController::HideConsole() noexcept
{
	Self::ShowConsole(SW_HIDE);
}

EasyWindow::EasyWindow(const EasyWindowSets& WPS) noexcept
{
	this->Create(WPS);
}

EasyWindow::~EasyWindow() noexcept
{
	this->Destroy();
}

HWND EasyWindow::Create(const EasyWindowSets& WPS) noexcept
{
	if (_isCreated)
		return nullptr;
	_isCreated = true;
	hWnd = EasyWindowController::InitWindow(WPS);
	wndIndex = EasyWindowController::GetWindowIndex(hWnd);
	return hWnd;
}

HWND EasyWindow::Detach() noexcept
{
	if (!_isCreated)
		return nullptr;
	_isCreated = false;
	HWND hwnd = hWnd;
	hWnd = nullptr;
	wndIndex = EWF_NOWINDOW_INDEX;
	return hwnd;
}

void EasyWindow::Destroy() noexcept
{
	EasyWindowController::CloseWindowIndex(wndIndex);
	this->Detach();
}

HWND EasyWindow::GetHandle() const noexcept
{
	return hWnd;
}

EasyWindowIndex EasyWindow::GetIndex() const noexcept
{
	return wndIndex;
}

EasyWindowStruct& EasyWindow::GetInfo() const noexcept
{
	static EasyWindowStruct nullStu;
	if (_isCreated)
		return vecWindowStruct[wndIndex];
	nullStu = {};
	return nullStu;
}

void EasyWindow::SetCanvas(const EasyImage2* canvas) noexcept
{
	if (_isCreated)
		vecWindowStruct[wndIndex].pCanvas = canvas;
}

const EasyImage2* EasyWindow::GetCanvas() const noexcept
{
	return _isCreated ? vecWindowStruct[wndIndex].pCanvas : nullptr;
}

void EasyWindow::SetProcFunc(WNDPROC wndProc) noexcept
{
	if (_isCreated)
		vecWindowStruct[wndIndex].WndProcFunc = wndProc;
}

WNDPROC EasyWindow::GetProcFunc() const noexcept
{
	return _isCreated ? vecWindowStruct[wndIndex].WndProcFunc : nullptr;
}

bool EasyWindow::isCreated() const noexcept
{
	return _isCreated;
}

bool EasyWindow::isAlive() const noexcept
{
	return _isCreated ? vecWindowStruct[wndIndex].isAlive : false;
}

void EasyWindow::SetQuickDraw(UINT nSkipPixels) noexcept
{
	if (_isCreated)
		vecWindowStruct[wndIndex].nSkipPixels = nSkipPixels;
}

void EasyWindow::Redraw() noexcept
{
	if (_isCreated)
	{
		::InvalidateRect(hWnd, nullptr, false);
		::UpdateWindow(hWnd);
	}
}

void EasyWindow::BeginTask() noexcept
{
	if (_isCreated)
		EasyWindowController::BeginTaskIndex(wndIndex);
}

void EasyWindow::EndTask() noexcept
{
	if (_isCreated)
		EasyWindowController::EndTaskIndex(wndIndex);
}

bool EasyWindow::isInTask() const noexcept
{
	return _isCreated ? vecWindowStruct[wndIndex].isInTask : false;
}

void EasyWindow::WaitForTask() const noexcept
{
	if (_isCreated)
		EasyWindowController::WaitForProcessingIndex(wndIndex);
}

void EasyWindow::BeginProcessing() noexcept
{
	if (_isCreated)
		EasyWindowController::BeginProcessingIndex(wndIndex);
}

void EasyWindow::EndProcessing() noexcept
{
	if (_isCreated)
		EasyWindowController::EndProcessingIndex(wndIndex);
}

bool EasyWindow::isInProcessing() const noexcept
{
	return _isCreated ? vecWindowStruct[wndIndex].isBusyProcessing : false;
}

void EasyWindow::WaitForProcessing() const noexcept
{
	if (_isCreated)
		EasyWindowController::WaitForProcessingIndex(wndIndex);
}

void EasyWindow::CreateTray(LPCTSTR lpszTrayName) noexcept
{
	if (_isCreated)
		EasyWindowController::CreateTrayIndex(wndIndex, lpszTrayName);
}

void EasyWindow::DeleteTray() noexcept
{
	if (_isCreated)
		EasyWindowController::DeleteTrayIndex(wndIndex);
}

void EasyWindow::SetTrayMenu(HMENU hMenu) noexcept
{
	if (_isCreated)
		EasyWindowController::SetTrayMenuIndex(wndIndex, hMenu);
}

void EasyWindow::SetTrayMenuProcFunc(TRAYPROC pFunc) noexcept
{
	if (_isCreated)
		EasyWindowController::SetTrayMenuProcFuncIndex(wndIndex, pFunc);
}

DWORD EasyWindow::SetStyle(DWORD lNewStyle) noexcept
{
	if (_isCreated)
		return (DWORD)::SetWindowLongPtr(hWnd, GWL_STYLE, lNewStyle);
	return 0;
}

DWORD EasyWindow::GetStyle() const noexcept
{
	if (_isCreated)
		return (DWORD)::GetWindowLongPtr(hWnd, GWL_STYLE);
	return 0;
}

DWORD EasyWindow::SetExStyle(DWORD lNewExStyle) noexcept
{
	if (_isCreated)
		return (DWORD)::SetWindowLongPtr(hWnd, GWL_EXSTYLE, lNewExStyle);
	return 0;
}

DWORD EasyWindow::GetExStyle() const noexcept
{
	if (_isCreated)
		return (DWORD)::GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	return 0;
}

POINT EasyWindow::GetPos() const noexcept
{
	if (_isCreated)
		return EasyHandle::GetWindowPos(hWnd);
	return {};
}

LONG EasyWindow::GetX() const noexcept
{
	return this->GetPos().x;
}

LONG EasyWindow::GetY() const noexcept
{
	return this->GetPos().y;
}

SIZE EasyWindow::GetWindowSize() const noexcept
{
	if (_isCreated)
		return EasyHandle::GetWindowSize(hWnd);
	return EasyHandle::screenSize.size;
}

LONG EasyWindow::GetWindowCX() const noexcept
{
	return this->GetWindowSize().cx;
}

LONG EasyWindow::GetWindowCY() const noexcept
{
	return this->GetWindowSize().cy;
}

SIZE EasyWindow::GetClientSize() const noexcept
{
	if (_isCreated)
		return vecWindowStruct[wndIndex].Size;
	return EasyHandle::screenSize.size;
}

LONG EasyWindow::GetClientCX() const noexcept
{
	return this->GetClientSize().cx;
}

LONG EasyWindow::GetClientCY() const noexcept
{
	return this->GetClientSize().cy;
}

void EasyWindow::Move(LONG x, LONG y) noexcept
{
	if (_isCreated)
		EasyHandle::SetWindowPos(hWnd, x, y);
}

void EasyWindow::MoveRel(LONG dx, LONG dy) noexcept
{
	if (_isCreated)
		EasyHandle::SetWindowPosRel(hWnd, dx, dy);
}

void EasyWindow::Resize(LONG cx, LONG cy) noexcept
{
	if (_isCreated)
		EasyHandle::SetWindowSize(hWnd, cx, cy);
}

bool EasyWindow::isForegroundWindow() const noexcept
{
	if (_isCreated)
		return ::GetForegroundWindow() == hWnd;
	return false;
}

EasyMessage EasyWindow::GetMessage(BYTE filter) noexcept
{
	return EasyWindowController::GetMessageIndex(wndIndex, filter);
}

void EasyWindow::GetMessage(EasyMessage* msg, BYTE filter) noexcept
{
	EasyWindowController::GetMessageIndex(wndIndex, msg, filter);
}

bool EasyWindow::PeekMessage(EasyMessage* msg, BYTE filter, bool remove) noexcept
{
	return EasyWindowController::PeekMessageIndex(wndIndex, msg, filter, remove);
}

void EasyWindow::ClearMessage(BYTE filter) noexcept
{
	EasyWindowController::ClearMsgIndex(wndIndex, filter);
}

_EASYG_END