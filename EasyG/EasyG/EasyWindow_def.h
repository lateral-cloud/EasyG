#pragma once

#include "EasyImage.h"
#include "EasyMessage.h"
#include <vector>
#include <condition_variable>
#include <mutex>
#include <string>

_EASYG_BEGIN

class EasySysCtrlBase;
struct EasyWindowStruct;

typedef void(*TRAYPROC)(UINT);
typedef int									EasyWindowIndex;
template<class _Ty>
using EasyWindowVec = std::vector<_Ty>;
typedef std::mutex							EasyWindowMutexT;
typedef std::condition_variable				EasyWindowCVT;
typedef std::unique_lock<EasyWindowMutexT>	EasyWindowLock;
typedef std::lock_guard<EasyWindowMutexT>	EasyWindowLock2;
typedef EasyWindowVec<EasyWindowStruct>		EasyWindowStuVecT;
typedef EasyWindowVec<EasyMessage>			EasyWindowMegVecT;
typedef EasyWindowVec<EasySysCtrlBase*>		EasyWindowSysCtrlVecT;
typedef PTSZ								ScreenSize;

#define EWF_NOWINDOW_INDEX					-1
#define EWINDOW_DEFAULT_PROC				((LRESULT)(-10086))
#define EWF_MSG_RESERVE_SIZE				100
#define EWF_SYSCTRL_RESERVE_SIZE			100
#define EWF_NORMAL							0x00
#define EWF_SHOWCONSOLE						0x01
#define EWF_NOCLOSE							0x02
#define EWF_NOMINIMIZE						0x04
#define EWF_DBLCLKS							0x08
#define EWF_HIDECONSOLE						0x10

struct EasyWindowStruct
{
	NOTIFYICONDATA			nid{};
	EasyWindowMegVecT		vecMessage{ EWF_MSG_RESERVE_SIZE };
	EasyWindowSysCtrlVecT	vecSysCtrl{ EWF_SYSCTRL_RESERVE_SIZE };
	HWND					hWnd = nullptr;
	HWND					hParent = nullptr;
	HMENU					hTrayMenu = nullptr;
	WNDPROC					WndProcFunc = nullptr;
	TRAYPROC				TrayMenuProcFunc = nullptr;
	SIZE					Size{};
	SIZE					FrameSize{};
	const EasyImage2*		pCanvas = nullptr;
	int						nSkipPixels = 0;
	bool					isAlive = false;
	bool					isUseTray = false;
	bool					isUseTrayMenu = false;
	bool					isBusyProcessing = false;
	bool					bHasCtrl = false;
	bool					isInTask = false;
	bool					bStretch = false;
	void Init(HWND hparent, const EasyImage2* pcanvas, WNDPROC windowprocess) noexcept;
	void Init(HWND hparent, int cx, int cy, WNDPROC windowprocess) noexcept;
};

struct EasyWindowSets
{
	PTSZ ptsz;
	WNDPROC WndProc = nullptr;
	HWND hParent = nullptr;
	LPCTSTR lpszWindowName = L"";
	UINT Style = 0;
	UINT Flag = 0;
	bool bPrePos = false;
	EasyWindowSets(LPCTSTR WindowName, const SIZE& size, WNDPROC wndproc, HWND hparent, UINT flag, UINT style) noexcept
		:lpszWindowName(WindowName), ptsz({}, size), WndProc(wndproc), hParent(hparent), Flag(flag), Style(style) {}
	EasyWindowSets(LPCTSTR WindowName, const SIZE& size, WNDPROC wndproc, HWND hparent) noexcept
		: lpszWindowName(WindowName), ptsz({}, size), WndProc(wndproc), hParent(hparent) {}
	EasyWindowSets(LPCTSTR WindowName, const SIZE& size, UINT flag, UINT style) noexcept
		: lpszWindowName(WindowName), ptsz({}, size), Flag(flag), Style(style) {}
	EasyWindowSets(LPCTSTR WindowName, const SIZE& size) noexcept
		: lpszWindowName(WindowName), ptsz({}, size) {}
};

