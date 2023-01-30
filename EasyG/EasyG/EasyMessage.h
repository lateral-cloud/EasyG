#pragma once

#include "EasyGHead.h"

_EASYG_BEGIN

struct EasyMessageMouse
{
	bool lbutton : 1;
	bool rbutton : 1;
	bool shift : 1;
	bool ctrl : 1;
	bool mbutton : 1;
	bool xbutton1 : 1;
	bool xbutton2 : 1;
	union
	{
		short wheel;
		struct
		{
			bool is_xbutton1 : 1;
			bool is_xbutton2 : 1;
		};
	};
private:
	int nothing1;
public:
	short x;
	short y;
};

struct EasyMessageKey
{
	BYTE vkcode;
private:
	int nothing1;
public:
	short count;
	BYTE scancode;
	bool extended : 1;
private:
	bool nothing : 4;
public:
	bool contextcode : 1;
	bool prevdown : 1;
	bool transstatus : 1;
};

struct EasyMessageWindow
{
	short active;
	short mininstate;
private:
	int nothing1;
public:
	HWND hwnd;
};

struct EasyMessageCommand
{
	HWND hwnd;
	bool lbutton : 1;
	bool rbutton : 1;
	bool shift : 1;
	bool ctrl : 1;
	bool mbutton : 1;
	bool xbutton1 : 1;
	bool xbutton2 : 1;
	short command;

	short GetAppCommand() const noexcept
	{
		return command & ~0xf000;
	}
	WORD GetDeviceCommand() const noexcept
	{
		return command & 0xf000;
	}
};

class EasyMessage
{
public:
	HWND hwnd;
	UINT message;
	DWORD time;
	POINT pt;
	union
	{
		struct
		{
			WPARAM wParam;
			LPARAM lParam;
		};

		EasyMessageMouse mouse;

		EasyMessageKey key;

		EasyMessageCommand cmd;
	};

	EasyMessage() noexcept = default;
	EasyMessage(const MSG& msg) noexcept : hwnd(msg.hwnd), message(msg.message), time(msg.time), pt(msg.pt), wParam(msg.wParam), lParam(msg.lParam) {}
	operator MSG& () noexcept { return *(MSG*)this; }
	operator const MSG& () const noexcept { return *(const MSG*)this; }
};

constexpr BYTE EMT_UNKNOW = 0x00, EMT_MOUSE = 0x01, EMT_KEY = 0x02, EMT_CHAR = 0x04, EMT_WINDOW = 0x08;
constexpr BYTE EMT_ALLMSG = EMT_MOUSE | EMT_KEY | EMT_CHAR | EMT_WINDOW;

BYTE GetMessageType(UINT message) noexcept
{
	switch (message)
	{
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_RBUTTONDBLCLK:
		return EMT_MOUSE;
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		return EMT_KEY;
	case WM_CHAR:
		return EMT_CHAR;
	case WM_ACTIVATE:
	case WM_MOVE:
	case WM_SIZE:
		return EMT_WINDOW;
	default:
		return EMT_UNKNOW;
	}
}

_EASYG_END