class EasyWindowController
{
public:
	typedef EasyWindowController Self;
protected:
	static EasyWindowMutexT EasyWindowMutex;
	static EasyWindowCVT EW_ProcCV;
	static EasyWindowCVT EW_TaskCV;
	static EasyWindowCVT EW_AliveCV;
	static EasyWindowCVT EW_MsgCV;
	static EasyWindowCVT EW_CreateCV;
	static EasyWindowStuVecT vecWindowStruct;
	static UINT WndCount;
	static UINT AliveWndCount;
	EasyWindowController() noexcept = default;
	EasyWindowController(EasyWindowController&&) noexcept = default;
	EasyWindowController(const EasyWindowController&) noexcept = default;
	EasyWindowController& operator=(EasyWindowController&&) noexcept = default;
	EasyWindowController& operator=(const EasyWindowController&) noexcept = default;
	~EasyWindowController() noexcept = default;
public:
	static const UINT WM_TRAY;
	static const UINT WM_SYSCTRL_CREATE;
	static const UINT WM_SYSCTRL_DELETE;
	static const UINT WM_USER_REDRAW;
	static const UINT WM_TASKBARCREATED;

	static EasyWindowStruct& GetWindowStruct(EasyWindowIndex index) noexcept;
	static bool isValidWindowIndex(EasyWindowIndex index) noexcept;
	static bool isAliveWindowIndex(EasyWindowIndex index) noexcept;
	static EasyWindowIndex GetWindowIndex(HWND hWnd) noexcept;
	static EasyWindowIndex RegisterWindowStu(const EasyWindowStruct& wndStu) noexcept;
	static bool isAnyWindow() noexcept;
	static void WaitForProcessingIndex(EasyWindowIndex index) noexcept;
	static void BeginProcessingIndex(EasyWindowIndex index) noexcept;
	static void EndProcessingIndex(EasyWindowIndex index) noexcept;
	static void WaitForTaskIndex(EasyWindowIndex index) noexcept;
	static void BeginTaskIndex(EasyWindowIndex index) noexcept;
	static void EndTaskIndex(EasyWindowIndex index) noexcept;
	static void WaitForEnd(EasyWindowIndex index) noexcept;
	static void EndWindow(EasyWindowIndex index) noexcept;
	static void FreeWindowIndex(EasyWindowIndex index) noexcept;
	static void CloseWindowIndex(EasyWindowIndex index) noexcept;
	static void SetWndProcIndex(EasyWindowIndex index, WNDPROC WindowProcess) noexcept;
	static void BindWindowCanvasIndex(EasyWindowIndex index, const EasyImage2* canvas) noexcept;
	static void WaitExit() noexcept;
	static void AutoExit() noexcept;
	static void QuickDraw(EasyWindowIndex index, UINT nSkipPixels) noexcept;
	static void ShowTray(NOTIFYICONDATA* nid) noexcept;
	static void CreateTrayIndex(EasyWindowIndex index, LPCTSTR lpszTrayName, HICON hIcon = nullptr) noexcept;
	static void DeleteTrayIndex(EasyWindowIndex index) noexcept;
	static void SetTrayMenuIndex(EasyWindowIndex index, HMENU hMenu) noexcept;
	static void SetTrayMenuProcFuncIndex(EasyWindowIndex index, TRAYPROC pFunc) noexcept;
	static EasyWindowMegVecT& GetMsgVecIndex(EasyWindowIndex index) noexcept;
	static void RemoveMsgIndex(EasyWindowIndex index) noexcept;
	static void ClearMsgIndex(EasyWindowIndex index, BYTE filter) noexcept;
	static bool isNewMessageIndex(EasyWindowIndex index, BYTE filter) noexcept;
	static EasyMessage GetNextMessageIndex(EasyWindowIndex index, BYTE filter) noexcept;
	static void GetMessageIndex(EasyWindowIndex index, EasyMessage* pmsg, BYTE filter) noexcept;
	static EasyMessage GetMessageIndex(EasyWindowIndex index, BYTE filter) noexcept;
	static bool PeekMessageIndex(EasyWindowIndex index, EasyMessage* pmsg, BYTE filter, bool removemsg = true) noexcept;
	static void RegisterMessageIndex(EasyWindowIndex index, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static void ShowConsole(UINT style = SW_SHOW) noexcept;
	static void HideConsole() noexcept;
protected:
	static void OnSize(EasyWindowIndex index) noexcept;
	static void OnTray(EasyWindowIndex index, LPARAM lParam) noexcept;
	static void OnTaskBarCreated(EasyWindowIndex index) noexcept;
	static void OnPaint(EasyWindowIndex index) noexcept;
	static void OnPaint(EasyWindowIndex index, HDC hdc) noexcept;
	static void OnMove(EasyWindowIndex index) noexcept;
	static void OnDestroy(EasyWindowIndex index, WPARAM wParam) noexcept;
	static HWND OnSysCtrlCreate(EasyWindowIndex index, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT SysCtrlProc(EasyWindowIndex index, UINT msg, WPARAM wParam, LPARAM lParam, bool& bRet) noexcept;
	static void OnCreate(EasyWindowIndex index, HWND hWnd, LPARAM lParam) noexcept;
	static void CloseWindowIndexBase(EasyWindowIndex index) noexcept;
	static void InitWindowBase(const EasyWindowSets& WPS, int* nDoneFlag, HWND* hWnd) noexcept;
public:
	static HWND InitWindow(const EasyWindowSets& WPS) noexcept;
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};

class EasyWindow : public EasyWindowController
{
protected:
	EasyWindowIndex wndIndex = EWF_NOWINDOW_INDEX;
	HWND hWnd = nullptr;
	bool _isCreated = false;
	using EasyWindowController::InitWindow;
	using EasyWindowController::WndProc;
public:
	EasyWindow() noexcept = default;
	EasyWindow(const EasyWindowSets& WPS) noexcept;
	~EasyWindow() noexcept;

	HWND Create(const EasyWindowSets& WPS) noexcept;
	HWND Detach() noexcept;
	void Destroy() noexcept;
	HWND GetHandle() const noexcept;
	EasyWindowIndex GetIndex() const noexcept;
	EasyWindowStruct& GetInfo() const noexcept;
	void SetCanvas(const EasyImage2* canvas) noexcept;
	const EasyImage2* GetCanvas() const noexcept;
	void SetProcFunc(WNDPROC wndProc) noexcept;
	WNDPROC GetProcFunc() const noexcept;
	bool isCreated() const noexcept;
	bool isAlive() const noexcept;
	void SetQuickDraw(UINT nSkipPixels) noexcept;
	void Redraw() noexcept;
	void BeginTask() noexcept;
	void EndTask() noexcept;
	bool isInTask() const noexcept;
	void WaitForTask() const noexcept;
protected:
	void BeginProcessing() noexcept;
	void EndProcessing() noexcept;
public:
	bool isInProcessing() const noexcept;
	void WaitForProcessing() const noexcept;
	void CreateTray(LPCTSTR lpszTrayName) noexcept;
	void DeleteTray() noexcept;
	void SetTrayMenu(HMENU hMenu) noexcept;
	void SetTrayMenuProcFunc(TRAYPROC pFunc) noexcept;
	DWORD SetStyle(DWORD lNewStyle) noexcept;
	DWORD GetStyle() const noexcept;
	DWORD SetExStyle(DWORD lNewExStyle) noexcept;
	DWORD GetExStyle() const noexcept;
	POINT GetPos() const noexcept;
	LONG GetX() const noexcept;
	LONG GetY() const noexcept;
	SIZE GetWindowSize() const noexcept;
	LONG GetWindowCX() const noexcept;
	LONG GetWindowCY() const noexcept;
	SIZE GetClientSize() const noexcept;
	LONG GetClientCX() const noexcept;
	LONG GetClientCY() const noexcept;
	void Move(LONG x, LONG y) noexcept;
	void MoveRel(LONG dx, LONG dy) noexcept;
	void Resize(LONG cx, LONG cy) noexcept;
	bool isForegroundWindow() const noexcept;
	EasyMessage GetMessage(BYTE filter = EMT_ALLMSG) noexcept;
	void GetMessage(EasyMessage* msg, BYTE filter = EMT_ALLMSG) noexcept;
	bool PeekMessage(EasyMessage* msg, BYTE filter = EMT_ALLMSG, bool remove = true) noexcept;
	void ClearMessage(BYTE filter = EMT_ALLMSG) noexcept;
};

_EASYG_END